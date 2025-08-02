#include <chrono>
#include "commonUtilities.h"
#include "GenericDictionary.h"
#include <iostream>
#include <memory>
#include <string>
#include "TaskModel.h"
#include "UserModel.h"
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
: modified{false},
  taskID{0},
  creatorID{0},
  assignToID{0},
  description{""},
  percentageComplete{0.0},
  estimatedEffort{0},
  actualEffortToDate{0.0},
  priorityGroup{0},
  priority{0},
  personal{false}
{
    std::chrono::year_month_day today = getTodaysDate();
    setCreationDate(today);
}

TaskModel::TaskModel(UserModel_shp creator)
: TaskModel()
{
    setCreatorID(creator->getUserID());
    setAssignToID(creator->getUserID());
}

TaskModel::TaskModel(UserModel_shp creator, std::string description)
: TaskModel()
{
    setCreatorID(creator->getUserID());
    setAssignToID(creator->getUserID());
    setDescription(description);
}

bool TaskModel::hasRequiredValues() const
{
    if (description.empty() || description.length() < 10)
    {
        return false;
    }
    
    return true;
}

void TaskModel::addEffortHours(double hours)
{
    double actualEffortHours = getactualEffortToDate();
    actualEffortHours += hours;
    setActualEffortToDate(actualEffortHours);
}

std::chrono::year_month_day TaskModel::getactualStartDate() const
{
    return actualStartDate.value_or(std::chrono::year_month_day());
}

std::chrono::year_month_day TaskModel::getEstimatedCompletion() const
{
    return std::chrono::year_month_day();
}

std::chrono::year_month_day TaskModel::getCompletionDate() const
{
    return std::chrono::year_month_day();
}

void TaskModel::setCreatorID(std::size_t inCreatorID)
{
    modified = true;
    creatorID = inCreatorID;
}

void TaskModel::setAssignToID(std::size_t inAssignedID)
{
    modified = true;
    assignToID = inAssignedID;
}

void TaskModel::setDescription(std::string inDescription)
{
    modified = true;
    description = inDescription;
}

void TaskModel::setStatus(TaskModel::TaskStatus inStatus)
{
    modified = true;
    status = inStatus;
}

void TaskModel::setParentTaskID(std::size_t inParentTaskID)
{
    modified = true;
    parentTaskID = inParentTaskID;
}

void TaskModel::setPercentageComplete(double inPercentComplete)
{
    modified = true;
    percentageComplete = inPercentComplete;
}

void TaskModel::setCreationDate(std::chrono::year_month_day inCreationDate)
{
    modified = true;
    creationDate = inCreationDate;
}

void TaskModel::setDueDate(std::chrono::year_month_day inDueDate)
{
    modified = true;
    dueDate = inDueDate;
}

void TaskModel::setScheduledStart(std::chrono::year_month_day startDate)
{
    modified = true;
    scheduledStart = startDate;
}

void TaskModel::setactualStartDate(std::chrono::year_month_day startDate)
{
    modified = true;
    actualStartDate = startDate;
}

void TaskModel::setEstimatedCompletion(std::chrono::year_month_day completionDate)
{
    modified = true;
    estimatedCompletion = completionDate;
}

void TaskModel::setCompletionDate(std::chrono::year_month_day inCompletionDate)
{
    modified = true;
    completionDate = inCompletionDate;
}

void TaskModel::setEstimatedEffort(unsigned int inEstimatedHours)
{
    modified = true;
    estimatedEffort = inEstimatedHours;
}

void TaskModel::setActualEffortToDate(double effortHoursYTD)
{
    modified = true;
    actualEffortToDate = effortHoursYTD;
}

void TaskModel::setPriorityGroup(unsigned int inPriorityGroup)
{
    modified = true;
    priorityGroup = inPriorityGroup;
}

void TaskModel::setPriorityGroupC(const char priorityGroup)
{
    unsigned int group = priorityGroup - 'A' + 1;
    setPriorityGroup(group);
}

void TaskModel::setPriority(unsigned int inPriority)
{
    modified = true;
    priority = inPriority;
}

void TaskModel::setPersonal(bool personalIn)
{
    modified = true;
    personal = personalIn;
}

void TaskModel::addDependency(std::size_t taskId)
{
    modified = true;
    dependencies.push_back(taskId);
}

void TaskModel::setTaskID(std::size_t newID)
{
    modified = true;
    taskID = newID;
}

std::string TaskModel::taskStatusString() const
{
    TaskModel::TaskStatus status = getStatus();
    auto statusName = taskStatusConversionTable.lookupName(status);
    return statusName.has_value()? *statusName : "Unknown TaskStatus Value";
}

TaskModel::TaskStatus TaskModel::stringToStatus(std::string statusName) const
{
    auto status = taskStatusConversionTable.lookupID(statusName);
    return status.has_value()? *status : UnknowStatus;
}

bool TaskModel::diffTask(TaskModel& other)
{
    return (taskID == other.taskID && description == other.description &&
        other.creatorID == creatorID);
}

