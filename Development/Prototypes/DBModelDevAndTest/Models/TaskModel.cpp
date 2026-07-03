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
  m_CreatorID = 0;
  m_AssignToID = 0;
  m_Description = "";
  m_EstimatedEffort = 0;
  m_ActualEffort = 0.0;
  m_PriorityCategory = 0;
  m_Priority = 0;
  m_Personal = false;
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
    m_PrimaryKey = taskId;
    m_CreatorID = creator;
    m_AssignToID = assignTo;
    m_Description = description;
    m_Status = status;
    m_ParentTaskID = parentTask;
    m_DueDate = dueDate;
    m_PlanedStart = scheduledStart;
    m_ActualStart = actualStartDate;
    m_EstimatedCompletion = estimatedCompletion;
    m_Completed = completionDate;
    m_EstimatedEffort = estimatedEffort;
    m_ActualEffort = actualEffort;
    m_PriorityCategory = priorityCategory;
    m_Priority = priority;
    m_Personal = personal;
    m_Created = creationed;
    m_LastUpdate = lastModified;
    m_Deleted = hidden;

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
        appendErrorMessage(std::format("{} not in Database, nothing to delete!", m_ModelName));

        return false;
    }

    if (userRequestingDelete != m_CreatorID)
    {
        appendErrorMessage(std::format("Permission denied, deleting user ({}) is not the creator ({}) of the task", userRequestingDelete, m_CreatorID));

        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatDeleteStatement());

        m_Deleted = true;
        
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.hide() : {}", m_ModelName, e.what()));
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
    return m_ActualStart.value_or(std::chrono::year_month_day());
}

std::chrono::year_month_day TaskModel::getEstimatedCompletion() const
{
    return m_EstimatedCompletion.value_or(std::chrono::year_month_day());
}

std::chrono::year_month_day TaskModel::getCompletionDate() const
{
    return m_Completed.value_or(std::chrono::year_month_day());
}

void TaskModel::setCreatorID(std::size_t creatorID)
{
    m_Modified = true;
    m_CreatorID = creatorID;
}

void TaskModel::setAssignToID(std::size_t assignedID)
{
    m_Modified = true;
    m_AssignToID = assignedID;
}

void TaskModel::setDescription(std::string description)
{
    m_Modified = true;
    m_Description = description;
}

void TaskModel::setStatus(TaskModel::TaskStatus status)
{
    m_Modified = true;
    m_Status = status;

    if (m_Status == TaskModel::TaskStatus::Work_in_Progress)
    {
        if (!m_ActualStart.has_value())
        {
            setactualStartDate(getTodaysDate());
        }
    }

    if (m_Status == TaskModel::TaskStatus::Complete)
    {
        setCompletionDate(getTodaysDate());
    }
}

void TaskModel::setParentTaskID(std::size_t parentTaskID)
{
    m_Modified = true;
    m_ParentTaskID = parentTaskID;
}

void TaskModel::setCreationDate(std::chrono::system_clock::time_point created)
{
    m_Modified = true;
    m_Created = created;
}

void TaskModel::setDueDate(std::chrono::year_month_day dueDate)
{
    m_Modified = true;
    m_DueDate = dueDate;
}

void TaskModel::setScheduledStart(std::chrono::year_month_day startDate)
{
    m_Modified = true;
    m_PlanedStart = startDate;
}

void TaskModel::setactualStartDate(std::chrono::year_month_day startDate)
{
    m_Modified = true;
    m_ActualStart = startDate;
}

void TaskModel::setEstimatedCompletion(std::chrono::year_month_day completionDate)
{
    m_Modified = true;
    m_EstimatedCompletion = completionDate;
}

void TaskModel::setCompletionDate(std::chrono::year_month_day completionDate)
{
    m_Modified = true;
    m_Completed = completionDate;
}

void TaskModel::setEstimatedEffort(unsigned int estimatedHours)
{
    m_Modified = true;
    m_EstimatedEffort = estimatedHours;
}

void TaskModel::setActualEffortToDate(double effortHoursYTD)
{
    m_Modified = true;
    m_ActualEffort = effortHoursYTD;
}

void TaskModel::setPriorityGroup(unsigned int category)
{
    m_Modified = true;
    m_PriorityCategory = category;
}

void TaskModel::setPriorityGroupC(const char category)
{
    unsigned int group = category - 'A' + 1;
    setPriorityGroup(group);
}

void TaskModel::setPriority(unsigned int priority)
{
    m_Modified = true;
    m_Priority = priority;
}

void TaskModel::setPersonal(bool personal)
{
    m_Modified = true;
    m_Personal = personal;
}

void TaskModel::addDependency(std::size_t taskId)
{
    m_Modified = true;
    m_Dependencies.push_back(taskId);
}

void TaskModel::setLastUpdate(std::chrono::system_clock::time_point lastModified)
{
    m_Modified = true;
    m_LastUpdate = lastModified;
}

void TaskModel::setTaskID(std::size_t taskID)
{
    m_Modified = true;
    m_PrimaryKey = taskID;
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
    return (m_PrimaryKey == other.m_PrimaryKey &&
        m_Description == other.m_Description &&
        other.m_CreatorID == m_CreatorID &&
        m_AssignToID == other.m_AssignToID &&
        m_DueDate == other.m_DueDate &&
        m_PlanedStart == other.m_PlanedStart &&
        m_PlanedStart == other.m_PlanedStart &&
        m_ActualEffort == other.m_ActualEffort &&
        m_PriorityCategory == other.m_PriorityCategory &&
        m_Priority == other.m_Priority &&
        m_Personal == other.m_Personal &&
        m_Deleted == other.m_Deleted &&
        m_Dependencies.size() == other.m_Dependencies.size()
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

    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),
        "CALL AddTask({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16})",
            m_CreatorID,
            m_AssignToID,
            m_Description,
            m_ParentTaskID,
            getStatusIntVal(),
            stdchronoDateToBoostMySQLDate(m_DueDate.value()),
            stdchronoDateToBoostMySQLDate(m_PlanedStart.value()),
            optionalDateConversion(m_ActualStart),
            optionalDateConversion(m_EstimatedCompletion),
            optionalDateConversion(m_Completed),
            m_EstimatedEffort,
            m_ActualEffort,
            m_PriorityCategory,
            m_Priority,
            m_Personal,
            dependencyCount,
            depenenciesText
    );
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

    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),
        "CALL UpdateTaskAllFields({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17}, {18})",
        m_PrimaryKey, m_CreatorID, m_AssignToID, m_Description, m_ParentTaskID, getStatusIntVal(),
            stdchronoDateToBoostMySQLDate(m_DueDate.value()),
            stdchronoDateToBoostMySQLDate(m_PlanedStart.value()),
            optionalDateConversion(m_ActualStart),
            optionalDateConversion(m_EstimatedCompletion),
            optionalDateConversion(m_Completed),
            m_EstimatedEffort, m_ActualEffort, m_PriorityCategory,m_Priority, m_Personal,
            dependencyCount, depenenciesText, m_Deleted
    );
}

std::string TaskModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(), "CALL HideTask({}, {})", m_CreatorID, m_PrimaryKey);
}

void TaskModel::initRequiredFields()
{
    m_MissingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingDescription, this), "description"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingCreatorID, this), "user ID for creator"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingAssignedID, this), "user ID for assigned user"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingEffortEstimate, this), "estimated effort in hours"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingPriorityGroup, this), "priority"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingScheduledStart, this), "scheduled start date"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingDueDate, this), "due date (deadline)"});
}

void TaskModel::addDependencies(const std::string & dependenciesText)
{
    std::vector<std::string> dependencyStrings = explodeTextField(dependenciesText);

    if (!dependencyStrings.empty())
    {
        for (auto dependencyStr: dependencyStrings)
        {
            m_Dependencies.push_back(static_cast<std::size_t>(std::stol(dependencyStr)));
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


