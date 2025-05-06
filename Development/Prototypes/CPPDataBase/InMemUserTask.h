#ifndef INMEMUSERTASK_H_
#define INMEMUSERTASK_H_

#include <iostream>
#include <string>

class InMemUserTask
{
public:
    enum class TaskStatus
    {
        Not_Started, On_Hold, Waiting_for_Dependency, Work_in_Progress, Complete
    };

    InMemUserTask();
    ~InMemUserTask();
    std::string taskStatusString() const;

    friend std::ostream& operator<<(std::ostream& os, const InMemUserTask& obj)
    {
        os << "User Task\n";
        os << "\tDatabase Task ID" << obj.taskID;
        return os;
    };


private:
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
