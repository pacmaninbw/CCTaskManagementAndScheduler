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
        "TaskID", "CreatedBy", "AsignedTo", "Description", "ParentTask", "Status", "CreatedOn",
        "RequiredDelivery", "ScheduledStart", "ActualStart", "EstimatedCompletion", "Completed",
        "EstimatedEffortHours", "ActualEffortHours", "SchedulePriorityGroup", "PriorityInGroup",
        "Personal", "DependencyCount", "Dependencies", "LastUpdateTS", "Hidden"
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetTaskByTaskID({})", taskId);
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetTaskByDescriptionAndAssignedUser({}, {})", assignedUserID, description);
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetActiveTasksForAssignedUser({})", assignedUserID);
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetUnstartedDueForStartForAssignedUser({}, {})",
            assignedUserID, stdchronoDateToBoostMySQLDate(getTodaysDatePlus(OneWeek)));
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetTasksCompletedByAssignedAfterDate({},{})",
        assignedUserID, stdchronoDateToBoostMySQLDate(searchStartDate));
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetTasksByAssignedIDandParentID({}, {})", assignedUserID, parentID);
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetDefaultDashboardTaskList({}, {})", assignedUserID, stdchronoDateToBoostMySQLDate(searchStartDate));
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
    unsigned int estimatedEffort;
    double actualEffortToDate;
    unsigned int priorityGroup;
    unsigned int priority;
    bool personal;
    std::optional<std::chrono::system_clock::time_point> lastUpdate;
    std::size_t dependencyCount;
    std::string dependenciesText;

    // Required fields.
    std::size_t taskId = queryRow.at(m_TaskIdIdx).as_uint64();
    std::size_t creatorID = queryRow.at(m_CreatedByIdx).as_uint64();
    std::size_t assignToID = queryRow.at(m_AssignedToIdx).as_uint64();
    std::string description = queryRow.at(m_DescriptionIdx).as_string();
    creationTimeStamp = boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_CreatedOnIdx).as_datetime());
    dueDate = boostMysqlDateToChronoDate(queryRow.at(m_DueDateIdx).as_date());
    scheduledStart = boostMysqlDateToChronoDate(queryRow.at(m_PlanedStartIdx).as_date());
    estimatedEffort = queryRow.at(m_EstEffortHoursIdx).as_uint64();
    actualEffortToDate = queryRow.at(m_EfforToDateIdx).as_double();
    priorityGroup = queryRow.at(m_PriorityCategoryIdx).as_uint64();
    priority = queryRow.at(m_PriorityIdx).as_uint64();
    personal = queryRow.at(m_PersonalIdx).as_int64();
    lastUpdate = boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_LastUpdateIdx).as_datetime());


    // Optional fields.
    if (!queryRow.at(m_ParentIdx).is_null())
    {
        parentTaskID = queryRow.at(m_ParentIdx).as_uint64();
    }

    if (!queryRow.at(m_StatusIdx).is_null())
    {
        statusVal = static_cast<TaskModel::TaskStatus>(queryRow.at(m_StatusIdx).as_uint64());
    }

    if (!queryRow.at(m_ActualStartIdx).is_null())
    {
        actualStartDate = boostMysqlDateToChronoDate(queryRow.at(m_ActualStartIdx).as_date());
    }

    if (!queryRow.at(m_PlanedEndIdx).is_null())
    {
        estimatedCompletion = boostMysqlDateToChronoDate(queryRow.at(m_PlanedEndIdx).as_date());
    }

    if (!queryRow.at(m_CompletedIdx).is_null())
    {
        completionDate = boostMysqlDateToChronoDate(queryRow.at(m_CompletedIdx).as_date());
    }

    dependencyCount = queryRow.at(m_DependencyCountIdx).as_uint64();
    if (dependencyCount > 0)
    {
        dependenciesText = queryRow.at(m_DepenedencyListIdx).as_string();
    }

    return std::make_shared<TaskModel>(taskId, creatorID, assignToID, description, statusVal, parentTaskID, dueDate, scheduledStart,
        actualStartDate, estimatedCompletion, completionDate, estimatedEffort,actualEffortToDate, priorityGroup, priority, personal,
        dependencyCount, dependenciesText, creationTimeStamp, lastUpdate);

}

void TaskQueryProcessor::fillRequiredIndexes()
{
    assignValueToIndex("TaskID", m_TaskIdIdx);
    assignValueToIndex("CreatedBy", m_CreatedByIdx);
    assignValueToIndex("AsignedTo", m_AssignedToIdx);
    assignValueToIndex("Description", m_DescriptionIdx);
    assignValueToIndex("ParentTask", m_ParentIdx);
    assignValueToIndex("Status", m_StatusIdx);
    assignValueToIndex("CreatedOn", m_CreatedOnIdx);
    assignValueToIndex("RequiredDelivery", m_DueDateIdx);
    assignValueToIndex("ScheduledStart", m_PlanedStartIdx);
    assignValueToIndex("ActualStart", m_ActualStartIdx);
    assignValueToIndex("EstimatedCompletion", m_PlanedEndIdx);
    assignValueToIndex("Completed", m_CompletedIdx);
    assignValueToIndex("EstimatedEffortHours", m_EstEffortHoursIdx);
    assignValueToIndex("ActualEffortHours", m_EfforToDateIdx);
    assignValueToIndex("SchedulePriorityGroup", m_PriorityCategoryIdx);
    assignValueToIndex("PriorityInGroup", m_PriorityIdx);
    assignValueToIndex("Personal", m_PersonalIdx);
    assignValueToIndex("DependencyCount", m_DependencyCountIdx);
    assignValueToIndex("Dependencies", m_DepenedencyListIdx);
    assignValueToIndex("LastUpdateTS", m_LastUpdateIdx);
    assignValueToIndex("Hidden", m_HiddenIdx);
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

    return testExceptionAndSuccessNArgs("TaskQueryProcessor::getTaskByTaskID", std::bind(&TaskQueryProcessor::getTaskByTaskID, this, std::placeholders::_1), 0);
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

