// Project Header Files
#include "commonUtilities.h"
#include "GenericDictionary.h"
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
  m_estimatedEffort = 0;
  m_actualEffort = 0.0;
  m_priorityCategory = 0;
  m_priority = 0;
  m_personal = false;
}

TaskModel::TaskModel(
    std::size_t taskId,
    std::size_t creator,
    std::size_t assignTo,
    std::string description,
    std::optional<TaskStatus> status,
    std::optional<std::size_t> parentTask,
    std::optional<std::chrono::year_month_day> dueDate,
    std::optional<std::chrono::year_month_day> scheduledStart,
    std::optional<std::chrono::year_month_day> actualStartDate,
    std::optional<std::chrono::year_month_day> estimatedCompletion,
    std::optional<std::chrono::year_month_day> completionDate,
    unsigned int estimatedEffort,
    double actualEffort,
    unsigned int priorityCategory,
    unsigned int priority,
    bool personal,
    std::size_t dependencyCount,
    std::string dependencies,
    std::optional<std::chrono::system_clock::time_point> creationed,
    std::optional<std::chrono::system_clock::time_point> lastModified,
    bool hidden
)
: TaskModel()
{
    m_primaryKey = taskId;
    m_creatorID = creator;
    m_assignToID = assignTo;
    m_description = description;
    m_status = status;
    m_parentTaskID = parentTask;
    m_dueDate = dueDate;
    m_planedStart = scheduledStart;
    m_actualStart = actualStartDate;
    m_estimatedCompletion = estimatedCompletion;
    m_completed = completionDate;
    m_estimatedEffort = estimatedEffort;
    m_actualEffort = actualEffort;
    m_priorityCategory = priorityCategory;
    m_priority = priority;
    m_personal = personal;
    m_created = creationed;
    m_lastUpdate = lastModified;
    m_deleted = hidden;

    if (dependencyCount)
    {
        addDependencies(dependencies);
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

void TaskModel::setEstimatedEffort(unsigned int estimatedHours)
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

    boost::mysql::format_sql_to(fctx, "INSERT INTO tasks ("
        "created_by, "
        "assigned_to, "
        "description, "
        "parent_task, "
        "task_status, "
        "due_date, "
        "planned_start, "
        "actual_start, "
        "estimated_delivery, "
        "delivered, "
        "est_hours_effort, "
        "hours_effort, "
        "priority_category, "
        "priority, "
        "personal, "
        "dependency_count, "
        "dependencies"
    ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_creatorID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_assignToID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_description);
    boost::mysql::format_sql_to(fctx, "{}, ", m_parentTaskID);
    boost::mysql::format_sql_to(fctx, "{}, ", getStatusIntVal());
    boost::mysql::format_sql_to(fctx, "{}, ", stdchronoDateToBoostMySQLDate(m_dueDate.value()));
    boost::mysql::format_sql_to(fctx, "{}, ", stdchronoDateToBoostMySQLDate(m_planedStart.value()));
    boost::mysql::format_sql_to(fctx, "{}, ", optionalDateConversion(m_actualStart));
    boost::mysql::format_sql_to(fctx, "{}, ", optionalDateConversion(m_estimatedCompletion));
    boost::mysql::format_sql_to(fctx, "{}, ", optionalDateConversion(m_completed));
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
    boost::mysql::format_sql_to(fctx, "tasks.actual_start = {}, ", optionalDateConversion(m_actualStart));
    boost::mysql::format_sql_to(fctx, "tasks.estimated_delivery = {}, ", optionalDateConversion(m_estimatedCompletion));
    boost::mysql::format_sql_to(fctx, "tasks.delivered = {}, ", optionalDateConversion(m_completed));
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

void TaskModel::addDependencies(const std::string & dependenciesText)
{
    std::vector<std::string> dependencyStrings = explodeTextField(dependenciesText);

    if (!dependencyStrings.empty())
    {
        for (auto dependencyStr: dependencyStrings)
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

    return implodeTextField(dependencyStrings);
}


