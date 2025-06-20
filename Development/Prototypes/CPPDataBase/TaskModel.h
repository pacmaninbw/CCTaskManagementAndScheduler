#ifndef TASKMODEL_H_
#define TASKMODEL_H_

#include <chrono>
#include <iostream>
#include <string>
#include "ModelBase.h"
#include "UserModel.h"

class TaskModel : public ModelBase
{
public:
    enum class TaskStatus
    {
        Not_Started=1, On_Hold=2, Waiting_for_Dependency=3, Work_in_Progress=4, Complete=5
    };

    TaskModel();
    // new task from GUI or program.
    TaskModel(UserModel& creator, std::string& descriptionIn, unsigned int estimatedHoursEffort, std::string dueDate,
        std::string startDate, TaskModel* parentTaskp = nullptr, TaskStatus statusIn = TaskStatus::Not_Started,
        unsigned int majorPriority = 1, unsigned int minorPriority = 1
    );
    // existing task from database.
    TaskModel(std::size_t taskIDfromDB, std::size_t createdByUidfromDb, std::size_t assignedToUidfromDb,
        std::string descriptionfromDb, std::size_t parentTaskIDfromDb, unsigned int statusfromDb,
        double percentageCompletefromDb, std::string createdOnDatefromDb, std::string requiredDeliveryDatefromDb,
        std::string scheduledStartDatefromDb, std::string actualStartDatefromDb, std::string estimatedCompletionDatefromDb,
        std::string completedDatefromDb, unsigned int estimatedEffortHoursfromDb, double actualEffortHoursfromDb,
        unsigned int priorityGroupfromDb, unsigned int priorityfromDb      
    );
    ~TaskModel();
    std::size_t getTaskID() const;
    std::size_t getCreatorID() const;
    std::size_t getAssignToID() const;
    std::string getDescription() const;
    TaskModel::TaskStatus getStatus() const;
    std::size_t getParentTaskID() const;
    double getPercentageComplete() const;
    std::chrono::year_month_day getCreationDate() const;
    std::chrono::year_month_day getDueDate() const;
    std::chrono::year_month_day getScheduledStart() const;
    std::chrono::year_month_day getactualStartDate() const;
    std::chrono::year_month_day getEstimatedCompletion() const;
    std::chrono::year_month_day getCompletionDate() const;
    unsigned int getEstimatedEffort() const;
    float getactualEffortToDate() const;
    unsigned int getPriorityGoup() const;
    unsigned int getPriority() const;
    bool hasOptionalFieldStatus() const;
    bool hasOptionalFieldParentTaskID() const;
    bool hasOptionalFieldActualStartDate() const;
    bool hasOptionalFieldEstimatedCompletion() const;
    bool hasOptionalFieldCompletionDate() const;
    std::string taskStatusString() const;

    friend std::ostream& operator<<(std::ostream& os, const TaskModel& obj)
    {
        os << "TaskModel:\n";
        os << obj.primaryKeyFieldName;
        os << "Column Names: ";
        for (const auto& [key, value] : obj.dataFields)
        {
            os << key;
            os << ", ";
        }
        os << "\n";
        for (const auto& [key, value] : obj.dataFields)
        {
            PTS_DataField_shp currentField = value;
            os << currentField->fieldInfo();
            os << "\n";
        }
        return os;
    };


private:
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<TaskModel::TaskStatus>(statusI); };

/*
 * Member variables.
 */
    // std::size_t taskID; // Using primaryKey as TaskID
    TaskStatus status;
};

#endif // TASKMODEL_H_


