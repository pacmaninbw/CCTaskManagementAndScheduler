#include <chrono>
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

bool TaskModel::getDatabaseValues()
{
    return false;
}

bool TaskModel::updateDatabase()
{
    return false;
}

std::chrono::year_month_day TaskModel::getactualStartDate() const
{
    return std::chrono::year_month_day();
}

std::chrono::year_month_day TaskModel::getEstimatedCompletion() const
{
    return std::chrono::year_month_day();
}

std::chrono::year_month_day TaskModel::getCompletionDate() const
{
    return std::chrono::year_month_day();
}

void TaskModel::setCreatorID(std::size_t creatorID)
{
    modified = true;
    creatorID = creatorID;
}

void TaskModel::setCreatorID(UserModel_shp creator)
{
    setCreatorID(creator->getUserID());
}

void TaskModel::setAssignToID(std::size_t assignedID)
{
    modified = true;
    assignToID = assignedID;
}

void TaskModel::setAssignToID(UserModel_shp assignedUser)
{
    setAssignToID(assignedUser->getUserID());
}

void TaskModel::setDescription(std::string description)
{
    modified = true;
    description = description;
}

void TaskModel::setStatus(TaskModel::TaskStatus status)
{
    modified = true;
    status = status;
}

void TaskModel::setStatus(std::string statusStr)
{
    setStatus(stringToStatus(statusStr));
}

void TaskModel::setParentTaskID(std::size_t parentTaskID)
{
    modified = true;
    parentTaskID = parentTaskID;
}

void TaskModel::setParentTaskID(std::shared_ptr<TaskModel> parentTask)
{
    setParentTaskID(parentTask->getTaskID());
}

void TaskModel::setPercentageComplete(double percentComplete)
{
    modified = true;
    percentageComplete = percentComplete;
}

void TaskModel::setCreationDate(std::chrono::year_month_day creationDate)
{
    modified = true;
    creationDate = creationDate;
}

void TaskModel::setDueDate(std::chrono::year_month_day dueDate)
{
    modified = true;
    dueDate = dueDate;
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

void TaskModel::setCompletionDate(std::chrono::year_month_day completionDate)
{
    modified = true;
    completionDate = completionDate;
}

void TaskModel::setEstimatedEffort(unsigned int estimatedHours)
{
    modified = true;
    estimatedEffort = estimatedHours;
}

void TaskModel::setactualEffortToDate(double effortHoursYTD)
{
    modified = true;
    actualEffortToDate = effortHoursYTD;
}

void TaskModel::setPriorityGroup(unsigned int priorityGroup)
{
    modified = true;
    priorityGroup = priorityGroup;
}

void TaskModel::setPriorityGroup(const char priorityGroup)
{
    unsigned int group = priorityGroup - 'A' + 1;
    setPriorityGroup(group);
}

void TaskModel::setPriority(unsigned int priority)
{
    modified = true;
    priority = priority;
}

void TaskModel::setPersonal(bool personalIn)
{
    modified = true;
    personal = personalIn;
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


void TaskModel::addEffortHours(double hours)
{
    double actualEffortHours = getactualEffortToDate();
    actualEffortHours += hours;
    setactualEffortToDate(actualEffortHours);
}

std::chrono::year_month_day TaskModel::getTodaysDate()
{
    std::chrono::time_point<std::chrono::system_clock> today = std::chrono::system_clock::now();
    return std::chrono::floor<std::chrono::days>(today);
}

bool TaskModel::diffTask(TaskModel& other)
{
    return (taskID == other.taskID && description == other.description &&
        other.creatorID == creatorID);
}

