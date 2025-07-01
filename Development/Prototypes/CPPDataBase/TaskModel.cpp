#include <iostream>
#include "ModelBase.h"
#include <string>
#include "TaskModel.h"
#include "UserModel.h"

TaskModel::TaskModel()
    : ModelBase("TaskModel", "TaskID")
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
}

TaskModel::TaskModel(
    UserModel_shp creator, std::string &descriptionIn, unsigned int estimatedHoursEffort, std::string dueDate, std::string startDate,
    TaskModel *parentTaskp, TaskStatus statusIn, unsigned int majorPriority, unsigned int minorPriority)
    : TaskModel()
{
    setFieldValue("CreatedBy", creator->getUserID());
    setFieldValue("AsignedTo", creator->getUserID());
    setFieldValue("Description", descriptionIn);
    setFieldValue("EstimatedEffortHours", estimatedHoursEffort);
    setFieldValue("RequiredDelivery", stringToDate(dueDate));
    setFieldValue("ScheduledStart", stringToDate(startDate));
    setFieldValue("Status", static_cast<unsigned int>(statusIn));
    setFieldValue("SchedulePriorityGroup", majorPriority);
    setFieldValue("PriorityInGroup", minorPriority);
    setFieldValue("ActualEffortHours", 0.0);
    setFieldValue("PercentageComplete", 0.0);
   
    std::chrono::year_month_day today = getTodaysDate();
    setFieldValue("CreatedOn", today);

    if (parentTaskp)
    {
        setFieldValue("ParentTask", parentTaskp->getTaskID());
    }
}

TaskModel::~TaskModel()
{

}

std::string TaskModel::taskStatusString() const
{
    switch (status)
    {
        default:
            return "Unknown TaskStatus Value";
        case TaskStatus::Not_Started :
            return "Not Started";
        case TaskStatus::On_Hold:
            return "On Hold";
        case TaskStatus::Waiting_for_Dependency:
            return "Waiting for Dependency";
        case TaskStatus::Work_in_Progress:
            return "Work in Progress";
        case TaskStatus::Complete:
            return "Completed";
    }
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
    return statusFromInt(getIntFieldValue("Status"));
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

unsigned int TaskModel::getPriorityGoup() const
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

void TaskModel::setParentTaskID(std::size_t parentTaskID)
{
        setFieldValue("ParentTask", parentTaskID);
}

void TaskModel::setParentTaskID(std::shared_ptr<TaskModel> parentTask)
{
    setFieldValue("ParentTask", parentTask->getTaskID());
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

void TaskModel::setPriorityGoup(unsigned int priorityGroup)
{
    setFieldValue("SchedulePriorityGroup", priorityGroup);
}

void TaskModel::setPriorityGoup(const unsigned char priorityGroup)
{
    unsigned int group = priorityGroup - 'A';
    setFieldValue("SchedulePriorityGroup", group);
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
