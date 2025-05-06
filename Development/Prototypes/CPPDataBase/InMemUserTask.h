#ifndef INMEMUSERTASK_H_
#define INMEMUSERTASK_H_

#include "InMemUser.h"
#include <iostream>
#include <string>

class InMemUserTask
{
public:
    enum class TaskStatus
    {
        Not_Started=1, On_Hold=2, Waiting_for_Dependency=3, Work_in_Progress=4, Complete=5
    };

    InMemUserTask();
    // new task from GUI or program.
    InMemUserTask(InMemUser& creator, std::string& descriptionIn, unsigned int estimatedHoursEffort, std::string dueDate,
        std::string startDate, InMemUserTask* parentTaskp = nullptr, TaskStatus statusIn = TaskStatus::Not_Started,
        unsigned int majorPriority = 1, unsigned int minorPriority = 1
    );
    // existing task from database.
    InMemUserTask(std::size_t taskIDfromDB, std::size_t createdByUidfromDb, std::size_t assignedToUidfromDb,
        std::string descriptionfromDb, std::size_t parentTaskIDfromDb, unsigned int statusfromDb,
        double percentageCompletefromDb, std::string createdOnDatefromDb, std::string requiredDeliveryDatefromDb,
        std::string scheduledStartDatefromDb, std::string actualStartDatefromDb, std::string estimatedCompletionDatefromDb,
        std::string completedDatefromDb, unsigned int estimatedEffortHoursfromDb, double actualEffortHoursfromDb,
        unsigned int priorityGroupfromDb, unsigned int priorityfromDb      
    );
    ~InMemUserTask();
    std::string taskStatusString() const;

    friend std::ostream& operator<<(std::ostream& os, const InMemUserTask& obj)
    {
        os << "User Task:\n";
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
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<InMemUserTask::TaskStatus>(statusI); };

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

#endif // INMEMUSERTASK_H_
