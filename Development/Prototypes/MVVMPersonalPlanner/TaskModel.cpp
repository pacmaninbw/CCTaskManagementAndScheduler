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
    addDataField("CreatedBy", PTS_DataField::PTS_DB_FieldType::Key, true);
    addDataField("AsignedTo", PTS_DataField::PTS_DB_FieldType::Key, true);
    addDataField("Description", PTS_DataField::PTS_DB_FieldType::VarChar256, true);
    addDataField("ParentTask", PTS_DataField::PTS_DB_FieldType::Key);
    addDataField("Status", PTS_DataField::PTS_DB_FieldType::UnsignedInt);
    addDataField("PercentageComplete", PTS_DataField::PTS_DB_FieldType::Double, true);
    addDataField("CreatedOn", PTS_DataField::PTS_DB_FieldType::Date, true);
    addDataField("RequiredDelivery", PTS_DataField::PTS_DB_FieldType::Date, true);
    addDataField("ScheduledStart", PTS_DataField::PTS_DB_FieldType::Date, true);
    addDataField("ActualStart", PTS_DataField::PTS_DB_FieldType::Date);
    addDataField("EstimatedCompletion", PTS_DataField::PTS_DB_FieldType::Date);
    addDataField("Completed", PTS_DataField::PTS_DB_FieldType::Date);
    addDataField("EstimatedEffortHours", PTS_DataField::PTS_DB_FieldType::UnsignedInt, true);
    addDataField("ActualEffortHours", PTS_DataField::PTS_DB_FieldType::Double, true);
    addDataField("SchedulePriorityGroup", PTS_DataField::PTS_DB_FieldType::UnsignedInt, true);
    addDataField("PriorityInGroup", PTS_DataField::PTS_DB_FieldType::UnsignedInt, true);

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

std::size_t TaskModel::getTaskID() const 
{
    return getPrimaryKey();
}

std::size_t TaskModel::getCreatorID() const 
{
    return getKeyFieldValue("CreatedBy");
}

std::size_t TaskModel::getAssignToID() const 
{
    return getKeyFieldValue("AsignedTo");
}

std::string TaskModel::getDescription() const 
{
    return getStringFieldValue("Description") ;
}

TaskModel::TaskStatus TaskModel::getStatus() const 
{
    return statusFromInt(getUnsignedIntFieldValue("Status"));
}

unsigned int TaskModel::getStatusIntVal() const
{
    return getUnsignedIntFieldValue("Status");
}

std::size_t TaskModel::getParentTaskID() const 
{
    return getKeyFieldValue("ParentTask");
}

double TaskModel::getPercentageComplete() const 
{
    return getDoubleFieldValue("PercentageComplete");
}

std::chrono::year_month_day TaskModel::getCreationDate() const 
{
    return getDateFieldValue("CreatedOn");
}

std::chrono::year_month_day TaskModel::getDueDate() const 
{
    return getDateFieldValue("RequiredDelivery");
}

std::chrono::year_month_day TaskModel::getScheduledStart() const 
{
    return getDateFieldValue("ScheduledStart");
}

std::chrono::year_month_day TaskModel::getactualStartDate() const 
{
    return getDateFieldValue("ActualStart");
}

std::chrono::year_month_day TaskModel::getEstimatedCompletion() const 
{
return getDateFieldValue("EstimatedCompletion");
}

std::chrono::year_month_day TaskModel::getCompletionDate() const
{
    return getDateFieldValue("Completed");
}

unsigned int TaskModel::getEstimatedEffort() const
{
    return getUnsignedIntFieldValue("EstimatedEffortHours");
}

double TaskModel::getactualEffortToDate() const
{
    return getDoubleFieldValue("ActualEffortHours");
}

unsigned int TaskModel::getPriorityGroup() const
{
    return getUnsignedIntFieldValue("SchedulePriorityGroup");
}

unsigned int TaskModel::getPriority() const
{
    return getUnsignedIntFieldValue("PriorityInGroup");
}

bool TaskModel::hasOptionalFieldStatus() const
{
    return fieldHasValue("Status");
}

bool TaskModel::hasOptionalFieldParentTaskID() const
{
    return fieldHasValue("ParentTask");
}

bool TaskModel::hasOptionalFieldActualStartDate() const
{
    return fieldHasValue("ActualStart");
}

bool TaskModel::hasOptionalFieldEstimatedCompletion() const
{
    return fieldHasValue("EstimatedCompletion");
}

bool TaskModel::hasOptionalFieldCompletionDate() const
{
    return fieldHasValue("Completed");
}

void TaskModel::setCreatorID(std::size_t creatorID)
{
    setFieldValue("CreatedBy", creatorID);
}

void TaskModel::setCreatorID(UserModel_shp creator)
{
    setCreatorID(creator->getPrimaryKey());
}

void TaskModel::setAssignToID(std::size_t assignedID)
{
    setFieldValue("AsignedTo", assignedID);
}

void TaskModel::setAssignToID(UserModel_shp assignedUser)
{
    setAssignToID(assignedUser->getPrimaryKey());
}

void TaskModel::setDescription(std::string description)
{
    setFieldValue("Description", description);
}

void TaskModel::setStatus(TaskModel::TaskStatus status)
{
    setFieldValue("Status", static_cast<unsigned int>(status));
}

void TaskModel::setStatus(std::string statusStr)
{
    TaskModel::TaskStatus status = stringToStatus(statusStr);
    if (status != UnknowStatus)
    {
        setStatus(status);
    }
}

void TaskModel::setParentTaskID(std::size_t parentTaskID)
{
    setFieldValue("ParentTask", parentTaskID);
}

void TaskModel::setParentTaskID(std::shared_ptr<TaskModel> parentTask)
{
    setParentTaskID(parentTask->getTaskID());
}

void TaskModel::setPercentageComplete(double percentComplete)
{
    setFieldValue("PercentageComplete", percentComplete);
}

void TaskModel::setCreationDate(std::chrono::year_month_day creationDate)
{
    setFieldValue("CreatedOn", creationDate);
}

void TaskModel::setDueDate(std::chrono::year_month_day dueDate)
{
    setFieldValue("RequiredDelivery", dueDate);
}

void TaskModel::setScheduledStart(std::chrono::year_month_day startDate)
{
    setFieldValue("ScheduledStart", startDate);
}

void TaskModel::setactualStartDate(std::chrono::year_month_day startDate)
{
    setFieldValue("ActualStart", startDate);
}

void TaskModel::setEstimatedCompletion(std::chrono::year_month_day completionDate)
{
    setFieldValue("EstimatedCompletion", completionDate);
}

void TaskModel::setCompletionDate(std::chrono::year_month_day completionDate)
{
    setFieldValue("Completed", completionDate);
}

void TaskModel::setEstimatedEffort(unsigned int estimatedHours)
{
    setFieldValue("EstimatedEffortHours", estimatedHours);
}

void TaskModel::setactualEffortToDate(double effortHoursYTD)
{
    setFieldValue("ActualEffortHours", effortHoursYTD);
}

void TaskModel::setPriorityGroup(unsigned int priorityGroup)
{
    setFieldValue("SchedulePriorityGroup", priorityGroup);
}

void TaskModel::setPriorityGroup(const char priorityGroup)
{
    unsigned int group = priorityGroup - 'A' + 1;
    setPriorityGroup(group);
}

void TaskModel::setPriority(unsigned int priority)
{
    setFieldValue("PriorityInGroup", priority);
}

void TaskModel::addEffortHours(double hours)
{
    double actualEffortHours = getactualEffortToDate();
    actualEffortHours += hours;
    setactualEffortToDate(actualEffortHours);
}

