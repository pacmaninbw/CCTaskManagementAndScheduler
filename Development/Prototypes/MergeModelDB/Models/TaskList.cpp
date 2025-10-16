// Project Header Files
#include "ListDBInterface.h"
#include "TaskList.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

TaskList::TaskList()
: ListDBInterface<TaskModel>()
{

}

TaskListValues TaskList::getActiveTasksForAssignedUser(std::size_t assignedUserID)
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In TaskList::getActiveTasksForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator.formatSelectActiveTasksForAssignedUser(assignedUserID);
        return runQueryFillTaskList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskListValues();
}

TaskListValues TaskList::getUnstartedDueForStartForAssignedUser(std::size_t assignedUserID)
{
    errorMessages.clear();
    appendErrorMessage("In TaskList::getUnstartedDueForStartForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator.formatSelectUnstartedDueForStartForAssignedUser(assignedUserID);
        return runQueryFillTaskList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskListValues();
}

TaskListValues TaskList::getTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
    std::chrono::year_month_day& searchStartDate)
{
    errorMessages.clear();
    appendErrorMessage("In TaskList::getTasksCompletedByAssignedAfterDate : ");

    try
    {
        firstFormattedQuery = queryGenerator.formatSelectTasksCompletedByAssignedAfterDate(
            assignedUserID, searchStartDate);
        return runQueryFillTaskList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskListValues();
}

TaskListValues TaskList::getTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID)
{
    errorMessages.clear();
    appendErrorMessage("In TaskList::getTasksByAssignedIDandParentID : ");

    try
    {
        firstFormattedQuery = queryGenerator.formatSelectTasksByAssignedIDandParentID(
            assignedUserID, parentID);
        return runQueryFillTaskList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskListValues();
}

TaskListValues TaskList::fillTaskList()
{
    TaskListValues taskList;

    for (auto taskID: primaryKeyResults)
    {
        TaskModel_shp newTask = std::make_shared<TaskModel>(TaskModel());
        newTask->selectByTaskID(taskID);
        taskList.push_back(newTask);
    }

    return taskList;
}

TaskListValues TaskList::runQueryFillTaskList()
{
    if (firstFormattedQuery.empty())
    {
        appendErrorMessage(std::format("Formatting select multiple tasks query string failed {}",
            queryGenerator.getAllErrorMessages()));
        return TaskListValues();
    }
    if (runFirstQuery())
    {
        return fillTaskList();
    }

    return TaskListValues();
}

