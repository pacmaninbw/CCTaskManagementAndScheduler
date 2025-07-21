#include <chrono>
#include "GenericDictionary.h"
#include <iostream>
#include <memory>
#include "ModelBase.h"
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
    : ModelBase("TaskModel", "Tasks", "TaskID")
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

