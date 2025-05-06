#include <ctime>
#include "InMemUser.h"
#include "InMemUserTask.h"
#include <iostream>
#include <string>

InMemUserTask::InMemUserTask()
:   taskID{0},
    createdByUid{0},
    assignedToUid{0},
    description{"descriptionIn"},
    parentTaskID{0},
    status{TaskStatus::Not_Started},
    percentageComplete{0.0},
    createdOnDate{""},
    requiredDeliveryDate{""},
    scheduledStartDate{""},
    actualStartDate{""},
    estimatedCompletionDate{""},
    completedDate{""},
    estimatedEffortHours{0},
    actualEffortHours{0.0},
    priorityGroup{1},
    priority{1}
{

}

InMemUserTask::InMemUserTask(
    InMemUser &creator, std::string &descriptionIn, unsigned int estimatedHoursEffort, std::string dueDate, std::string startDate,
    InMemUserTask *parentTaskp, TaskStatus statusIn, unsigned int majorPriority, unsigned int minorPriority)
    : taskID{0},
    createdByUid{creator.getUserID()},
    assignedToUid{creator.getUserID()},
    description{descriptionIn},
    parentTaskID{0},
    status{statusIn},
    percentageComplete{0.0},
    createdOnDate{""},
    requiredDeliveryDate{dueDate},
    scheduledStartDate{startDate},
    actualStartDate{""},
    estimatedCompletionDate{""},
    completedDate{""},
    estimatedEffortHours{estimatedHoursEffort},
    actualEffortHours{0.0},
    priorityGroup{majorPriority},
    priority{minorPriority}
{
    setCreationDate();

    if (parentTaskp)
    {
        parentTaskID = parentTaskp->parentTaskID;
    }
}

InMemUserTask::InMemUserTask(
    std::size_t taskIDfromDB, std::size_t createdByUidfromDb, std::size_t assignedToUidfromDb, std::string descriptionfromDb,
    std::size_t parentTaskIDfromDb, unsigned int statusfromDb, double percentageCompletefromDb, std::string createdOnDatefromDb,
    std::string requiredDeliveryDatefromDb, std::string scheduledStartDatefromDb, std::string actualStartDatefromDb,
    std::string estimatedCompletionDatefromDb, std::string completedDatefromDb, unsigned int estimatedEffortHoursfromDb,
    double actualEffortHoursfromDb, unsigned int priorityGroupfromDb, unsigned int priorityfromDb)
    : taskID{taskIDfromDB},
    createdByUid{createdByUidfromDb},
    assignedToUid{assignedToUidfromDb},
    description{descriptionfromDb},
    parentTaskID{parentTaskIDfromDb},
    status{statusFromInt(statusfromDb)},
    percentageComplete{percentageCompletefromDb},
    createdOnDate{createdOnDatefromDb},
    requiredDeliveryDate{requiredDeliveryDatefromDb},
    scheduledStartDate{scheduledStartDatefromDb},
    actualStartDate{actualStartDatefromDb},
    estimatedCompletionDate{estimatedCompletionDatefromDb},
    completedDate{completedDatefromDb},
    estimatedEffortHours{estimatedEffortHoursfromDb},
    actualEffortHours{actualEffortHoursfromDb},
    priorityGroup{priorityGroupfromDb},
    priority{priorityfromDb}
{
}

InMemUserTask::~InMemUserTask()
{

}

std::string InMemUserTask::taskStatusString() const
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

void InMemUserTask::setCreationDate()
{
    std::time_t currentTime = std::time(0);
    std::tm* now = std::localtime(&currentTime);

    int year = now->tm_year + 1900;
    int month = now->tm_mon + 1;
    int day = now->tm_mday;

    createdOnDate = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
}

