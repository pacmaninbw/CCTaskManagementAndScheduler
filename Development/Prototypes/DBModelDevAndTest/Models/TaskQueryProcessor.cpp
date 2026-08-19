// Project Header Files
#include "QueryProcessor.h"
#include "chronoBoostConversions.h"
#include "TaskQueryProcessor.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

TaskQueryProcessor::TaskQueryProcessor()
: QueryProcessor<TaskModel, TaskDbQueryValues>("Task")
{
}

static boost::mysql::constant_string_view baseTaskQuery(
    "SELECT tasks.*, task_dependencies.dependent_task FROM tasks LEFT "
    "JOIN task_dependencies ON tasks.task_id = task_dependencies.dependency ");

TaskModel_shp TaskQueryProcessor::getTaskByTaskID(std::size_t taskId) noexcept
{
    clearErrorMessages();
    TaskModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, baseTaskQuery);
        boost::mysql::format_sql_to(fctx, "WHERE tasks.task_id = {}", taskId);

        StaticQueryTask localResult = staticRunQueryAsync<TaskDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, taskId, e.what()));
    }

    return found;
}

TaskList TaskQueryProcessor::getTaskByDescriptionAndAssignedUser(std::string_view description, std::size_t assignedUserID) noexcept
{
    clearErrorMessages();
    TaskList results;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, baseTaskQuery);
        boost::mysql::format_sql_to(fctx, "WHERE tasks.description = {} ", description);
        boost::mysql::format_sql_to(fctx, "AND tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1");

        StaticQueryTask localResult = staticRunQueryAsync<TaskDbQueryValues>(std::move(fctx).get().value());
        results = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return results;
}

TaskList TaskQueryProcessor::getActiveTasksForAssignedUser(std::size_t assignedUserID) noexcept
{
    clearErrorMessages();
    TaskList results;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, baseTaskQuery);
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.delivered IS NULL ");
        boost::mysql::format_sql_to(fctx, "AND (tasks.task_status IS NOT NULL AND tasks.task_status <> 0) ");
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1");

        StaticQueryTask localResult = staticRunQueryAsync<TaskDbQueryValues>(std::move(fctx).get().value());
        results = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return results;
}

TaskList TaskQueryProcessor::getUnstartedDueForStartForAssignedUser(std::size_t assignedUserID) noexcept
{
    clearErrorMessages();
    TaskList results;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, baseTaskQuery);
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.planned_start < {} ", common::toBoostMySQLDate(getTodaysDatePlus(OneWeek)));
        boost::mysql::format_sql_to(fctx, "AND (tasks.task_status IS NULL OR tasks.task_status = 0) ");
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1");

        StaticQueryTask localResult = staticRunQueryAsync<TaskDbQueryValues>(std::move(fctx).get().value());
        results = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return results;
}

TaskList TaskQueryProcessor::getTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
    std::chrono::year_month_day searchStartDate) noexcept
{
    clearErrorMessages();
    TaskList results;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, baseTaskQuery);
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.delivered >= {}", common::toBoostMySQLDate(searchStartDate));

        StaticQueryTask localResult = staticRunQueryAsync<TaskDbQueryValues>(std::move(fctx).get().value());
        results = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return results;
}

TaskList TaskQueryProcessor::getTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID) noexcept
{
    clearErrorMessages();
    TaskList results;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, baseTaskQuery);
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.parent_task = {} ", parentID);
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1");

        StaticQueryTask localResult = staticRunQueryAsync<TaskDbQueryValues>(std::move(fctx).get().value());
        results = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return results;
}

TaskList TaskQueryProcessor::getDefaultDashboardTaskList(std::size_t assignedUserID, std::chrono::year_month_day searchStartDate) noexcept
{
    clearErrorMessages();
    TaskList results;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, baseTaskQuery);
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.delivered IS NULL ");
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1 ");
        boost::mysql::format_sql_to(fctx, "AND (tasks.task_status = {} ", static_cast<unsigned int>(TaskModel::TaskStatus::Work_in_Progress));
        boost::mysql::format_sql_to(fctx, "OR tasks.due_date < {}) ", common::toBoostMySQLDate(searchStartDate));
        boost::mysql::format_sql_to(fctx, "ORDER BY tasks.priority_category ASC, tasks.priority ASC");

        StaticQueryTask localResult = staticRunQueryAsync<TaskDbQueryValues>(std::move(fctx).get().value());
        results = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return results;
}

std::vector<ListExceptionTestElement> TaskQueryProcessor::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetByTaskID, this),
        "getTaskByTaskID"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetByDescriptionAndAssignedUser, this),
        "getTaskByDescriptionAndAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetActiveTasksForAssignedUser, this),
        "getActiveTasksForAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetUnstartedDueForStartForAssignedUser, this),
        "getUnstartedDueForStartForAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetTasksCompletedByAssignedAfterDate, this),
        "getTasksCompletedByAssignedAfterDate"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetTasksByAssignedIDandParentID, this),
        "getTasksByAssignedIDandParentID"});
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetDefaultDashboardTaskList, this),
        "getDefaultDashboardTaskList"});

    return exceptionTests;
}

TestStatus TaskQueryProcessor::testExceptionGetByTaskID() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("TaskQueryProcessor::getTaskByTaskID",
        std::bind(&TaskQueryProcessor::getTaskByTaskID, this, std::placeholders::_1), 0);
}

TestStatus TaskQueryProcessor::testExceptionGetByDescriptionAndAssignedUser() noexcept
{
    selfTestResetAllValues();
    return testListExceptionAndSuccessNArgs
        ("TaskQueryProcessor::getTaskByDescriptionAndAssignedUser",
            std::bind(&TaskQueryProcessor::getTaskByDescriptionAndAssignedUser, this, std::placeholders::_1, std::placeholders::_2),
            "A task description", 1);
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

    std::chrono::year_month_day searchStartDate = common::TestDateValue;
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

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::getTasksByAssignedIDandParentID",
         std::bind(&TaskQueryProcessor::getTasksByAssignedIDandParentID, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, parentid);

}

TestStatus TaskQueryProcessor::testExceptionGetDefaultDashboardTaskList() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day searchStartDate = common::ProductionTestDataAddedDate;
    std::size_t assignedUser = 1;

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::testExceptionGetDefaultDashboardTaskList()",
         std::bind(&TaskQueryProcessor::getDefaultDashboardTaskList, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, searchStartDate);
}


