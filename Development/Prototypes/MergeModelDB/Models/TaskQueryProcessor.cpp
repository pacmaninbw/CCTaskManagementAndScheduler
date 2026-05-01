// Project Header Files
#include "QueryProcessor.h"
#include "TaskQueryProcessor.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

TaskQueryProcessor::TaskQueryProcessor()
: QueryProcessor<TaskModel>()
{

}

TaskQueryProcessorValues TaskQueryProcessor::getActiveTasksForAssignedUser(std::size_t assignedUserID) noexcept
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In TaskQueryProcessor::getActiveTasksForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectActiveTasksForAssignedUser(assignedUserID);
        return runQueryFillTaskQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskQueryProcessorValues();
}

TaskQueryProcessorValues TaskQueryProcessor::getUnstartedDueForStartForAssignedUser(std::size_t assignedUserID) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In TaskQueryProcessor::getUnstartedDueForStartForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectUnstartedDueForStartForAssignedUser(assignedUserID);
        return runQueryFillTaskQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskQueryProcessorValues();
}

TaskQueryProcessorValues TaskQueryProcessor::getTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
    std::chrono::year_month_day searchStartDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In TaskQueryProcessor::getTasksCompletedByAssignedAfterDate : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectTasksCompletedByAssignedAfterDate(
            assignedUserID, searchStartDate);
        return runQueryFillTaskQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskQueryProcessorValues();
}

TaskQueryProcessorValues TaskQueryProcessor::getTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In TaskQueryProcessor::getTasksByAssignedIDandParentID : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectTasksByAssignedIDandParentID(
            assignedUserID, parentID);
        return runQueryFillTaskQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskQueryProcessorValues();
}

TaskQueryProcessorValues TaskQueryProcessor::getDefaultDashboardTaskQueryProcessor(std::size_t assignedUserID, std::chrono::year_month_day searchStartDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In TaskQueryProcessor::getDefaultDashboardTaskQueryProcessor : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatDefaultTaskTableSelect(assignedUserID, searchStartDate);
        return runQueryFillTaskQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return TaskQueryProcessorValues();
}

TaskQueryProcessorValues TaskQueryProcessor::fillTaskQueryProcessor()
{
    TaskQueryProcessorValues TaskQueryProcessor;

    for (auto taskID: primaryKeyResults)
    {
        TaskModel_shp newTask = std::make_shared<TaskModel>(TaskModel());
        newTask->selectByTaskID(taskID);
        TaskQueryProcessor.push_back(newTask);
    }

    return TaskQueryProcessor;
}

TaskQueryProcessorValues TaskQueryProcessor::runQueryFillTaskQueryProcessor()
{
    if (firstFormattedQuery.empty())
    {
        appendErrorMessage(std::format("Formatting select multiple tasks query string failed {}",
            queryGenerator->getAllErrorMessages()));
        return TaskQueryProcessorValues();
    }
    
    if (runFirstQuery())
    {
        return fillTaskQueryProcessor();
    }

    return TaskQueryProcessorValues();
}

std::vector<ListExceptionTestElement> TaskQueryProcessor::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetActiveTasksForAssignedUser, this),
        "getActiveTasksForAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetUnstartedDueForStartForAssignedUser, this),
        "getUnstartedDueForStartForAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetTasksCompletedByAssignedAfterDate, this),
        "getTasksCompletedByAssignedAfterDate"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetTasksByAssignedIDandParentID, this),
        "getTasksByAssignedIDandParentID"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetDefaultDashboardTaskQueryProcessor, this),
        "getDefaultDashboardTaskQueryProcessor"});

    return exceptionTests;
}

TestStatus TaskQueryProcessor::testExceptionGetActiveTasksForAssignedUser() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::testExceptionGetActiveTasksForAssignedUser()",
         std::bind(&TaskQueryProcessor::getActiveTasksForAssignedUser, this, std::placeholders::_1), 1);
}

TestStatus TaskQueryProcessor::testExceptionGetUnstartedDueForStartForAssignedUser() noexcept
{
   selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::testExceptionGetUnstartedDueForStartForAssignedUser()",
         std::bind(&TaskQueryProcessor::getUnstartedDueForStartForAssignedUser, this, std::placeholders::_1), 1);
}

TestStatus TaskQueryProcessor::testExceptionGetTasksCompletedByAssignedAfterDate() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day searchStartDate = commonTestDateValue;
    std::size_t assignedUser = 1;

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::testExceptionGetTasksCompletedByAssignedAfterDate()",
         std::bind(&TaskQueryProcessor::getTasksCompletedByAssignedAfterDate, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, searchStartDate);
}

TestStatus TaskQueryProcessor::testExceptionGetTasksByAssignedIDandParentID() noexcept
{
    selfTestResetAllValues();

    std::size_t assignedUser = 1;
    std::size_t parentid = 1;

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::testExceptionGetUnstartedDueForStartForAssignedUser()",
         std::bind(&TaskQueryProcessor::getTasksByAssignedIDandParentID, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, parentid);

}

TestStatus TaskQueryProcessor::testExceptionGetDefaultDashboardTaskQueryProcessor() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day searchStartDate = commonProductionTestDataAddedDate;
    std::size_t assignedUser = 1;

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::testExceptionGetDefaultDashboardTaskQueryProcessor()",
         std::bind(&TaskQueryProcessor::getDefaultDashboardTaskQueryProcessor, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, searchStartDate);
}
