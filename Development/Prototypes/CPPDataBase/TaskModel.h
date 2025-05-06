#ifndef TASKMODEL_H_
#define TASKMODEL_H_

#include "UserModel.h"
#include <iostream>
#include <string>

class TaskModel
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
    void dbSetTaskID(std::size_t dbTaskID) { taskID = dbTaskID; };
    std::size_t getCreatorID() const { return createdByUid; };
    std::size_t getAssignToID() const { return assignedToUid; };
    std::string getDescription() const { return description; };
    TaskModel::TaskStatus getStatus() const { return status; };
    std::size_t getParentTaskID() const { return parentTaskID; };
    double getPercentageComplete() const { return percentageComplete; };
    std::string getCreationDate() const { return createdOnDate; };
    std::string getDueDate() const { return requiredDeliveryDate; };
    std::string getScheduledStart() const { return scheduledStartDate; };
    std::string getactualStartDate() const { return actualStartDate; };
    std::string getEstimatedCompletion() const { return estimatedCompletionDate; };
    std::string getCompletionDate() const { return completedDate; };
    unsigned int getEstimatedEffort() const { return estimatedEffortHours; };
    double getactualEffortToDate() const { return actualEffortHours; };
    unsigned int getPriorityGoup() const { return priorityGroup; };
    unsigned int getPriority() const { return priority; };
    std::string taskStatusString() const;

    friend std::ostream& operator<<(std::ostream& os, const TaskModel& obj)
    {
        os << "TaskModel:\n";
        os << "\tDatabase Task ID:\t" << obj.taskID << "\n";
        os << "\tCreated By User ID:\t" << obj.createdByUid << "\n";
        os << "\tAssign to User ID:\t" << obj.assignedToUid << "\n";
        os << "\tDescription:\t" << obj.description << "\n";
        os << "\tStatus integer value:\t" << static_cast<int>(obj.status) << "\n";
        os << "\tParent Task ID:\t" << obj.parentTaskID << "\n";
        os << "\tPercentage Complete:\t" << obj.percentageComplete << "%\n";
        os << "\tCreated on date:\t" << obj.createdOnDate << "\n";
        os << "\tRequired Deliverly date:\t" << obj.requiredDeliveryDate << "\n";
        os << "\tScheduled Start date:\t" << obj.scheduledStartDate << "\n";
        os << "\tActual Start date:\t" << obj.actualStartDate << "\n";
        os << "\tEstimated Completion date:\t" << obj.estimatedCompletionDate << "\n";
        os << "\tCompleted date:\t" << obj.completedDate << "\n";
        os << "\tEstimated Effort in hours:\t" << obj.estimatedEffortHours << "\n";
        os << "\tActual Effort Hours to date:\t" << obj.actualEffortHours << "\n";
        os << "\tPriority Group:\t" << obj.priorityGroup << "\n";
        os << "\tPriority:\t" << obj.priority << "%\n";
        return os;
    };


private:
    void setCreationDate();
    std::string createDateString(int month, int day, int year);
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<TaskModel::TaskStatus>(statusI); };

/*
 * Member variables.
 */
    std::size_t taskID;
    std::size_t createdByUid;
    std::size_t assignedToUid;
    std::string description;
    std::size_t parentTaskID;
    TaskStatus status;
    double percentageComplete;
    std::string createdOnDate;
    std::string requiredDeliveryDate;
    std::string scheduledStartDate;
    std::string actualStartDate;
    std::string estimatedCompletionDate;
    std::string completedDate;
    unsigned int estimatedEffortHours;
    double actualEffortHours;
    unsigned int priorityGroup;
    unsigned int priority;
};

#endif // TASKMODEL_H_
