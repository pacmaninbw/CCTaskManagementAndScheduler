#include <chrono>
#include <ctime>
#include <format>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include "ModelBase.h"
#include "TaskModel.h"
#include "UserModel.h"

const std::size_t fieldCount = 16;

TaskModel::TaskModel()
    :  ModelBase("TaskID"),
    createdByUid{0},
    assignedToUid{0},
    description{"descriptionIn"},
    parentTaskID{0},
    status{TaskStatus::Not_Started},
    percentageComplete{0.0},
    createdOnDate{getTodaysDate()},
    requiredDeliveryDate{getTodaysDate()},
    scheduledStartDate{getTodaysDate()},
    actualStartDate{getTodaysDate()},
    estimatedCompletionDate{getTodaysDate()},
    completedDate{getTodaysDate()},
    estimatedEffortHours{0},
    actualEffortHours{0.0},
    priorityGroup{1},
    priority{1}
{

}

TaskModel::TaskModel(
    UserModel &creator, std::string &descriptionIn, unsigned int estimatedHoursEffort, std::string dueDate, std::string startDate,
    TaskModel *parentTaskp, TaskStatus statusIn, unsigned int majorPriority, unsigned int minorPriority)
    :  ModelBase("TaskID"),
    createdByUid{creator.getUserID()},
    assignedToUid{creator.getUserID()},
    description{descriptionIn},
    parentTaskID{0},
    status{statusIn},
    percentageComplete{0.0},
    createdOnDate{getTodaysDate()},
    requiredDeliveryDate{stringToDate(dueDate)},
    scheduledStartDate{stringToDate(startDate)},
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

TaskModel::TaskModel(
    std::size_t taskIDfromDB, std::size_t createdByUidfromDb, std::size_t assignedToUidfromDb, std::string descriptionfromDb,
    std::size_t parentTaskIDfromDb, unsigned int statusfromDb, double percentageCompletefromDb, std::string createdOnDatefromDb,
    std::string requiredDeliveryDatefromDb, std::string scheduledStartDatefromDb, std::string actualStartDatefromDb,
    std::string estimatedCompletionDatefromDb, std::string completedDatefromDb, unsigned int estimatedEffortHoursfromDb,
    double actualEffortHoursfromDb, unsigned int priorityGroupfromDb, unsigned int priorityfromDb)
    :  ModelBase("TaskID", taskIDfromDB),
    createdByUid{createdByUidfromDb},
    assignedToUid{assignedToUidfromDb},
    description{descriptionfromDb},
    parentTaskID{parentTaskIDfromDb},
    status{statusFromInt(statusfromDb)},
    percentageComplete{percentageCompletefromDb},
    createdOnDate{stringToDate(createdOnDatefromDb)},
    requiredDeliveryDate{stringToDate(requiredDeliveryDatefromDb)},
    scheduledStartDate{stringToDate(scheduledStartDatefromDb)},
    actualStartDate{stringToDate(actualStartDatefromDb)},
    estimatedCompletionDate{stringToDate(estimatedCompletionDatefromDb)},
    completedDate{stringToDate(completedDatefromDb)},
    estimatedEffortHours{estimatedEffortHoursfromDb},
    actualEffortHours{actualEffortHoursfromDb},
    priorityGroup{priorityGroupfromDb},
    priority{priorityfromDb}
{
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

void TaskModel::setCreationDate()
{
    createdOnDate = getTodaysDate();
}

std::string TaskModel::createDateString(int month, int day, int year)
{
    std::string dateString = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);

    return dateString;
}

std::string TaskModel::dateToString(std::chrono::year_month_day taskDate)
{
    std::stringstream ss;
    ss << taskDate;
    return ss.str();
}

std::chrono::year_month_day TaskModel::stringToDate(std::string dateString)
{
    std::chrono::year_month_day dateValue = getTodaysDate();

    // First try the ISO standard date.
    std::istringstream ss(dateString);
    ss >> std::chrono::parse("%Y-%m-%d", dateValue);
    if (!ss.fail())
    {
        return dateValue;
    }

    // The ISO standard didn't work, try some local dates
    std::locale usEnglish("en_US.UTF-8");
    std::vector<std::string> legalFormats = {
        {"%B %d, %Y"},
        {"%m/%d/%Y"},
        {"%m-%d-%Y"}
    };

    ss.imbue(usEnglish);
    for (auto legalFormat: legalFormats)
    {
        ss >> std::chrono::parse(legalFormat, dateValue);
        if (!ss.fail())
        {
            return dateValue;
        }
    }

    return dateValue;
}

std::chrono::year_month_day TaskModel::getTodaysDate()
{
    std::chrono::time_point<std::chrono::system_clock> today = std::chrono::system_clock::now();
    return std::chrono::floor<std::chrono::days>(today);
}
