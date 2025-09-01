#include <algorithm>
#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include "BoostDBInterfaceCore.h"
#include <exception>
#include <format>
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
: BoostDBInterfaceCore()
{
}

static constexpr unsigned int OneWeek = 7;
static constexpr unsigned int TwoWeeks = 14;

std::size_t TaskDbInterface::insert(TaskModel &task)
{
    std::size_t taskID = 0;
    prepareForRunQueryAsync();

    if (task.isInDatabase())
    {
        appendErrorMessage("Task already in Database, use Update!");
        return taskID;
    }

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
        NSBM::results localResult = runQueryAsync(formatInsertTask(task));

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
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE TaskID = {}", taskId);

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        newTask = processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskDbInterface::getTaskByTaskID({}) : {}", taskId, e.what()));
    }

    return newTask;
}

TaskModel_shp TaskDbInterface::getTaskByDescriptionAndAssignedUser(std::string_view description, const UserModel& assignedUser)
{
    TaskModel_shp newTask = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE Description = {} AND AsignedTo = {}", description, assignedUser.getUserID());

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        newTask = processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskDbInterface::getTaskByDescriptionAndAssignedUser({}) : {}", description, e.what()));
    }

    return newTask;
}

TaskModel_shp TaskDbInterface::getParentTask(const TaskModel& task)
{
    if (task.rawParentTaskID().has_value())
    {
        return getTaskByTaskID(task.getParentTaskID());
    }

    return nullptr;
}

TaskList TaskDbInterface::getActiveTasksForAssignedUser(const UserModel &assignedUser)
{
    prepareForRunQueryAsync();

    std::cerr << std::format("getAllCurrentActiveTasksForAssignedUser({}) NOT Implemented", assignedUser.getUserID()) << "\n";

    return TaskList();
}

TaskList TaskDbInterface::getUnstartedDueForStartForAssignedUser(const UserModel &assignedUser)
{
    prepareForRunQueryAsync();

    TaskList unstartedTasks;

    try {
        constexpr unsigned int notStarted = static_cast<unsigned int>(TaskModel::TaskStatus::Not_Started);

        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE AsignedTo = {} AND ScheduledStart < {} AND (Status IS NULL OR Status = {})",
            assignedUser.getUserID(), stdchronoDateToBoostMySQLDate(getTodaysDatePlus(OneWeek)), notStarted);

        NSBM::results localResults = runQueryAsync(std::move(fctx).get().value());

        unstartedTasks = processResults(localResults);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskDbInterface::getUnstartedDueForStartForAssignedUser({}) : {}", assignedUser.getUserID(), e.what()));
    }

    return unstartedTasks;
}

TaskList TaskDbInterface::getTasksCompletedByAssignedAfterDate(const UserModel& assignedUser, std::chrono::year_month_day& searchStartDate)
{
    prepareForRunQueryAsync();
    std::size_t userId = assignedUser.getUserID();
    TaskList completedTasks;

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

bool TaskDbInterface::update(TaskModel &task)
{
    prepareForRunQueryAsync();

    if (!task.isInDatabase())
    {
        appendErrorMessage("Task has no primary key, Use INSERT!");
        return false;
    }
 
    if (!task.isModified())
    {
        appendErrorMessage("Task not modified!");
        return false;
    }

    try
    {
        NSBM::results localResult = runQueryAsync(formatUpdateTask(task));

        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskDbInterface::update : {}", e.what()));
        return false;
    }
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

void TaskDbInterface::processResultRow(NSBM::row_view rv, TaskModel_shp newTask)
{
    // Required fields.
    newTask->setTaskID(rv.at(taskIdIdx).as_uint64());
    newTask->setCreatorID(rv.at(createdByIdx).as_uint64());
    newTask->setAssignToID(rv.at(assignedToIdx).as_uint64());
    newTask->setDescription(rv.at(descriptionIdx).as_string());
    newTask->setPercentageComplete(rv.at(percentageCompleteIdx).as_double());
    newTask->setCreationDate(boostMysqlDateTimeToChronoTimePoint(rv.at(createdOnIdx).as_date()));
    newTask->setDueDate(boostMysqlDateTimeToChronoTimePoint(rv.at(requiredDeliveryIdx).as_date()));
    newTask->setScheduledStart(boostMysqlDateTimeToChronoTimePoint(rv.at(scheduledStartIdx).as_date()));
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
        newTask->setactualStartDate(boostMysqlDateTimeToChronoTimePoint(rv.at(actualStartIdx).as_date()));
    }

    if (!rv.at(estimatedCompletionIdx).is_null())
    {
        newTask->setEstimatedCompletion(boostMysqlDateTimeToChronoTimePoint(rv.at(estimatedCompletionIdx).as_date()));
    }

    if (!rv.at(completedIdx).is_null())
    {
        newTask->setCompletionDate(boostMysqlDateTimeToChronoTimePoint(rv.at(completedIdx).as_date()));
    }

    std::size_t dependencyCount = rv.at(dependencyCountIdx).as_uint64();
    if (dependencyCount > 0)
    {
        std::string dependenciesText = rv.at(depenedenciesTextIdx).as_string();
        addDependencies(dependenciesText, newTask);
    }

    // All the set functions set modified, since this user is new in memory it is not modified.
    newTask->clearModified();
}

std::string TaskDbInterface::formatInsertTask(TaskModel &task)
{
    std::size_t dependencyCount = task.getDependencies().size();
    std::optional<std::string> depenenciesText;
    if (dependencyCount)
    {
        std::vector<std::size_t> dependencyList = task.getDependencies();
        depenenciesText = buildDependenciesText(dependencyList);
    }

    return NSBM::format_sql(format_opts.value(),
        "INSERT INTO Tasks (CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup, Personal, DependencyCount, Dependencies)"
            " VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17}, {18})",
            task.getCreatorID(),
            task.getAssignToID(),
            task.getDescription(),
            task.rawParentTaskID(),
            task.getStatusIntVal(),
            task.getPercentageComplete(),
            stdchronoDateToBoostMySQLDate(task.getCreationDate()),
            stdchronoDateToBoostMySQLDate(task.getDueDate()),
            stdchronoDateToBoostMySQLDate(task.getScheduledStart()),
            optionalDateConversion(task.rawActualStartDate()),
            optionalDateConversion(task.rawEstimatedCompletion()),
            optionalDateConversion(task.rawCompletionDate()),
            task.getEstimatedEffort(),
            task.getactualEffortToDate(),
            task.getPriorityGroup(),
            task.getPriority(),
            task.isPersonal(),
            dependencyCount,
            depenenciesText
    );
}

std::string TaskDbInterface::formatUpdateTask(TaskModel &task)
{

    std::size_t dependencyCount = task.getDependencies().size();
    std::optional<std::string> depenenciesText;
    if (dependencyCount)
    {
        std::vector<std::size_t> dependencyList = task.getDependencies();
        depenenciesText = buildDependenciesText(dependencyList);
    }

    return NSBM::format_sql(format_opts.value(),
        "UPDATE Tasks SET"
            " CreatedBy = {0},"
            " AsignedTo = {1},"
            " Description = {2},"
            " ParentTask = {3},"
            " Status = {4},"
            " PercentageComplete = {5},"
            " CreatedOn = {6},"
            " RequiredDelivery = {7},"
            " ScheduledStart = {8},"
            " ActualStart = {9},"
            " EstimatedCompletion = {10},"
            " Completed = {11},"
            " EstimatedEffortHours = {12},"
            " ActualEffortHours = {13},"
            " SchedulePriorityGroup = {14},"
            " PriorityInGroup = {15},"
            " Personal = {16},"
            " DependencyCount = {17},"
            "Dependencies = {18}"
        " WHERE TaskID = {19} ",
        task.getCreatorID(),
        task.getAssignToID(),
        task.getDescription(),
        task.rawParentTaskID(),
        task.getStatusIntVal(),
        task.getPercentageComplete(),
        stdchronoDateToBoostMySQLDate(task.getCreationDate()),
        stdchronoDateToBoostMySQLDate(task.getDueDate()),
        stdchronoDateToBoostMySQLDate(task.getScheduledStart()),
        optionalDateConversion(task.rawActualStartDate()),
        optionalDateConversion(task.rawEstimatedCompletion()),
        optionalDateConversion(task.rawCompletionDate()),
        task.getEstimatedEffort(),
        task.getactualEffortToDate(),
        task.getPriorityGroup(),
        task.getPriority(),
        task.isPersonal(),
        dependencyCount,
        depenenciesText,
        task.getTaskID()
    );

    return std::string();
}

void TaskDbInterface::addDependencies(const std::string& dependenciesText, TaskModel_shp newTask)
{
    std::vector<std::string> dependencyStrings = explodeTextField(dependenciesText);

    if (!dependencyStrings.empty())
    {
        for (auto dependencyStr: dependencyStrings)
        {
            newTask->addDependency(static_cast<std::size_t>(std::stol(dependencyStr)));
        }
    }
    else
    {
        std::runtime_error NoExpectedDependencies("Dependencies expected but not found!");
        throw NoExpectedDependencies;
    }
}

std::string TaskDbInterface::buildDependenciesText(std::vector<std::size_t>& dependencyList) noexcept
{
    if (dependencyList.size() > 1)
    {
        std::sort(dependencyList.begin(), dependencyList.end());
    }

    std::vector<std::string> dependencyStrings;
    for (auto dependency: dependencyList)
    {
        dependencyStrings.push_back(std::to_string(dependency));
    }

    return implodeTextField(dependencyStrings);
}

