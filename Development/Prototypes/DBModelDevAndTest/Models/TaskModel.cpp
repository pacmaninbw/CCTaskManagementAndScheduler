// Project Header Files
#include "commonTestValues.h"
#include "commonUtilities.h"
#include "GenericDictionary.h"
#include "chronoBoostConversions.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <algorithm>
#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

static const TaskModel::TaskStatus UnknowStatus = static_cast<TaskModel::TaskStatus>(-1);

static std::vector<GenericDictionary<TaskModel::TaskStatus, std::string>::DictType> statusConversionsDefs = {
    {TaskModel::TaskStatus::Not_Started, "Not Started"},
    {TaskModel::TaskStatus::On_Hold, "On Hold"},
    {TaskModel::TaskStatus::Waiting_for_Dependency, "Waiting for Dependency"},
    {TaskModel::TaskStatus::Work_in_Progress, "Work in Progress"},
    {TaskModel::TaskStatus::Complete, "Completed"}
};

static GenericDictionary<TaskModel::TaskStatus, std::string> taskStatusConversionTable(statusConversionsDefs);

TaskModel::TaskModel()
: ModelDBInterface("Task", "TaskID")
{
  m_creatorID = 0;
  m_assignToID = 0;
  m_description = "";
  m_estimatedEffort = 0.0;
  m_actualEffort = 0.0;
  m_priorityCategory = 0;
  m_priority = 0;
  m_personal = false;
}

TaskModel::TaskModel(const TaskDbQueryValues &dbTranslator)
: TaskModel()
{
    m_primaryKey = dbTranslator.task_id;
    m_creatorID = dbTranslator.created_by;
    m_assignToID = dbTranslator.assigned_to;
    m_description = dbTranslator.description;
    m_status = static_cast<TaskModel::TaskStatus>(dbTranslator.task_status.value_or(0));
    m_dueDate = common::toChronoDate(dbTranslator.due_date);
    m_planedStart = common::toChronoDate(dbTranslator.planned_start);
    m_actualStart = dbTranslator.actual_start.transform(common::toChronoDate);
    m_estimatedCompletion = dbTranslator.estimated_delivery.transform(common::toChronoDate);
    m_completed = dbTranslator.delivered.transform(common::toChronoDate);
    m_estimatedEffort = dbTranslator.est_hours_effort;
    m_actualEffort = dbTranslator.hours_effort;
    m_priorityCategory = dbTranslator.priority_category;
    m_priority = dbTranslator.priority;
    m_personal = dbTranslator.personal;
    m_createdTimeStamp = common::toChronoTimePoint(dbTranslator.creation_timestamp);
    m_lastUpdateTimeStamp = common::toChronoTimePoint(dbTranslator.last_modified_time_stamp);
    m_deleted = dbTranslator.deleted;
    m_parentTaskID = dbTranslator.dependent_task;
    m_lastModifiedByUser = dbTranslator.last_modified_by_user;
}

TaskModel::TaskModel(std::size_t creatorID)
: TaskModel()
{
    setCreatorID(creatorID);
    setAssignToID(creatorID);
}

TaskModel::TaskModel(std::size_t creatorID, std::string description)
: TaskModel()
{
    setCreatorID(creatorID);
    setAssignToID(creatorID);
    setDescription(description);
}

bool TaskModel::insert() noexcept
{
    if (!preInsertCheck())
    {
        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatInsertStatement());
        m_primaryKey =  getPrimaryKeyValue(localResult);

        if (m_parentTaskID.has_value())
        {
            if (m_parentTaskID.value() != 0 && m_parentTaskID.value() != m_primaryKey)
            {
                insertDependency();
            }
        }

        m_modified = false;

        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.insert : {}", getModelName(), e.what()));
        return false;
    }
}

bool TaskModel::update() noexcept
{
    if (!preUpdateCheck())
    {
        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatUpdateStatement());

        if (m_parentTaskID.has_value())
        {
            updateDependency();
        }

        m_modified = false;
            
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.update : {}", getModelName(), e.what()));
        return false;
    }
}

bool TaskModel::hide(std::size_t userRequestingDelete) noexcept
{
    clearErrorMessages();

    if (!isInDataBase())
    {
        appendErrorMessage(std::format("{} not in Database, nothing to delete!", getModelName()));

        return false;
    }

    if (userRequestingDelete != m_creatorID)
    {
        appendErrorMessage(std::format("Permission denied, deleting user ({}) is not the creator ({}) of the task", userRequestingDelete, m_creatorID));

        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatDeleteStatement());

        m_deleted = true;
        
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.hide() : {}", getModelName(), e.what()));
        return false;
    }
}

void TaskModel::addEffortHours(double hours)
{
    double actualEffortHours = getactualEffortToDate();
    actualEffortHours += hours;
    setActualEffortToDate(actualEffortHours);
}

std::string TaskModel::getStatusStringVal() const
{
    TaskModel::TaskStatus status = getStatus();
    auto statusName = taskStatusConversionTable.lookupName(status);
    return statusName.has_value()? *statusName : "Unknown TaskStatus Value";
}

std::chrono::year_month_day TaskModel::getactualStartDate() const
{
    return m_actualStart.value_or(std::chrono::year_month_day());
}

std::chrono::year_month_day TaskModel::getEstimatedCompletion() const
{
    return m_estimatedCompletion.value_or(std::chrono::year_month_day());
}

std::chrono::year_month_day TaskModel::getCompletionDate() const
{
    return m_completed.value_or(std::chrono::year_month_day());
}

void TaskModel::setCreatorID(std::size_t creatorID)
{
    m_modified = true;
    m_creatorID = creatorID;
}

void TaskModel::setAssignToID(std::size_t assignedID)
{
    m_modified = true;
    m_assignToID = assignedID;
}

void TaskModel::setDescription(std::string description)
{
    m_modified = true;
    m_description = description;
}

void TaskModel::setStatus(TaskModel::TaskStatus status)
{
    m_modified = true;
    m_status = status;

    if (m_status == TaskModel::TaskStatus::Work_in_Progress)
    {
        if (!m_actualStart.has_value())
        {
            setactualStartDate(common::getTodaysDate());
        }
    }

    if (m_status == TaskModel::TaskStatus::Complete)
    {
        setCompletionDate(common::getTodaysDate());
    }
}

void TaskModel::setParentTaskID(std::size_t parentTaskID)
{
    if (parentTaskID == 0)
    {
        appendErrorMessage(std::format("In TaskModel.{} : {}", __func__, "Parent Task ID must be greater than zero."));
        throw std::out_of_range("Parent Task ID must be greater than zero.");
    }

    if (m_primaryKey > 0 && parentTaskID == m_primaryKey)
    {
        appendErrorMessage(std::format("In TaskModel.{} : {}", __func__, "Parent Task ID can't be same as current Task id"));
        throw std::out_of_range("Parent Task ID can't be same as current Task id");
    }

    m_modified = true;
    m_parentTaskID = parentTaskID;
}

void TaskModel::setDueDate(std::chrono::year_month_day dueDate)
{
    m_modified = true;
    m_dueDate = dueDate;
}

void TaskModel::setScheduledStart(std::chrono::year_month_day startDate)
{
    m_modified = true;
    m_planedStart = startDate;
}

void TaskModel::setactualStartDate(std::chrono::year_month_day startDate)
{
    m_modified = true;
    m_actualStart = startDate;
}

void TaskModel::setEstimatedCompletion(std::chrono::year_month_day completionDate)
{
    m_modified = true;
    m_estimatedCompletion = completionDate;
}

void TaskModel::setCompletionDate(std::chrono::year_month_day completionDate)
{
    m_modified = true;
    m_completed = completionDate;
}

void TaskModel::setEstimatedEffort(double estimatedHours)
{
    m_modified = true;
    m_estimatedEffort = estimatedHours;
}

void TaskModel::setActualEffortToDate(double effortHoursYTD)
{
    m_modified = true;
    m_actualEffort = effortHoursYTD;
}

void TaskModel::setPriorityGroup(unsigned int category)
{
    m_modified = true;
    m_priorityCategory = category;
}

void TaskModel::setPriorityGroupC(const char category)
{
    unsigned int group = category - 'A' + 1;
    setPriorityGroup(group);
}

void TaskModel::setPriority(unsigned int priority)
{
    m_modified = true;
    m_priority = priority;
}

void TaskModel::setPersonal(bool personal)
{
    m_modified = true;
    m_personal = personal;
}

void TaskModel::setTaskID(std::size_t taskID)
{
    m_modified = true;
    m_primaryKey = taskID;
}

std::string TaskModel::taskStatusString(TaskModel::TaskStatus inVal) const
{
    auto statusName = taskStatusConversionTable.lookupName(inVal);
    return statusName.has_value()? *statusName : "Unknown TaskStatus Value";
}

TaskModel::TaskStatus TaskModel::stringToStatus(std::string statusName) const
{
    auto status = taskStatusConversionTable.lookupID(statusName);
    return status.has_value()? *status : UnknowStatus;
}

bool TaskModel::diffTask(TaskModel& other)
{
    // Ignoring optional fields
    return (m_primaryKey == other.m_primaryKey &&
        m_description == other.m_description &&
        other.m_creatorID == m_creatorID &&
        m_assignToID == other.m_assignToID &&
        m_dueDate == other.m_dueDate &&
        m_planedStart == other.m_planedStart &&
        m_planedStart == other.m_planedStart &&
        m_actualEffort == other.m_actualEffort &&
        m_priorityCategory == other.m_priorityCategory &&
        m_priority == other.m_priority &&
        m_personal == other.m_personal &&
        m_deleted == other.m_deleted
    );
}

/*
 * To make the code more maintainable each field / column in a table will have
 * its own line in insert and update statements. 
 */
std::string TaskModel::formatInsertStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());

    boost::mysql::format_sql_to(fctx, "INSERT INTO tasks (");
    boost::mysql::format_sql_to(fctx, "created_by, ");
    boost::mysql::format_sql_to(fctx, "assigned_to, ");
    boost::mysql::format_sql_to(fctx, "description, ");
    boost::mysql::format_sql_to(fctx, "task_status, ");
    boost::mysql::format_sql_to(fctx, "due_date, ");
    boost::mysql::format_sql_to(fctx, "planned_start, ");
    boost::mysql::format_sql_to(fctx, "actual_start, ");
    boost::mysql::format_sql_to(fctx, "estimated_delivery, ");
    boost::mysql::format_sql_to(fctx, "delivered, ");
    boost::mysql::format_sql_to(fctx, "est_hours_effort, ");
    boost::mysql::format_sql_to(fctx, "hours_effort, ");
    boost::mysql::format_sql_to(fctx, "priority_category, ");
    boost::mysql::format_sql_to(fctx, "priority, ");
    boost::mysql::format_sql_to(fctx, "personal, ");
    boost::mysql::format_sql_to(fctx, "last_modified_by_user ");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_creatorID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_assignToID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_description);
    boost::mysql::format_sql_to(fctx, "{}, ", getStatusIntVal());
    boost::mysql::format_sql_to(fctx, "{}, ", common::toBoostMySQLDate(m_dueDate.value()));
    boost::mysql::format_sql_to(fctx, "{}, ", common::toBoostMySQLDate(m_planedStart.value()));
    boost::mysql::format_sql_to(fctx, "{}, ", m_actualStart.transform(common::toBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "{}, ", m_estimatedCompletion.transform(common::toBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "{}, ", m_completed.transform(common::toBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "{}, ", m_estimatedEffort);
    boost::mysql::format_sql_to(fctx, "{}, ", m_actualEffort);
    boost::mysql::format_sql_to(fctx, "{}, ", m_priorityCategory);
    boost::mysql::format_sql_to(fctx, "{}, ", m_priority);
    boost::mysql::format_sql_to(fctx, "{}, ", m_personal);
    boost::mysql::format_sql_to(fctx, "{} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_creatorID);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string TaskModel::formatUpdateStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE tasks SET ");
    boost::mysql::format_sql_to(fctx, "tasks.created_by = {}, ", m_creatorID);
    boost::mysql::format_sql_to(fctx, "tasks.assigned_to = {}, ", m_assignToID);
    boost::mysql::format_sql_to(fctx, "tasks.description = {}, ", m_description);
    boost::mysql::format_sql_to(fctx, "tasks.task_status = {}, ", getStatusIntVal());
    boost::mysql::format_sql_to(fctx, "tasks.due_date = {}, ", common::toBoostMySQLDate(m_dueDate.value()));
    boost::mysql::format_sql_to(fctx, "tasks.planned_start = {}, ", common::toBoostMySQLDate(m_planedStart.value()));
    boost::mysql::format_sql_to(fctx, "tasks.actual_start = {}, ", m_actualStart.transform(common::toBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "tasks.estimated_delivery = {}, ", m_estimatedCompletion.transform(common::toBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "tasks.delivered = {}, ", m_completed.transform(common::toBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "tasks.est_hours_effort = {}, ", m_estimatedEffort);
    boost::mysql::format_sql_to(fctx, "tasks.hours_effort = {}, ", m_actualEffort);
    boost::mysql::format_sql_to(fctx, "tasks.priority_category = {}, ", m_priorityCategory);
    boost::mysql::format_sql_to(fctx, "tasks.priority = {}, ", m_priority);
    boost::mysql::format_sql_to(fctx, "tasks.personal = {}, ", m_personal);
    boost::mysql::format_sql_to(fctx, "tasks.deleted = {}, ", m_deleted);
    boost::mysql::format_sql_to(fctx, "tasks.last_modified_by_user = {} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_creatorID);
    boost::mysql::format_sql_to(fctx, "WHERE tasks.task_id = {} ", m_primaryKey);

    return (std::move(fctx).get().value());
}

std::string TaskModel::formatDeleteStatement()
{
    return boost::mysql::format_sql(getFormatOptions(),
        "UPDATE tasks SET tasks.deleted = 1, tasks.last_modified_by_user = {}  WHERE tasks.created_by = {} AND tasks.task_id = {}",
        m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_creatorID, m_creatorID, m_primaryKey);
}

void TaskModel::initRequiredFields()
{
    m_missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingDescription, this), "description"});
    m_missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingCreatorID, this), "user ID for creator"});
    m_missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingAssignedID, this), "user ID for assigned user"});
    m_missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingEffortEstimate, this), "estimated effort in hours"});
    m_missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingPriorityGroup, this), "priority"});
    m_missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingScheduledStart, this), "scheduled start date"});
    m_missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingDueDate, this), "due date (deadline)"});
}

void TaskModel::insertDependency()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "INSERT INTO task_dependencies (dependency, dependent_task) VALUES ({}, {})",
        m_primaryKey, m_parentTaskID.value());

    runQueryAsync(std::move(fctx).get().value());
}

void TaskModel::updateDependency()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE task_dependencies SET task_dependencies.dependent_task = {} ",
        m_parentTaskID.value());
    boost::mysql::format_sql_to(fctx, "WHERE task_dependencies.dependency = {}", m_primaryKey);

    runQueryAsync(std::move(fctx).get().value());
}

void TaskModel::selfTestResetAllValues() noexcept
{
    ModelDBInterface::selfTestResetAllValues();

    m_creatorID = 0;
    m_assignToID = 0;
    m_description.clear();
    m_status.reset();
    m_parentTaskID.reset();
    m_createdTimeStamp.reset();
    m_dueDate.reset();
    m_planedStart.reset();
    m_actualStart.reset();
    m_estimatedCompletion.reset();
    m_completed.reset();
    m_estimatedEffort = 0;
    m_actualEffort = 0.0;
    m_priorityCategory = 0;
    m_priority = 0;
    m_personal = false;
    m_lastUpdateTimeStamp.reset();
}

TestStatus TaskModel::testEqualityOperator() noexcept
{
    TaskModel other;

    other.setTaskID(m_primaryKey);

    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;

    return (*this == other)? TESTPASSED : TESTFAILED;
}

TestStatus TaskModel::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "description", "user ID for creator", "user ID for assigned user", "estimated effort in hours",
            "priority", "scheduled start date", "due date (deadline)", " missing required values"
    };

    setTaskID(0);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setDescription("Test missing required fields: Set Description");

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setCreatorID(1);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setAssignToID(1);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setEstimatedEffort(5);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setPriorityGroup(1);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setScheduledStart(common::TestDateRangeStartValue);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setDueDate(common::TestDateRangeEndValue);

    expectedErrors.clear();
    clearErrorMessages();

    setCreatedTimeStamp(common::TestTimeStampValue);
    
    if (m_verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", getModelName(), __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cerr << std::format("{}::testAllInsertFailures() Expected successful insert failed\n{}\n", getModelName(), m_errorMessages);
        return TESTFAILED;
    }

    return TESTPASSED;
}

void TaskModel::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

std::vector<ExceptionTestElement> TaskModel::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&TaskModel::testExceptionInsert, this), "testExceeptionInsert"});
    exceptionTests.push_back({std::bind(&TaskModel::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&TaskModel::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}

TestStatus TaskModel::testExceptionInsert() noexcept
{
    selfTestResetAllValues();
    m_forceException = true;

    setCreatorID(1);
    setAssignToID(2);
    setDescription("Testing Exception handling");
    setEstimatedEffort(6);
    setScheduledStart(common::TestDateValue);
    setDueDate(common::TestDateRangeEndValue);
    setPriorityGroup('A');
    setPriority(1);
    setParentTaskID(1);
    setCreatedTimeStamp(common::TestTimeStampValue);

    if (testFormatExceptionCatchSuccessNArgs(
        "TaskModel::formatInsertStatement", std::bind(&TaskModel::formatInsertStatement, this)) == TESTFAILED)
    {
        std::cerr << "TaskModel::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("TaskModel::insert", std::bind(&TaskModel::insert, this));
}

TestStatus TaskModel::testExceptionUpdate() noexcept
{
    selfTestResetAllValues();

    m_forceException = true;

    setTaskID(1);
    setDescription("Testing Exception handling");
    setCreatorID(1);
    setAssignToID(2);
    setEstimatedEffort(6);
    setScheduledStart(common::TestDateValue);
    setDueDate(common::TestDateRangeEndValue);
    setPriorityGroup('A');
    setPriority(1);
    setCreatedTimeStamp(common::TestTimeStampValue);
    setLastModifiedTimeStamp(common::TestTimeStampValue);
    
    if (testFormatExceptionCatchSuccessNArgs(
        "TaskModel::formatUpdateStatement", std::bind(&TaskModel::formatUpdateStatement, this)) == TESTFAILED)
    {
        std::cerr << "TaskModel::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("TaskModel::update", std::bind(&TaskModel::update, this));
}

TestStatus TaskModel::testExceptionHide() noexcept
{
    selfTestResetAllValues();
    m_forceException = true;

    std::size_t creatorIDTestValue = 1;

    setTaskID(53);
    setCreatorID(creatorIDTestValue);
    setAssignToID(2);
    setDescription("Testing Exception handling");
    setEstimatedEffort(6);
    setScheduledStart(common::TestDateValue);
    setDueDate(common::TestDateRangeEndValue);
    setPriorityGroup('A');
    setPriority(1);
    setParentTaskID(1);
    setCreatedTimeStamp(common::TestTimeStampValue);

    return testExceptionAndSuccessNArgs("TaskModel::hide", std::bind(&TaskModel::hide, this, std::placeholders::_1), creatorIDTestValue);
}

std::vector<AttributeTestFunction> TaskModel::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<AttributeTestFunction> attributeAccessTests; 
    attributeAccessTests.push_back({std::bind(&TaskModel::testTaskIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testCreatorIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testAssignToIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testDescriptionAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testStatusAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testParentTaskIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testCreationDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testLastUpdateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testDueDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testScheduledStartAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testActualStartDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testEstimatedCompletionAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testCompletionDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testEstimatedEffortAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testActualEffortToDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testPriorityGroupAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testPriorityGroupCAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testPriorityAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testPersonalAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testMarkComplete, this)});
    attributeAccessTests.push_back({std::bind(&TaskModel::testLastModifiedByUserAccess, this)});

    return attributeAccessTests;
}

TestStatus TaskModel::testTaskIdAccesss()
{
    std::size_t testPrimaryKey = 33;

    return testPrimaryKeyAccessFunctions(testPrimaryKey, 
        std::bind(&TaskModel::setTaskID, this, std::placeholders::_1),
        std::bind(&TaskModel::getTaskID, this));
}

TestStatus TaskModel::testCreatorIDAccess()
{
    std::size_t testValue = 1;

    return testForeignKeyFields(testValue, &m_creatorID, "Creator User ID",
        std::bind(&TaskModel::setCreatorID, this, std::placeholders::_1),
        std::bind(&TaskModel::getCreatorID, this));
}

TestStatus TaskModel::testAssignToIDAccess()
{
    std::size_t testValue = 2;

    return testForeignKeyFields(testValue, &m_assignToID, "Assigned User ID",
        std::bind(&TaskModel::setAssignToID, this, std::placeholders::_1),
        std::bind(&TaskModel::getAssignToID, this));
}

TestStatus TaskModel::testDescriptionAccess()
{
    std::string testValue("Test task description access");
    return testAccessorFunctions<std::string>(testValue, &m_description, "Description",
        std::bind(&TaskModel::setDescription, this, std::placeholders::_1),
        std::bind(&TaskModel::getDescription, this));
}

TestStatus TaskModel::testStatusAccess()
{
    TestStatus testStatus = TESTPASSED;
    TaskStatus testValue = TaskModel::TaskStatus::On_Hold;

    std::cout << "Running self test on set and get functions for TaskModel::status\n";

    m_modified = false;
    setStatus(testValue);
    if (testStatusVerifyValueAndGetStatus(testValue) == TESTFAILED)
    {
        testStatus = TESTFAILED;
    }

    m_modified = false;
    testValue = TaskModel::TaskStatus::Complete;
    std::string statusName = taskStatusString(testValue);
    setStatus(statusName);
    if (testStatusVerifyValueAndGetStatus(testValue) == TESTFAILED)
    {
        testStatus = TESTFAILED;
    }

    std::cout << "Self test on set and get functions for TaskModel::status PASSED\n";

    return testStatus;
}

TestStatus TaskModel::testStatusVerifyValueAndGetStatus(TaskStatus testValue)
{
    if (!isModified())
    {
        std::cerr << "In self test for: TaskModel::status set function for status FAILED to set modified\n";
        return TESTFAILED;
    }

    if (!m_status.has_value() || m_status.value() != testValue)
    {
        if (!m_status.has_value())
        {
            std::cerr  << "In self test for: TaskModel::status Set function for status FAILED to set member value\n";
        }
        if (m_status.value() != testValue)
        {
            std::cerr  << "In self test for: TaskModel::status expected value: " << static_cast<unsigned int>(testValue)
                        << "actual value: " << static_cast<unsigned int>(m_status.value()) << " FAILED to set member value\n";
        }
        return TESTFAILED;
    }

    std::optional<TaskModel::TaskStatus> returnValue = getStatus();
    if (!returnValue.has_value() || returnValue.value() != testValue)
    {
        std::cerr  << "In self test for: TaskModel::status Get function for status FAILED\n";
        return TESTFAILED;
    }

    if (getStatusIntVal() != static_cast<unsigned int>(returnValue.value()))
    {
        std::cerr  << "In self test for: TaskModel::getStatusIntVal() FAILED\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}


TestStatus TaskModel::testParentTaskIDAccess()
{
    TestStatus testStatus = TESTPASSED;
    std::size_t testValue = 37;

    std::cout << "Running self test on set and get functions for TaskModel::parentTaskID\n";

    m_modified = false;
    setParentTaskID(testValue);
    if (testParentTaskIDVerifyValueAndGetParentTaskID(testValue) == TESTFAILED)
    {
        testStatus = TESTFAILED;
    }

    testValue = 42;
    TaskModel_shp parent = std::make_shared<TaskModel>(TaskModel());
    parent->setTaskID(testValue);
    m_modified = false;
    setParentTaskID(parent);
    if (testParentTaskIDVerifyValueAndGetParentTaskID(testValue) == TESTFAILED)
    {
        testStatus = TESTFAILED;
    }

    if (testStatus == TESTPASSED)
    {
        std::cout << "Self test on set and get functions for TaskModel::parentTaskID PASSED\n";
    }
    else
    {
        std::cout << "Self test on set and get functions for TaskModel::parentTaskID FAILED\n";
    }

    return testStatus;
}

TestStatus TaskModel::testParentTaskIDVerifyValueAndGetParentTaskID(std::size_t testValue)
{
    if (!isModified())
    {
        std::cerr << "In self test for: TaskModel::parentID set function for parentID FAILED to set modified\n";
        return TESTFAILED;
    }

    if (!m_parentTaskID.has_value() || m_parentTaskID.value() != testValue)
    {
        if (!m_parentTaskID.has_value())
        {
            std::cerr  << "In self test for: TaskModel::parentTaskID Set function for parentTaskID FAILED to set member value\n";
        }
        if (m_parentTaskID.value() != testValue)
        {
            std::cerr  << "In self test for: TaskModel::parentTaskID expected value: " << testValue <<
                "actual value: " << m_parentTaskID.value() << " FAILED to set member value\n";
        }
        return TESTFAILED;
    }

    std::optional<std::size_t> returnValue = getParentTaskID();
    if (!returnValue.has_value() || returnValue.value() != testValue)
    {
        std::cerr  << "In self test for: TaskModel::parentTaskID Get function for parentTaskID FAILED\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus TaskModel::testCreationDateAccess()
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_createdTimeStamp, "Creation TimeStamp",
        std::bind(&TaskModel::setCreatedTimeStamp, this, std::placeholders::_1),
        std::bind(&TaskModel::getCreatedTSValue, this));
}


TestStatus TaskModel::testLastUpdateAccess()
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_lastUpdateTimeStamp, "Last Update TimeStamp",
        std::bind(&TaskModel::setLastModifiedTimeStamp, this, std::placeholders::_1),
        std::bind(&TaskModel::getLastModifiedValue, this));
}

TestStatus TaskModel::testDueDateAccess()
{
    std::chrono::year_month_day testValue = common::TestDateValue;
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &m_dueDate, "Due Date",
        std::bind(&TaskModel::setDueDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getDueDate, this));
}

TestStatus TaskModel::testScheduledStartAccess()
{
    std::chrono::year_month_day testValue = common::TestDateRangeStartValue;
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &m_planedStart, "Scheduled Start Date",
        std::bind(&TaskModel::setScheduledStart, this, std::placeholders::_1),
        std::bind(&TaskModel::getScheduledStart, this));
}

TestStatus TaskModel::testActualStartDateAccess()
{
    std::chrono::year_month_day testValue = common::TestDateValue;
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &m_actualStart, "Actual Start Date",
        std::bind(&TaskModel::setactualStartDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getactualStartDate, this));
}

TestStatus TaskModel::testEstimatedEffortAccess()
{
    double testValue = 10.24;

    return testAccessorFunctions<double>(testValue, &m_estimatedEffort, "Estimated Effort",
        std::bind(&TaskModel::setEstimatedEffort, this, std::placeholders::_1),
        std::bind(&TaskModel::getEstimatedEffort, this));
}

TestStatus TaskModel::testCompletionDateAccess()
{
    std::chrono::year_month_day testValue = common::TestDateValue;
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &m_completed, "Completion Date",
        std::bind(&TaskModel::setCompletionDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getCompletionDate, this));
}

TestStatus TaskModel::testEstimatedCompletionAccess()
{
    std::chrono::year_month_day testValue = common::TestDateValue;
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &m_estimatedCompletion, "Completion Date",
        std::bind(&TaskModel::setEstimatedCompletion, this, std::placeholders::_1),
        std::bind(&TaskModel::getEstimatedCompletion, this));
}

TestStatus TaskModel::testActualEffortToDateAccess()
{
    double testValue = 752.75;
    if (testAccessorFunctions<double>(testValue, &m_actualEffort, "Actual Effort to date",
        std::bind(&TaskModel::setActualEffortToDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getactualEffortToDate, this)) == TESTFAILED)
    {
        return TESTFAILED;
    }

    return testAddEffort();
}

TestStatus TaskModel::testPriorityGroupAccess()
{
    unsigned int testValue = 3;

    return testAccessorFunctions<unsigned int>(testValue, &m_priorityCategory, "Priority Group Int Value",
        std::bind(&TaskModel::setPriorityGroup, this, std::placeholders::_1),
        std::bind(&TaskModel::getPriorityGroup, this));
}

TestStatus TaskModel::testPriorityGroupCAccess()
{
    const char testValue = 'B';
    unsigned int expectedInternalValue = testValue - 'A' + 1;
    std::string_view memberName("Priority Group Character");

    std::cout << "Running self test on set and get functions for " << getModelName() << "::" << memberName << "\n";

    m_modified = false;

    setPriorityGroupC(testValue);
    if (!isModified())
    {
        std::cerr << "In self test for: " << getModelName() << " set function for " << memberName << " FAILED to set modified\n";
        return TESTFAILED;
    }

    if (m_priorityCategory != expectedInternalValue)
    {
        std::cerr  << "In self test for: " << getModelName() << "Set function for " << memberName << " FAILED to set member value\n";
        std::cerr << "\tExpected Value: " << expectedInternalValue << "Actual Value: " << m_priorityCategory << "\n";
        return TESTFAILED;
    }

    if (getPriorityGroup() != expectedInternalValue)
    {
        std::cerr  << "In self test for: " << getModelName() << " Get function for " << memberName << " FAILED\n";
        std::cerr << "\tExpected Value: " << expectedInternalValue << "Actual Value: " << getPriorityGroup() << "\n";
        return TESTFAILED;
    }

    std::cout << "Self test on set and get functions for " << getModelName() << "::" << memberName << " PASSED\n";

    return TESTPASSED;
}

TestStatus TaskModel::testPriorityAccess()
{
    unsigned int testValue = 5;

    return testAccessorFunctions<unsigned int>(testValue, &m_priority, "Priority",
        std::bind(&TaskModel::setPriority, this, std::placeholders::_1),
        std::bind(&TaskModel::getPriority, this));
}

TestStatus TaskModel::testPersonalAccess()
{
    bool testValue = true;

    return testAccessorFunctions<bool>(testValue, &m_personal, "Personal",
        std::bind(&TaskModel::setPersonal, this, std::placeholders::_1),
        std::bind(&TaskModel::isPersonal, this));
}

TestStatus TaskModel::testLastModifiedByUserAccess() noexcept
{
    std::size_t testUserId = 1;

    return testLastModifiedByAccess(testUserId);
}

/*
 * This test assumes that getStatus() and getCompletionDate() are tested elsewhere.
 */
TestStatus TaskModel::testMarkComplete()
{
    std::cout << "Running self test for TaskModel::markComplete\n";

    m_modified = false;
    m_status.reset();
    m_completed.reset();

    markComplete();

    if (!m_status.has_value() || m_status.value() != TaskStatus::Complete)
    {
        if (!m_status.has_value())
        {
            std::cerr  << "In self test for: TaskModel::markComplete FAILED to set status member value\n";
        }
        if (m_status.value() != TaskStatus::Complete)
        {
            std::cerr  << "In self test for: TaskModel::markComplete expected value: " << static_cast<unsigned int>(TaskStatus::Complete)
                << "actual value: " << static_cast<unsigned int>(m_status.value()) << " FAILED to set status member value\n";
        }
        return TESTFAILED;
    }

    if (!m_completed.has_value())
    {
        std::cerr  << "In self test for: TaskModel::markComplete FAILED to set completionDate member value\n";
        return TESTFAILED;
    }

    std::cout << "Self test for TaskModel::markComplete PASSED\n";

    return TESTPASSED;
}

TestStatus TaskModel::testAddEffort()
{
    double addedHours = 7.25;
    double expectedHours = m_actualEffort + addedHours;

    m_modified = false;
    addEffortHours(addedHours);

    if (!isModified())
    {
        std::cerr << "TaskModel::addEffortHours() failed to set modified! test FAILED;\n";
        return TESTFAILED;
    }

    if (m_actualEffort != expectedHours)
    {
        std::cerr << "TaskModel::actualEffortToDate does not show additional hours! test FAILED;\n";
        return TESTFAILED;

    }

    if (getactualEffortToDate() != expectedHours)
    {
        std::cerr << "TaskModel::getactualEffortToDate() does not show additional hours! test FAILED;\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

