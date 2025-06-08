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
    void dbSetTaskID(std::size_t dbTaskID) { setPrimaryKey(dbTaskID); };
    std::size_t getTaskID() const { return getPrimaryKey(); };
    std::size_t getCreatorID() const { return createdByUid; };
    std::size_t getAssignToID() const { return assignedToUid; };
    std::string getDescription() const { return description; };
    TaskModel::TaskStatus getStatus() const { return status; };
    std::size_t getParentTaskID() const { return parentTaskID; };
    double getPercentageComplete() const { return percentageComplete; };
    std::string getCreationDate() { return dateToString(createdOnDate) ; };
    std::string getDueDate() { return dateToString(requiredDeliveryDate); };
    std::string getScheduledStart() { return dateToString(scheduledStartDate); };
    std::string getactualStartDate() { return dateToString(actualStartDate); };
    std::string getEstimatedCompletion() { return dateToString(estimatedCompletionDate); };
    std::string getCompletionDate() { return dateToString(completedDate); };
    unsigned int getEstimatedEffort() const { return estimatedEffortHours; };
    double getactualEffortToDate() const { return actualEffortHours; };
    unsigned int getPriorityGoup() const { return priorityGroup; };
    unsigned int getPriority() const { return priority; };
    std::string taskStatusString() const;

    friend std::ostream& operator<<(std::ostream& os, const TaskModel& obj)
    {
        os << "TaskModel:\n";
        os << obj.primaryKeyFieldName;
        return os;
    };


private:
    void setCreationDate();
    std::string createDateString(int month, int day, int year);
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<TaskModel::TaskStatus>(statusI); };
    std::string dateToString(std::chrono::year_month_day taskDate);
    std::chrono::year_month_day stringToDate(std::string dateString);
    std::chrono::year_month_day getTodaysDate();

/*
 * Member variables.
 */
    // std::size_t taskID; // Using primaryKey as TaskID
    std::size_t createdByUid;
    std::size_t assignedToUid;
    std::string description;
    std::size_t parentTaskID;
    TaskStatus status;
    double percentageComplete;
    std::chrono::year_month_day createdOnDate;
    std::chrono::year_month_day requiredDeliveryDate;
    std::chrono::year_month_day scheduledStartDate;
    std::chrono::year_month_day actualStartDate;
    std::chrono::year_month_day estimatedCompletionDate;
    std::chrono::year_month_day completedDate;
    unsigned int estimatedEffortHours;
    double actualEffortHours;
    unsigned int priorityGroup;
    unsigned int priority;
};

#endif // TASKMODEL_H_


