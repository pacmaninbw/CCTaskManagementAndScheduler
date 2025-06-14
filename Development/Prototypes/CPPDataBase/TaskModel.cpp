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
    UserModel &creator, std::string &descriptionIn, unsigned int estimatedHoursEffort, std::string dueDate, std::string startDate,
    TaskModel *parentTaskp, TaskStatus statusIn, unsigned int majorPriority, unsigned int minorPriority)
    : TaskModel()
{
    setFieldValue("CreatedBy", creator.getUserID());
    setFieldValue("AsignedTo", creator.getUserID());
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

TaskModel::TaskModel(
    std::size_t taskIDfromDB, std::size_t createdByUidfromDb, std::size_t assignedToUidfromDb, std::string descriptionfromDb,
    std::size_t parentTaskIDfromDb, unsigned int statusfromDb, double percentageCompletefromDb, std::string createdOnDatefromDb,
    std::string requiredDeliveryDatefromDb, std::string scheduledStartDatefromDb, std::string actualStartDatefromDb,
    std::string estimatedCompletionDatefromDb, std::string completedDatefromDb, unsigned int estimatedEffortHoursfromDb,
    double actualEffortHoursfromDb, unsigned int priorityGroupfromDb, unsigned int priorityfromDb)
    : TaskModel()
{
    setPrimaryKey(taskIDfromDB);
    initFieldValueNotChanged("CreatedBy", createdByUidfromDb);
    initFieldValueNotChanged("AsignedTo", assignedToUidfromDb);
    initFieldValueNotChanged("Description", descriptionfromDb);
    initFieldValueNotChanged("ParentTask", parentTaskIDfromDb);
    initFieldValueNotChanged("Status", statusfromDb);
    initFieldValueNotChanged("PercentageComplete", percentageCompletefromDb);
    initFieldValueNotChanged("CreatedOn", stringToDate(createdOnDatefromDb));
    initFieldValueNotChanged("RequiredDelivery", stringToDate(requiredDeliveryDatefromDb));
    initFieldValueNotChanged("ScheduledStart", stringToDate(scheduledStartDatefromDb));
    initFieldValueNotChanged("ActualStart", stringToDate(actualStartDatefromDb));
    initFieldValueNotChanged("EstimatedCompletion", stringToDate(estimatedCompletionDatefromDb));
    initFieldValueNotChanged("Completed", stringToDate(completedDatefromDb));
    initFieldValueNotChanged("EstimatedEffortHours", estimatedEffortHoursfromDb);
    initFieldValueNotChanged("ActualEffortHours", actualEffortHoursfromDb);
    initFieldValueNotChanged("SchedulePriorityGroup", priorityGroupfromDb);
    initFieldValueNotChanged("PriorityInGroup", priorityfromDb);
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

