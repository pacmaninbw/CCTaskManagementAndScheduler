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
    addDataField("Status", PTS_DataField::PTS_DB_FieldType::Int);
    addDataField("PercentageComplete", PTS_DataField::PTS_DB_FieldType::Double, true);
    addDataField("CreatedOn", PTS_DataField::PTS_DB_FieldType::Date, true);
    addDataField("RequiredDelivery", PTS_DataField::PTS_DB_FieldType::Date, true);
    addDataField("ScheduledStart", PTS_DataField::PTS_DB_FieldType::Date, true);
    addDataField("ActualStart", PTS_DataField::PTS_DB_FieldType::Date);
    addDataField("EstimatedCompletion", PTS_DataField::PTS_DB_FieldType::Date);
    addDataField("Completed", PTS_DataField::PTS_DB_FieldType::Date);
    addDataField("EstimatedEffortHours", PTS_DataField::PTS_DB_FieldType::UnsignedInt, true);
    addDataField("ActualEffortHours", PTS_DataField::PTS_DB_FieldType::Float, true);
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
    setFieldValue("Status", static_cast<int>(statusIn));
    setFieldValue("SchedulePriorityGroup", majorPriority);
    setFieldValue("PriorityInGroup", minorPriority);
    setFieldValue("ActualEffortHours", static_cast<float>(0.0));
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

std::size_t TaskModel::getTaskID() const { return getPrimaryKey(); }
std::size_t TaskModel::getCreatorID() const { return getKeyFieldValue("CreatedBy"); }
std::size_t TaskModel::getAssignToID() const { return getKeyFieldValue("AsignedTo"); }
std::string TaskModel::getDescription() const { return getStringFieldValue("Description") ; }
TaskModel::TaskStatus TaskModel::getStatus() const { return statusFromInt(getIntFieldValue("Status")); }
std::size_t TaskModel::getParentTaskID() const { return getKeyFieldValue("ParentTask"); }
double TaskModel::getPercentageComplete() const { return getDoubleFieldValue("PercentageComplete"); }
std::chrono::year_month_day TaskModel::getCreationDate() const { return getDateFieldValue("CreatedOn"); }
std::chrono::year_month_day TaskModel::getDueDate() const { return getDateFieldValue("RequiredDelivery"); }
std::chrono::year_month_day TaskModel::getScheduledStart() const { return getDateFieldValue("ScheduledStart"); }
std::chrono::year_month_day TaskModel::getactualStartDate() const { return getDateFieldValue("ActualStart"); }
std::chrono::year_month_day TaskModel::getEstimatedCompletion() const { return getDateFieldValue("EstimatedCompletion"); }
std::chrono::year_month_day TaskModel::getCompletionDate() const { return getDateFieldValue("Completed"); }
unsigned int TaskModel::getEstimatedEffort() const { return getUnsignedIntFieldValue("EstimatedEffortHours"); }
float TaskModel::getactualEffortToDate() const { return getFloatFieldValue("ActualEffortHours"); }
unsigned int TaskModel::getPriorityGoup() const { return getUnsignedIntFieldValue("SchedulePriorityGroup"); }
unsigned int TaskModel::getPriority() const { return getUnsignedIntFieldValue("PriorityInGroup"); }
bool TaskModel::hasOptionalFieldStatus() const { return fieldHasValue("Status"); }
bool TaskModel::hasOptionalFieldParentTaskID() const { return fieldHasValue("ParentTask"); }
bool TaskModel::hasOptionalFieldActualStartDate() const { return fieldHasValue("ActualStart"); }
bool TaskModel::hasOptionalFieldEstimatedCompletion() const { return fieldHasValue("EstimatedCompletion"); }
bool TaskModel::hasOptionalFieldCompletionDate() const { return fieldHasValue("Completed"); }

