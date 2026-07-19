// Project Header Files
#include "QueryProcessor.h"
#include "TaskQueryProcessor.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

TaskQueryProcessor::TaskQueryProcessor()
: QueryProcessor<TaskModel>("Task", {
        "task_id", "created_by", "assigned_to", "description", "parent_task", "task_status", "creation_timestamp",
        "due_date", "planned_start", "actual_start", "estimated_delivery", "delivered",
        "est_hours_effort", "hours_effort", "priority_category", "priority",
        "personal", "dependency_count", "dependencies", "last_modified_time_stamp", "deleted"
    }
)
{

}

TaskModel_shp TaskQueryProcessor::getTaskByTaskID(std::size_t taskId) noexcept
{
    clearErrorMessages();
    TaskModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM tasks WHERE tasks.task_id = {}", taskId);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        found = getOneResult(localResult);
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

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM tasks ");
        boost::mysql::format_sql_to(fctx, "WHERE tasks.description = {} ", description);
        boost::mysql::format_sql_to(fctx, "AND tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1");
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return TaskList();
}

TaskList TaskQueryProcessor::getActiveTasksForAssignedUser(std::size_t assignedUserID) noexcept
{
    clearErrorMessages();

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM tasks ");
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.delivered IS NULL ");
        boost::mysql::format_sql_to(fctx, "AND (tasks.task_status IS NOT NULL AND tasks.task_status <> 0) ");
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1");
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return TaskList();
}

TaskList TaskQueryProcessor::getUnstartedDueForStartForAssignedUser(std::size_t assignedUserID) noexcept
{
    clearErrorMessages();

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM tasks ");
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.planned_start < {} ", stdchronoDateToBoostMySQLDate(getTodaysDatePlus(OneWeek)));
        boost::mysql::format_sql_to(fctx, "AND (tasks.task_status IS NULL OR tasks.task_status = 0) ");
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1");
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return TaskList();
}

TaskList TaskQueryProcessor::getTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
    std::chrono::year_month_day searchStartDate) noexcept
{
    clearErrorMessages();

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM tasks ");
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.delivered >= {}", stdchronoDateToBoostMySQLDate(searchStartDate));
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return TaskList();
}

TaskList TaskQueryProcessor::getTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID) noexcept
{
    clearErrorMessages();

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM tasks ");
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.parent_task = {} ", parentID);
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1");
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return TaskList();
}

TaskList TaskQueryProcessor::getDefaultDashboardTaskList(std::size_t assignedUserID, std::chrono::year_month_day searchStartDate) noexcept
{
    clearErrorMessages();

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM tasks ");
        boost::mysql::format_sql_to(fctx, "WHERE tasks.assigned_to = {} ", assignedUserID);
        boost::mysql::format_sql_to(fctx, "AND tasks.delivered IS NULL ");
        boost::mysql::format_sql_to(fctx, "AND tasks.deleted <> 1 ");
        boost::mysql::format_sql_to(fctx, "AND (tasks.task_status = {} ", static_cast<unsigned int>(TaskModel::TaskStatus::Work_in_Progress));
        boost::mysql::format_sql_to(fctx, "OR tasks.due_date < {}) ", stdchronoDateToBoostMySQLDate(searchStartDate));
        boost::mysql::format_sql_to(fctx, "ORDER BY tasks.priority_category ASC, tasks.priority ASC");
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, assignedUserID, e.what()));
    }
    
    return TaskList();
}


TaskModel_shp TaskQueryProcessor::processResultRow(boost::mysql::row_view &queryRow)
{
    std::optional<TaskModel::TaskStatus> statusVal;
    std::optional<std::size_t> parentTaskID;
    std::optional<std::chrono::system_clock::time_point> creationTimeStamp;
    std::optional<std::chrono::year_month_day> dueDate;
    std::optional<std::chrono::year_month_day> scheduledStart;
    std::optional<std::chrono::year_month_day> actualStartDate;
    std::optional<std::chrono::year_month_day> estimatedCompletion;
    std::optional<std::chrono::year_month_day> completionDate;
    double estimatedEffort;
    double actualEffortToDate;
    unsigned int priorityGroup;
    unsigned int priority;
    bool personal;
    std::optional<std::chrono::system_clock::time_point> lastUpdate;
    std::size_t dependencyCount;
    std::string dependenciesText;

    // Required fields.
    std::size_t taskId = queryRow.at(m_taskIdIdx).as_uint64();
    std::size_t creatorID = queryRow.at(m_creatorIdx).as_uint64();
    std::size_t assignToID = queryRow.at(m_assigneeIdx).as_uint64();
    std::string description = queryRow.at(m_descriptionIdx).as_string();
    creationTimeStamp = boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_createdIdx).as_datetime());
    dueDate = boostMysqlDateToChronoDate(queryRow.at(m_dueDateIdx).as_date());
    scheduledStart = boostMysqlDateToChronoDate(queryRow.at(m_planedStartIdx).as_date());
    estimatedEffort = queryRow.at(m_estimatedEffortIdx).as_double();
    actualEffortToDate = queryRow.at(m_efforToDateIdx).as_double();
    priorityGroup = queryRow.at(m_priorityCategoryIdx).as_uint64();
    priority = queryRow.at(m_priorityIdx).as_uint64();
    personal = queryRow.at(m_personalIdx).as_int64();
    lastUpdate = boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_lastUpdateIdx).as_datetime());


    // Optional fields.
    if (!queryRow.at(m_parentIdx).is_null())
    {
        parentTaskID = queryRow.at(m_parentIdx).as_uint64();
    }

    if (!queryRow.at(m_statusIdx).is_null())
    {
        statusVal = static_cast<TaskModel::TaskStatus>(queryRow.at(m_statusIdx).as_uint64());
    }

    if (!queryRow.at(m_actualStartIdx).is_null())
    {
        actualStartDate = boostMysqlDateToChronoDate(queryRow.at(m_actualStartIdx).as_date());
    }

    if (!queryRow.at(m_planedEndIdx).is_null())
    {
        estimatedCompletion = boostMysqlDateToChronoDate(queryRow.at(m_planedEndIdx).as_date());
    }

    if (!queryRow.at(m_completedIdx).is_null())
    {
        completionDate = boostMysqlDateToChronoDate(queryRow.at(m_completedIdx).as_date());
    }

    dependencyCount = queryRow.at(m_dependencyCountIdx).as_uint64();
    if (dependencyCount > 0)
    {
        dependenciesText = queryRow.at(m_depenedencyListIdx).as_string();
    }

    return std::make_shared<TaskModel>(taskId, creatorID, assignToID, description, statusVal, parentTaskID,
        dueDate, scheduledStart, actualStartDate, estimatedCompletion, completionDate, estimatedEffort,
        actualEffortToDate, priorityGroup, priority, personal, dependencyCount, dependenciesText,
        creationTimeStamp, lastUpdate);

}

void TaskQueryProcessor::fillRequiredIndexes()
{
    assignValueToIndex("task_id", m_taskIdIdx);
    assignValueToIndex("created_by", m_creatorIdx);
    assignValueToIndex("assigned_to", m_assigneeIdx);
    assignValueToIndex("description", m_descriptionIdx);
    assignValueToIndex("parent_task", m_parentIdx);
    assignValueToIndex("task_status", m_statusIdx);
    assignValueToIndex("creation_timestamp", m_createdIdx);
    assignValueToIndex("due_date", m_dueDateIdx);
    assignValueToIndex("planned_start", m_planedStartIdx);
    assignValueToIndex("actual_start", m_actualStartIdx);
    assignValueToIndex("estimated_delivery", m_planedEndIdx);
    assignValueToIndex("delivered", m_completedIdx);
    assignValueToIndex("est_hours_effort", m_estimatedEffortIdx);
    assignValueToIndex("hours_effort", m_efforToDateIdx);
    assignValueToIndex("priority_category", m_priorityCategoryIdx);
    assignValueToIndex("priority", m_priorityIdx);
    assignValueToIndex("personal", m_personalIdx);
    assignValueToIndex("dependency_count", m_dependencyCountIdx);
    assignValueToIndex("dependencies", m_depenedencyListIdx);
    assignValueToIndex("last_modified_time_stamp", m_lastUpdateIdx);
    assignValueToIndex("deleted", m_hiddenIdx);
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

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::getTasksByAssignedIDandParentID",
         std::bind(&TaskQueryProcessor::getTasksByAssignedIDandParentID, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, parentid);

}

TestStatus TaskQueryProcessor::testExceptionGetDefaultDashboardTaskList() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day searchStartDate = commonProductionTestDataAddedDate;
    std::size_t assignedUser = 1;

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::testExceptionGetDefaultDashboardTaskList()",
         std::bind(&TaskQueryProcessor::getDefaultDashboardTaskList, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, searchStartDate);
}

