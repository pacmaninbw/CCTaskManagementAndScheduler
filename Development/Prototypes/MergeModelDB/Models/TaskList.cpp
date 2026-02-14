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

TaskListValues TaskList::getActiveTasksForAssignedUser(std::size_t assignedUserID) noexcept
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In TaskList::getActiveTasksForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectActiveTasksForAssignedUser(assignedUserID);
        return runQueryFillTaskList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskListValues();
}

TaskListValues TaskList::getUnstartedDueForStartForAssignedUser(std::size_t assignedUserID) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In TaskList::getUnstartedDueForStartForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectUnstartedDueForStartForAssignedUser(assignedUserID);
        return runQueryFillTaskList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskListValues();
}

TaskListValues TaskList::getTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
    std::chrono::year_month_day searchStartDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In TaskList::getTasksCompletedByAssignedAfterDate : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectTasksCompletedByAssignedAfterDate(
            assignedUserID, searchStartDate);
        return runQueryFillTaskList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskListValues();
}

TaskListValues TaskList::getTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In TaskList::getTasksByAssignedIDandParentID : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectTasksByAssignedIDandParentID(
            assignedUserID, parentID);
        return runQueryFillTaskList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskListValues();
}

TaskListValues TaskList::getDefaultDashboardTaskList(std::size_t assignedUserID, std::chrono::year_month_day searchStartDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In TaskList::getDefaultDashboardTaskList : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatDefaultTaskTableSelect(assignedUserID, searchStartDate);
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
            queryGenerator->getAllErrorMessages()));
        return TaskListValues();
    }
    
    if (runFirstQuery())
    {
        return fillTaskList();
    }

    return TaskListValues();
}

std::vector<ListExceptionTestElement> TaskList::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&TaskList::testExceptionGetActiveTasksForAssignedUser, this),
        "getActiveTasksForAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskList::testExceptionGetUnstartedDueForStartForAssignedUser, this),
        "getUnstartedDueForStartForAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskList::testExceptionGetTasksCompletedByAssignedAfterDate, this),
        "getTasksCompletedByAssignedAfterDate"});
    exceptionTests.push_back({std::bind(&TaskList::testExceptionGetTasksByAssignedIDandParentID, this),
        "getTasksByAssignedIDandParentID"});
    exceptionTests.push_back({std::bind(&TaskList::testExceptionGetDefaultDashboardTaskList, this),
        "getDefaultDashboardTaskList"});

    return exceptionTests;
}

TestStatus TaskList::testExceptionGetActiveTasksForAssignedUser() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("TaskList::testExceptionGetActiveTasksForAssignedUser()",
         std::bind(&TaskList::getActiveTasksForAssignedUser, this, std::placeholders::_1), 1);
}

TestStatus TaskList::testExceptionGetUnstartedDueForStartForAssignedUser() noexcept
{
   selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("TaskList::testExceptionGetUnstartedDueForStartForAssignedUser()",
         std::bind(&TaskList::getUnstartedDueForStartForAssignedUser, this, std::placeholders::_1), 1);
}

TestStatus TaskList::testExceptionGetTasksCompletedByAssignedAfterDate() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day searchStartDate = commonTestDateValue;
    std::size_t assignedUser = 1;

    return testListExceptionAndSuccessNArgs("TaskList::testExceptionGetTasksCompletedByAssignedAfterDate()",
         std::bind(&TaskList::getTasksCompletedByAssignedAfterDate, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, searchStartDate);
}

TestStatus TaskList::testExceptionGetTasksByAssignedIDandParentID() noexcept
{
    selfTestResetAllValues();

    std::size_t assignedUser = 1;
    std::size_t parentid = 1;

    return testListExceptionAndSuccessNArgs("TaskList::testExceptionGetUnstartedDueForStartForAssignedUser()",
         std::bind(&TaskList::getTasksByAssignedIDandParentID, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, parentid);

}

TestStatus TaskList::testExceptionGetDefaultDashboardTaskList() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day searchStartDate = commonTestDateValue;
    std::size_t assignedUser = 1;

    return testListExceptionAndSuccessNArgs("TaskList::testExceptionGetTasksCompletedByAssignedAfterDate()",
         std::bind(&TaskList::getDefaultDashboardTaskList, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, searchStartDate);
}
