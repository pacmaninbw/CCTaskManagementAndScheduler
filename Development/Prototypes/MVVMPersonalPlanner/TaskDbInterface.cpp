#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include "DBInterface.h"
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include "TaskDbInterface.h"
#include "TaskModel.h"
#include "UserDbInterface.h"
#include "UserModel.h"
#include <utility>

TaskDbInterface::TaskDbInterface()
: DBInterface()
{
}

static constexpr unsigned int OneWeek = 7;
static constexpr unsigned int TwoWeeks = 14;

std::size_t TaskDbInterface::insert(TaskModel &task)
{
    std::size_t taskID = 0;
    clearPreviousErrors();

    if (!task.isModified())
    {
        appendErrorMessage("Task not modified!");
        return taskID;
    }

    if (!task.hasRequiredValues())
    {
        appendErrorMessage("Task is missing required values!");
        return taskID;
    }

    try
    {
        NSBA::io_context ctx;
        NSBM::results localResult;

        NSBA::co_spawn(
            ctx, coRoInsertTask(task),
            [&localResult, this](std::exception_ptr ptr, NSBM::results result)
            {
                if (ptr)
                {
                    std::rethrow_exception(ptr);
                }
                localResult = std::move(result);
            }
        );

        ctx.run();

        taskID = localResult.last_insert_id();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskDbInterface::insert : {}", e.what()));
    }

    return taskID;
}

TaskModel_shp TaskDbInterface::getTaskByTaskID(std::size_t taskId)
{
    TaskModel_shp newTask = nullptr;
    clearPreviousErrors();

    try
    {
        NSBM::results localResult = runQueryAsync(
            std::bind(&TaskDbInterface::coRoSelectTaskById, this, std::placeholders::_1), taskId);

        newTask = processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskDbInterface::getTaskByTaskID({}) : {}", taskId, e.what()));
    }
    
    return newTask;
}

TaskModel_shp TaskDbInterface::getTaskByDescriptionAndAssignedUser(std::string_view description, UserModel& assignedUser)
{
    TaskModel_shp newTask = nullptr;
    clearPreviousErrors();

    try
    {
        std::size_t userId = assignedUser.getUserID();
        NSBM::results localResult = runQueryAsync(
            std::bind(&TaskDbInterface::coRoSelectTaskByDescriptionAndAssignedUser, this, std::placeholders::_1, std::placeholders::_2),
            description, userId);

        newTask = processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In askDbInterface::getTaskByDescriptionAndAssignedUser({}) : {}", description, e.what()));
    }
    
    return newTask;
}

TaskModel_shp TaskDbInterface::getParentTask(TaskModel& task)
{
    if (task.rawParentTaskID().has_value())
    {
        return getTaskByTaskID(task.getParentTaskID());
    }

    return nullptr;
}

TaskList TaskDbInterface::getActiveTasksForAssignedUser(UserModel &assignedUser)
{
    clearPreviousErrors();

    std::cerr << std::format("getAllCurrentActiveTasksForAssignedUser({}) NOT Implemented", assignedUser.getUserID()) << "\n";

    return TaskList();
}

TaskList TaskDbInterface::getUnstartedDueForStartForAssignedUser(UserModel &assignedUser)
{
    clearPreviousErrors();
    std::size_t userId = assignedUser.getUserID();
    TaskList unstartedTasks;
    std::chrono::year_month_day searchDate = getTodaysDatePlus(OneWeek);

    try {
        NSBM::results localResults = runQueryAsync(
            std::bind(&TaskDbInterface::coRoSelecUnstartedDueForStartForAssignedUsert, this, std::placeholders::_1, std::placeholders::_2),
            userId, searchDate);
        unstartedTasks = processResults(localResults);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskDbInterface::getUnstartedDueForStartForAssignedUser({}) : {}", userId, e.what()));
    }

    return unstartedTasks;
}

TaskList TaskDbInterface::getTasksCompletedByAssignedAfterDate(UserModel &assignedUser, std::chrono::year_month_day searchStartDate)
{
    clearPreviousErrors();
    std::size_t userId = assignedUser.getUserID();
    TaskList completedTasks;

    return TaskList();

    try {
        std::cerr << 
            std::format("getTasksCompletedByAssignedAfterDate({} on or after {}) NOT Implemented",
                userId, searchStartDate) <<
            "\n";

    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskDbInterface::getTasksCompletedByAssignedAfterDate({}) : {}", userId, e.what()));
    }

    return completedTasks;
}

/*
 * Private methods.
 */
TaskModel_shp TaskDbInterface::processResult(NSBM::results& results)
{
    if (results.rows().empty())
    {
        appendErrorMessage("Task not found!");
        return nullptr;
    }

    if (results.rows().size() > 1)
    {
        appendErrorMessage("Too many tasks found to process!");
        return nullptr;
    }

    TaskModel_shp newTask = std::make_shared<TaskModel>(TaskModel());
    NSBM::row_view rv = results.rows().at(0);

    processResultRow(rv, newTask);

    return newTask;
}

TaskList TaskDbInterface::processResults(NSBM::results& results)
{
    TaskList taskList;

    if (results.rows().empty())
    {
        appendErrorMessage("No Tasks found!");
        return taskList;
    }

    for (auto row: results.rows())
    {
        TaskModel_shp newTask = std::make_shared<TaskModel>(TaskModel());
        processResultRow(row, newTask);
        taskList.push_back(newTask);
    }
    return taskList;
}

/*
 * The indexes below are based on the following select statement, maintain this order
 * for any new select statements, add any new field indexes at the end.
 *       "SELECT TaskID, CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
 *           "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
 *           "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup FROM Tasks WHERE TaskID = {0}",
 */
constexpr std::size_t taskIdIdx = 0;
constexpr std::size_t createdByIdx = 1;
constexpr std::size_t assignedToIdx = 2;
constexpr std::size_t descriptionIdx = 3;
constexpr std::size_t parentTaskIdx = 4;
constexpr std::size_t statusIdx = 5;
constexpr std::size_t percentageCompleteIdx = 6;
constexpr std::size_t createdOnIdx = 7;
constexpr std::size_t requiredDeliveryIdx = 8;
constexpr std::size_t scheduledStartIdx = 9;
constexpr std::size_t actualStartIdx = 10;
constexpr std::size_t estimatedCompletionIdx = 11;
constexpr std::size_t completedIdx = 12;
constexpr std::size_t estimatedEffortHoursIdx = 13;
constexpr std::size_t actualEffortHoursIdx = 14;
constexpr std::size_t schedulePriorityGroupIdx = 15;
constexpr std::size_t priorityInGroupIdx = 16;
// Added fields
constexpr std::size_t personalIdx = 17;
constexpr std::size_t dependencyCountIdx = 18;

void TaskDbInterface::processResultRow(NSBM::row_view rv, TaskModel_shp newTask)
{
    // Required fields.
    newTask->setTaskID(rv.at(taskIdIdx).as_uint64());
    newTask->setCreatorID(rv.at(createdByIdx).as_uint64());
    newTask->setAssignToID(rv.at(assignedToIdx).as_uint64());
    newTask->setDescription(rv.at(descriptionIdx).as_string());
    newTask->setPercentageComplete(rv.at(percentageCompleteIdx).as_double());
    newTask->setCreationDate(convertBoostMySQLDateToChornoDate(rv.at(createdOnIdx).as_date()));
    newTask->setDueDate(convertBoostMySQLDateToChornoDate(rv.at(requiredDeliveryIdx).as_date()));
    newTask->setScheduledStart(convertBoostMySQLDateToChornoDate(rv.at(scheduledStartIdx).as_date()));
    newTask->setEstimatedEffort(rv.at(estimatedEffortHoursIdx).as_uint64());
    newTask->setActualEffortToDate(rv.at(actualEffortHoursIdx).as_double());
    newTask->setPriorityGroup(rv.at(schedulePriorityGroupIdx).as_uint64());
    newTask->setPriority(rv.at(priorityInGroupIdx).as_uint64());
    newTask->setPersonal(rv.at(personalIdx).as_int64());

    // Optional fields.
    if (!rv.at(parentTaskIdx).is_null())
    {
        newTask->setParentTaskID(rv.at(parentTaskIdx).as_uint64());
    }

    if (!rv.at(statusIdx).is_null())
    {
        newTask->setStatus(static_cast<TaskModel::TaskStatus>(rv.at(statusIdx).as_uint64()));
    }

    if (!rv.at(actualStartIdx).is_null())
    {
        newTask->setactualStartDate(convertBoostMySQLDateToChornoDate(rv.at(actualStartIdx).as_date()));
    }

    if (!rv.at(estimatedCompletionIdx).is_null())
    {
        newTask->setEstimatedCompletion(convertBoostMySQLDateToChornoDate(rv.at(estimatedCompletionIdx).as_date()));
    }
    if (!rv.at(completedIdx).is_null())
    {
        newTask->setCompletionDate(convertBoostMySQLDateToChornoDate(rv.at(completedIdx).as_date()));
    }

    std::size_t dependencyCount = rv.at(dependencyCountIdx).as_uint64();
    if (dependencyCount > 0)
    {
        addDependencies(newTask);
    }

    // All the set functions set modified, since this user is new in memory it is not modified.
    newTask->clearModified();
}

NSBA::awaitable<NSBM::results> TaskDbInterface::coRoInsertTask(TaskModel &task)
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results insertResult;
    std::size_t dependencyCount = task.getDependencies().size();

    co_await conn.async_execute(
        NSBM::with_params("INSERT INTO Tasks (CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup, Personal, DependencyCount)"
            " VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17})",
            task.getCreatorID(),
            task.getAssignToID(),
            task.getDescription(),
            task.rawParentTaskID(),
            task.getStatusIntVal(),
            task.getPercentageComplete(),
            convertChronoDateToBoostMySQLDate(task.getCreationDate()),
            convertChronoDateToBoostMySQLDate(task.getDueDate()),
            convertChronoDateToBoostMySQLDate(task.getScheduledStart()),
            optionalDateConversion(task.rawActualStartDate()),
            optionalDateConversion(task.rawEstimatedCompletion()),
            optionalDateConversion(task.rawCompletionDate()),
            task.getEstimatedEffort(),
            task.getactualEffortToDate(),
            task.getPriorityGroup(),
            task.getPriority(),
            task.isPersonal(),
            dependencyCount),
        insertResult
    );

    std::size_t taskID = insertResult.last_insert_id();
    std::vector<std::size_t> dependencies = task.getDependencies();
    if (taskID > 0 &&  dependencies.size() > 0)
    {
        NSBM::statement stmt = co_await conn.async_prepare_statement(
            "INSERT INTO TaskDependencies (TaskID, Dependency) VALUES (?, ?)"
        );
        for (auto dependency: dependencies)
        {
            NSBM::results result;
            co_await conn.async_execute(stmt.bind(taskID, dependency), result);
        }
        co_await conn.async_close_statement(stmt);
    }
    co_await conn.async_close();

    co_return insertResult;
}

std::optional<NSBM::date> TaskDbInterface::optionalDateConversion(std::optional<std::chrono::year_month_day> optDate)
{
    std::optional<NSBM::date> mySqlDate;

    if (optDate.has_value())
    {
        mySqlDate = convertChronoDateToBoostMySQLDate(optDate.value());
    }
    return mySqlDate;
}

NSBA::awaitable<NSBM::results> TaskDbInterface::coRoSelectTaskById(const std::size_t taskId)
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results selectResult;

    co_await conn.async_execute(
        NSBM::with_params("SELECT TaskID, CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup, Personal, DependencyCount FROM Tasks WHERE TaskID = {0}",
            taskId),
        selectResult
    );

    co_await conn.async_close();

    co_return selectResult;
}

NSBA::awaitable<NSBM::results> TaskDbInterface::coRoSelectTaskDependencies(const std::size_t taskId)
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results selectResult;

    co_await conn.async_execute(
        NSBM::with_params("SELECT Dependency FROM TaskDependencies WHERE TaskID = {0} ORDER BY Dependency ASC", taskId), selectResult);

    co_await conn.async_close();

    co_return selectResult;
}

void TaskDbInterface::addDependencies(TaskModel_shp newTask)
{
    std::size_t taskId = newTask->getTaskID();
    NSBM::results localResult = runQueryAsync(
        std::bind(&TaskDbInterface::coRoSelectTaskDependencies, this, std::placeholders::_1), taskId);

    if (!localResult.rows().empty())
    {
        for (auto row: localResult.rows())
        {
            newTask->addDependency(row.at(0).as_uint64());
        }
    }
    else
    {
        std::runtime_error NoExpectedDependencies("Dependencies expected but not found!");
        throw NoExpectedDependencies;
    }
}

NSBA::awaitable<NSBM::results> TaskDbInterface::coRoSelectTaskByDescriptionAndAssignedUser(
    std::string_view description, const std::size_t userID)
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results selectResult;

    co_await conn.async_execute(
        NSBM::with_params("SELECT TaskID, CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup, Personal, DependencyCount FROM Tasks WHERE Description = {0}"
            " AND AsignedTo = {1}", description, userID),
        selectResult
    );

    co_await conn.async_close();

    co_return selectResult;
}

NSBA::awaitable<NSBM::results> TaskDbInterface::coRoSelecUnstartedDueForStartForAssignedUsert(
    std::size_t userID, std::chrono::year_month_day searchStart)
{
    constexpr unsigned int notStarted = static_cast<unsigned int>(TaskModel::TaskStatus::Not_Started);
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results selectResult;

    co_await conn.async_execute(
        NSBM::with_params("SELECT TaskID, CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup, Personal, DependencyCount FROM Tasks WHERE AsignedTo = {0}"
            " AND ScheduledStart < {1} AND (Status IS NULL OR Status = {2})",
            userID, convertChronoDateToBoostMySQLDate(searchStart), notStarted),
        selectResult
    );

    co_await conn.async_close();

    co_return selectResult;
}

NSBA::awaitable<NSBM::results> TaskDbInterface::coRoSelectTasksWithStatusForAssignedUserBefore(
    std::size_t userID, unsigned int status, std::chrono::year_month_day searchStart)
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results selectResult;

    co_await conn.async_execute(
        NSBM::with_params("SELECT TaskID, CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup, Personal, DependencyCount FROM Tasks WHERE AsignedTo = {0}"
            " AND ScheduledStart < {1} AND Status = {2})",
            userID, convertChronoDateToBoostMySQLDate(searchStart), status),
        selectResult
    );

    co_await conn.async_close();

    co_return selectResult;
}
