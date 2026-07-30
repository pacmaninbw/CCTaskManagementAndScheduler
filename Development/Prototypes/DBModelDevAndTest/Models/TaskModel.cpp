// Project Header Files
#include "commonUtilities.h"
#include "GenericDictionary.h"
#include "stdChronoBoostConversions.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <algorithm>
#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <ranges>
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
    m_parentTaskID = dbTranslator.parent_task;
    m_dueDate = boostMysqlDateToChronoDate(dbTranslator.due_date);
    m_planedStart = boostMysqlDateToChronoDate(dbTranslator.planned_start);
    m_actualStart = dbTranslator.actual_start.transform(boostMysqlDateToChronoDate);
    m_estimatedCompletion = dbTranslator.estimated_delivery.transform(boostMysqlDateToChronoDate);
    m_completed = dbTranslator.delivered.transform(boostMysqlDateToChronoDate);
    m_estimatedEffort = dbTranslator.est_hours_effort;
    m_actualEffort = dbTranslator.hours_effort;
    m_priorityCategory = dbTranslator.priority_category;
    m_priority = dbTranslator.priority;
    m_personal = dbTranslator.personal;
    m_created = boostMysqlDateTimeToChronoTimePoint(dbTranslator.creation_timestamp);
    m_lastUpdate = boostMysqlDateTimeToChronoTimePoint(dbTranslator.last_modified_time_stamp);
    m_deleted = dbTranslator.deleted;

    if (dbTranslator.dependency_count)
    {
        const std::string temp = dbTranslator.dependencies.value();
        addDependencies(temp);
    }
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

bool TaskModel::hide(std::size_t userRequestingDelete) noexcept
{
    clearErrorMessages();

    if (!isInDataBase())
    {
        appendErrorMessage(std::format("{} not in Database, nothing to delete!", m_modelName));

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
        appendErrorMessage(std::format("In {}.hide() : {}", m_modelName, e.what()));
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
            setactualStartDate(getTodaysDate());
        }
    }

    if (m_status == TaskModel::TaskStatus::Complete)
    {
        setCompletionDate(getTodaysDate());
    }
}

void TaskModel::setParentTaskID(std::size_t parentTaskID)
{
    m_modified = true;
    m_parentTaskID = parentTaskID;
}

void TaskModel::setCreationDate(std::chrono::system_clock::time_point created)
{
    m_modified = true;
    m_created = created;
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

void TaskModel::addDependency(std::size_t taskId)
{
    m_modified = true;
    m_dependencies.push_back(taskId);
}

void TaskModel::setLastUpdate(std::chrono::system_clock::time_point lastModified)
{
    m_modified = true;
    m_lastUpdate = lastModified;
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
        m_deleted == other.m_deleted &&
        m_dependencies.size() == other.m_dependencies.size()
    );
}

/*
 * To make the code more maintainable each field / column in a table will have
 * its own line in insert and update statements. 
 */
std::string TaskModel::formatInsertStatement()
{
    std::size_t dependencyCount = getDependencies().size();
    std::optional<std::string> depenenciesText;
    if (dependencyCount)
    {
        std::vector<std::size_t> dependencyList = getDependencies();
        depenenciesText = buildDependenciesText(dependencyList);
    }

    boost::mysql::format_context fctx(getFormatOptions());

    boost::mysql::format_sql_to(fctx, "INSERT INTO tasks (");
    boost::mysql::format_sql_to(fctx, "created_by, ");
    boost::mysql::format_sql_to(fctx, "assigned_to, ");
    boost::mysql::format_sql_to(fctx, "description, ");
    boost::mysql::format_sql_to(fctx, "parent_task, ");
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
    boost::mysql::format_sql_to(fctx, "dependency_count, ");
    boost::mysql::format_sql_to(fctx, "dependencies");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_creatorID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_assignToID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_description);
    boost::mysql::format_sql_to(fctx, "{}, ", m_parentTaskID);
    boost::mysql::format_sql_to(fctx, "{}, ", getStatusIntVal());
    boost::mysql::format_sql_to(fctx, "{}, ", stdchronoDateToBoostMySQLDate(m_dueDate.value()));
    boost::mysql::format_sql_to(fctx, "{}, ", stdchronoDateToBoostMySQLDate(m_planedStart.value()));
    boost::mysql::format_sql_to(fctx, "{}, ", m_actualStart.transform(stdchronoDateToBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "{}, ", m_estimatedCompletion.transform(stdchronoDateToBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "{}, ", m_completed.transform(stdchronoDateToBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "{}, ", m_estimatedEffort);
    boost::mysql::format_sql_to(fctx, "{}, ", m_actualEffort);
    boost::mysql::format_sql_to(fctx, "{}, ", m_priorityCategory);
    boost::mysql::format_sql_to(fctx, "{}, ", m_priority);
    boost::mysql::format_sql_to(fctx, "{}, ", m_personal);
    boost::mysql::format_sql_to(fctx, "{}, ", dependencyCount);
    boost::mysql::format_sql_to(fctx, "{}", depenenciesText);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string TaskModel::formatUpdateStatement()
{
    std::size_t dependencyCount = getDependencies().size();
    std::optional<std::string> depenenciesText;
    if (dependencyCount)
    {
        std::vector<std::size_t> dependencyList = getDependencies();
        depenenciesText = buildDependenciesText(dependencyList);
    }

    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE tasks SET ");
    boost::mysql::format_sql_to(fctx, "tasks.created_by = {}, ", m_creatorID);
    boost::mysql::format_sql_to(fctx, "tasks.assigned_to = {}, ", m_assignToID);
    boost::mysql::format_sql_to(fctx, "tasks.description = {}, ", m_description);
    boost::mysql::format_sql_to(fctx, "tasks.parent_task = {}, ", m_parentTaskID);
    boost::mysql::format_sql_to(fctx, "tasks.task_status = {}, ", getStatusIntVal());
    boost::mysql::format_sql_to(fctx, "tasks.due_date = {}, ", stdchronoDateToBoostMySQLDate(m_dueDate.value()));
    boost::mysql::format_sql_to(fctx, "tasks.planned_start = {}, ", stdchronoDateToBoostMySQLDate(m_planedStart.value()));
    boost::mysql::format_sql_to(fctx, "tasks.actual_start = {}, ", m_actualStart.transform(stdchronoDateToBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "tasks.estimated_delivery = {}, ", m_estimatedCompletion.transform(stdchronoDateToBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "tasks.delivered = {}, ", m_completed.transform(stdchronoDateToBoostMySQLDate));
    boost::mysql::format_sql_to(fctx, "tasks.est_hours_effort = {}, ", m_estimatedEffort);
    boost::mysql::format_sql_to(fctx, "tasks.hours_effort = {}, ", m_actualEffort);
    boost::mysql::format_sql_to(fctx, "tasks.priority_category = {}, ", m_priorityCategory);
    boost::mysql::format_sql_to(fctx, "tasks.priority = {}, ", m_priority);
    boost::mysql::format_sql_to(fctx, "tasks.personal = {}, ", m_personal);
    boost::mysql::format_sql_to(fctx, "tasks.dependency_count = {}, ", dependencyCount);
    boost::mysql::format_sql_to(fctx, "tasks.dependencies = {}, ", depenenciesText);
    boost::mysql::format_sql_to(fctx, "tasks.deleted = {} ", m_deleted);
    boost::mysql::format_sql_to(fctx, "WHERE tasks.task_id = {} ", m_primaryKey);

    return (std::move(fctx).get().value());
}

std::string TaskModel::formatDeleteStatement()
{
    return boost::mysql::format_sql(getFormatOptions(),
        "UPDATE tasks SET tasks.deleted = 1 WHERE tasks.created_by = {} AND tasks.task_id = {}", m_creatorID, m_primaryKey);
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

void TaskModel::addDependencies(const std::string& dependenciesText)
{
    std::vector<std::string> dependencyStrings = dependenciesText
        | std::views::split(m_delimiter) 
        | std::ranges::to<std::vector<std::string>>();

    if (!dependencyStrings.empty())
    {
        for (auto& dependencyStr: dependencyStrings)
        {
            m_dependencies.push_back(static_cast<std::size_t>(std::stol(dependencyStr)));
        }
    }
    else
    {
        std::runtime_error NoExpectedDependencies("Dependencies expected but not found!");
        throw NoExpectedDependencies;
    }
}

std::string TaskModel::buildDependenciesText(std::vector<std::size_t>& dependencyList) noexcept
{
    if (dependencyList.size() > 1)
    {
        std::sort(dependencyList.begin(), dependencyList.end());

        auto last = std::unique(dependencyList.begin(), dependencyList.end());

        dependencyList.erase(last, dependencyList.end());
    }

    std::vector<std::string> dependencyStrings;
    for (auto dependency: dependencyList)
    {
        dependencyStrings.push_back(std::to_string(dependency));
    }
    auto joined_view = dependencyStrings | std::views::join_with(m_delimiter);
    return std::ranges::to<std::string>(joined_view);
}

