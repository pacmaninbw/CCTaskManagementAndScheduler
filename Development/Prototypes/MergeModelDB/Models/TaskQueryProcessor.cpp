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
    errorMessages.clear();
    TaskModel_shp found = nullptr;

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, listQueryBase);
        boost::mysql::format_sql_to(fctx, " WHERE TaskID = {}", taskId);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        TaskList shouldHaveOnlyOne = processResults(localResult);

        if (shouldHaveOnlyOne.empty())
        {
            appendErrorMessage(" Task not found!");
            return found;
        }

        if (shouldHaveOnlyOne.size() > 1)
        {
            appendErrorMessage("Too many Tasks found to process!");
            return found;
        }

        found = shouldHaveOnlyOne[0];
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskQueryProcessor::{}({}) : {}", __func__, taskId, e.what()));
    }

    return found;
}

TaskList TaskQueryProcessor::getTaskByDescriptionAndAssignedUser(std::string_view description, std::size_t assignedUserID) noexcept
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, listQueryBase);
        boost::mysql::format_sql_to(fctx, " WHERE Description = {} AND AsignedTo = {}", description, assignedUserID);
        boost::mysql::format_sql_to(fctx, " AND (Hidden IS NULL OR Hidden <> 1)");
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
    errorMessages.clear();

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatSelectActiveTasksForAssignedUser(assignedUserID));
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
    errorMessages.clear();

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatSelectUnstartedDueForStartForAssignedUser(assignedUserID));
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
    errorMessages.clear();

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatSelectTasksCompletedByAssignedAfterDate(
            assignedUserID, searchStartDate));
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
    errorMessages.clear();

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatSelectTasksByAssignedIDandParentID(
            assignedUserID, parentID));
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
    errorMessages.clear();

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatDefaultTaskTableSelect(assignedUserID, searchStartDate));
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
    std::size_t taskId = queryRow.at(taskIdIdx).as_uint64();
    std::size_t creatorID = queryRow.at(createdByIdx).as_uint64();
    std::size_t assignToID = queryRow.at(assignedToIdx).as_uint64();
    std::string description = queryRow.at(descriptionIdx).as_string();
    creationTimeStamp = boostMysqlDateTimeToChronoTimePoint(queryRow.at(createdOnIdx).as_datetime());
    dueDate = boostMysqlDateToChronoDate(queryRow.at(requiredDeliveryIdx).as_date());
    scheduledStart = boostMysqlDateToChronoDate(queryRow.at(scheduledStartIdx).as_date());
    estimatedEffort = queryRow.at(estimatedEffortHoursIdx).as_uint64();
    actualEffortToDate = queryRow.at(actualEffortHoursIdx).as_double();
    priorityGroup = queryRow.at(schedulePriorityGroupIdx).as_uint64();
    priority = queryRow.at(priorityInGroupIdx).as_uint64();
    personal = queryRow.at(personalIdx).as_int64();
    lastUpdate = boostMysqlDateTimeToChronoTimePoint(queryRow.at(lastUpdate_Idx).as_datetime());


    // Optional fields.
    if (!queryRow.at(parentTaskIdx).is_null())
    {
        parentTaskID = queryRow.at(parentTaskIdx).as_uint64();
    }

    if (!queryRow.at(statusIdx).is_null())
    {
        statusVal = static_cast<TaskModel::TaskStatus>(queryRow.at(statusIdx).as_uint64());
    }

    if (!queryRow.at(actualStartIdx).is_null())
    {
        actualStartDate = boostMysqlDateToChronoDate(queryRow.at(actualStartIdx).as_date());
    }

    if (!queryRow.at(estimatedCompletionIdx).is_null())
    {
        estimatedCompletion = boostMysqlDateToChronoDate(queryRow.at(estimatedCompletionIdx).as_date());
    }

    if (!queryRow.at(completedIdx).is_null())
    {
        completionDate = boostMysqlDateToChronoDate(queryRow.at(completedIdx).as_date());
    }

    dependencyCount = queryRow.at(dependencyCountIdx).as_uint64();
    if (dependencyCount > 0)
    {
        dependenciesText = queryRow.at(depenedenciesTextIdx).as_string();
    }

    return std::make_shared<TaskModel>(taskId, creatorID, assignToID, description, statusVal, parentTaskID, dueDate, scheduledStart,
        actualStartDate, estimatedCompletion, completionDate, estimatedEffort,actualEffortToDate, priorityGroup, priority, personal,
        dependencyCount, dependenciesText, creationTimeStamp, lastUpdate);

}

void TaskQueryProcessor::fillRequiredIndexes()
{
    assignValueToIndex("TaskID", taskIdIdx);
    assignValueToIndex("CreatedBy", createdByIdx);
    assignValueToIndex("AsignedTo", assignedToIdx);
    assignValueToIndex("Description", descriptionIdx);
    assignValueToIndex("ParentTask", parentTaskIdx);
    assignValueToIndex("Status", statusIdx);
    assignValueToIndex("CreatedOn", createdOnIdx);
    assignValueToIndex("RequiredDelivery", requiredDeliveryIdx);
    assignValueToIndex("ScheduledStart", scheduledStartIdx);
    assignValueToIndex("ActualStart", actualStartIdx);
    assignValueToIndex("EstimatedCompletion", estimatedCompletionIdx);
    assignValueToIndex("Completed", completedIdx);
    assignValueToIndex("EstimatedEffortHours", estimatedEffortHoursIdx);
    assignValueToIndex("ActualEffortHours", actualEffortHoursIdx);
    assignValueToIndex("SchedulePriorityGroup", schedulePriorityGroupIdx);
    assignValueToIndex("PriorityInGroup", priorityInGroupIdx);
    assignValueToIndex("Personal", personalIdx);
    assignValueToIndex("DependencyCount", dependencyCountIdx);
    assignValueToIndex("Dependencies", depenedenciesTextIdx);
    assignValueToIndex("LastUpdateTS", lastUpdate_Idx);
    assignValueToIndex("Hidden", hidden_Idx);
}

std::string TaskQueryProcessor::formatSelectActiveTasksForAssignedUser(std::size_t assignedUserID)
{
    constexpr unsigned int notStarted = static_cast<unsigned int>(TaskModel::TaskStatus::Not_Started);

    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, listQueryBase);
    boost::mysql::format_sql_to(fctx, " WHERE AsignedTo = {} AND Completed IS NULL AND (Status IS NOT NULL AND Status <> {})",
        assignedUserID, notStarted);
    boost::mysql::format_sql_to(fctx, " AND (Hidden IS NULL OR Hidden <> 1)");

    return std::move(fctx).get().value();
}

std::string TaskQueryProcessor::formatSelectUnstartedDueForStartForAssignedUser(std::size_t assignedUserID)
{
    constexpr unsigned int notStarted = static_cast<unsigned int>(TaskModel::TaskStatus::Not_Started);

    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, listQueryBase);
    boost::mysql::format_sql_to(fctx, " WHERE AsignedTo = {} AND ScheduledStart < {} AND (Status IS NULL OR Status = {})",
        assignedUserID, stdchronoDateToBoostMySQLDate(getTodaysDatePlus(OneWeek)), notStarted);
    boost::mysql::format_sql_to(fctx, " AND (Hidden IS NULL OR Hidden <> 1)");

    return std::move(fctx).get().value();
}

std::string TaskQueryProcessor::formatSelectTasksCompletedByAssignedAfterDate(std::size_t assignedUserID, std::chrono::year_month_day searchStartDate)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, listQueryBase);
    boost::mysql::format_sql_to(fctx, " WHERE AsignedTo = {} AND Completed >= {}",
        assignedUserID, stdchronoDateToBoostMySQLDate(searchStartDate));

    return std::move(fctx).get().value();
}

std::string TaskQueryProcessor::formatSelectTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, listQueryBase);
    boost::mysql::format_sql_to(fctx, " WHERE AsignedTo = {} AND ParentTask = {}", assignedUserID, parentID);
    boost::mysql::format_sql_to(fctx, " AND (Hidden IS NULL OR Hidden <> 1)");

    return std::move(fctx).get().value();
}

std::string TaskQueryProcessor::formatDefaultTaskTableSelect(std::size_t assignedUserID, std::chrono::year_month_day searchStartDate)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, listQueryBase);
    boost::mysql::format_sql_to(fctx, " WHERE AsignedTo = {}", assignedUserID);
    boost::mysql::format_sql_to(fctx, " AND RequiredDelivery < {} AND Completed IS NULL",
        stdchronoDateToBoostMySQLDate(searchStartDate));
    boost::mysql::format_sql_to(fctx, " AND (Hidden IS NULL OR Hidden <> 1)");
    boost::mysql::format_sql_to(fctx, " ORDER BY SchedulePriorityGroup ASC, PriorityInGroup ASC");

    return std::move(fctx).get().value();
}

std::vector<ListExceptionTestElement> TaskQueryProcessor::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
#if 0
    exceptionTests.push_back({std::bind(&TaskQueryProcessor::testExceptionGetByTaskID, this),
        "getTaskByTaskID"});
#endif
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

#if 0
TestStatus TaskQueryProcessor::testExceptionGetByTaskID() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::getTaskByTaskID", std::bind(&TaskQueryProcessor::getTaskByTaskID, this, std::placeholders::_1), 0);
}
#endif

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

    return testListExceptionAndSuccessNArgs("TaskQueryProcessor::testExceptionGetUnstartedDueForStartForAssignedUser()",
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

