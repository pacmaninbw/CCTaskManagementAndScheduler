#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "DBInterface.h"
#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#ifdef USINGPRIVATECONNECTIONDATA
    #include "./PrivateData/dbadmindata.h"
#else
    #include "dbadmindata.h"
#endif

DBInterface::DBInterface()
:   errorMessages{""}
{
    dbConnectionParameters.server_address.emplace_host_and_port(HostURL, MySQLPort);
    dbConnectionParameters.username = MySQLAdminUser;
    dbConnectionParameters.password = MySQLAdminPassword;
    dbConnectionParameters.database = PlannerDB;
}

bool DBInterface::insertIntoDataBase(TaskModel& task)
{
    clearPreviousErrors();
    if (!dbFormatOptionsAreSet)
    {
        if (!firstFormattedSqlStatement())
        {
            return false;
        }
    }

    if (task.isInDataBase())
    {
        appendErrorMessage("The task is already in the database.\n");
        return false;
    }
    if (!task.allRequiredFieldsHaveData())
    {
        appendErrorMessage(task.reportMissingRequiredFields());
        return false;
    }

    boost::asio::io_context ctx;

    // Launch our coroutine
    boost::asio::co_spawn(
        ctx,
        [&task, this] { return coro_insert_task(task); },
        // If any exception is thrown in the coroutine body, rethrow it.
        [](std::exception_ptr ptr) {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
        }
    );

    try
    {
        ctx.run();
    }
    catch (const std::exception& e)
    {
        std::string eMsg("MySQL Server Error: ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return false;
    }

    return true;
}

bool DBInterface::insertIntoDataBase(UserModel &user)
{
    clearPreviousErrors();
    if (!dbFormatOptionsAreSet)
    {
        if (!firstFormattedSqlStatement())
        {
            return false;
        }
    }
    
    if (user.isInDataBase())
    {
        appendErrorMessage("The user is already in the database.\n");
        return false;
    }
    if (!user.allRequiredFieldsHaveData())
    {
        appendErrorMessage(user.reportMissingRequiredFields());
        return false;
    }

    boost::asio::io_context ctx;

    // Launch our coroutine
    boost::asio::co_spawn(
        ctx,
        [&user, this] { return coro_insert_user(user); },
        // If any exception is thrown in the coroutine body, rethrow it.
        [](std::exception_ptr ptr) {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
        }
    );

    try
    {
        ctx.run();
    }
    catch (const std::exception& e)
    {
        std::string eMsg("MySQL Server Error: ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return false;
    }

    return true;
}

boost::mysql::date DBInterface::convertChronoDateToBoostMySQLDate(std::chrono::year_month_day source)
{
{
    std::chrono::sys_days tp = source;
    boost::mysql::date boostDate(tp);
    return boostDate;
}}

void DBInterface::getOptionalTaskFields(
    TaskModel &task,
    std::optional<std::size_t> &parentTaskID,
    std::optional<unsigned int> &status,
    std::optional<boost::mysql::date> &actualStart,
    std::optional<boost::mysql::date> &estimatedCompleteDate,
    std::optional<boost::mysql::date> &completeDate
)
{
    if (task.hasOptionalFieldStatus())
    {
        parentTaskID = task.getParentTaskID();
    }

    if (task.hasOptionalFieldStatus())
    {
        status = static_cast<unsigned int>(task.getStatus());
    }

    if (task.hasOptionalFieldActualStartDate())
    {
        actualStart = convertChronoDateToBoostMySQLDate(task.getactualStartDate());
    }

    if (task.hasOptionalFieldEstimatedCompletion())
    {
        estimatedCompleteDate = convertChronoDateToBoostMySQLDate(task.getEstimatedCompletion());
    }

    if (task.hasOptionalFieldCompletionDate())
    {
        completeDate = convertChronoDateToBoostMySQLDate(task.getCompletionDate());
    }
}

boost::asio::awaitable<void> DBInterface::coro_insert_task(TaskModel &task)
{
    std::string sqlStatement = formatInsertTask(task);
    std::cout << "Attempting to execute formatted statement " << sqlStatement << "\n";

    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    boost::mysql::results result;
    co_await conn.async_execute(sqlStatement, result);

    task.setPrimaryKey(result.last_insert_id());

    co_await conn.async_close();
}

boost::asio::awaitable<void> DBInterface::coro_insert_user(UserModel &user)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    boost::mysql::results result;
    co_await conn.async_execute(
        boost::mysql::with_params(
            "INSERT INTO PlannerTaskScheduleDB.UserProfile ("
            "LastName, FirstName, MiddleInitial, EmailAddress, LoginName, HashedPassWord, ScheduleDayStart, ScheduleDayEnd"
            ") VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7})",
            user.getLastName(),
            user.getFirstName(),
            user.getMiddleInitial(),
            user.getEmail(),
            user.getLoginName(),
            user.getPassword(),
            user.getStartTime(),
            user.getEndTime()
        ),
        result
    );

    user.setPrimaryKey(result.last_insert_id());

    co_await conn.async_close();
}

std::string DBInterface::formatInsertTask(TaskModel &task)
{
    boost::mysql::date createdOn = convertChronoDateToBoostMySQLDate(task.getCreationDate());
    boost::mysql::date dueDate = convertChronoDateToBoostMySQLDate(task.getDueDate());
    boost::mysql::date scheduledStart = convertChronoDateToBoostMySQLDate(task.getScheduledStart());
    std::optional<std::size_t> parentTaskID;
    std::optional<unsigned int> status;
    std::optional<boost::mysql::date> actualStart;
    std::optional<boost::mysql::date> estimatedCompleteDate;
    std::optional<boost::mysql::date> completeDate;

    getOptionalTaskFields(task, parentTaskID, status, actualStart, estimatedCompleteDate, completeDate);

    std::string sqlStatement = boost::mysql::format_sql(
    dbFormatOptions,
    R"sql(INSERT INTO PlannerTaskScheduleDB.Tasks (
        CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn, RequiredDelivery, ScheduledStart, 
        ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, ActualEffortHours, SchedulePriorityGroup, PriorityInGroup
        ) VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}))sql",
        task.getCreatorID(),
        task.getAssignToID(),
        task.getDescription(),
        parentTaskID,
        status,
        task.getPercentageComplete(),
        createdOn,
        dueDate,
        scheduledStart,
        actualStart,
        estimatedCompleteDate,
        completeDate,
        task.getEstimatedEffort(),
        task.getactualEffortToDate(),
        task.getPriorityGoup(),
        task.getPriority()
    );

    return sqlStatement;
}

boost::asio::awaitable<void> DBInterface::getFormatOptionsFromDB()
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    dbFormatOptions = conn.format_opts().value();
    dbFormatOptionsAreSet = true;

    co_await conn.async_close();
}

bool DBInterface::firstFormattedSqlStatement()
{
    boost::asio::io_context ctx;

    // Launch our coroutine
    boost::asio::co_spawn(
        ctx,
        [this] { return getFormatOptionsFromDB(); },
        // If any exception is thrown in the coroutine body, rethrow it.
        [](std::exception_ptr ptr) {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
        }
    );

    try
    {
        ctx.run();
    }
    catch (const std::exception& e)
    {
        std::string eMsg("MySQL Server Error: ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return false;
    }

    return true;
}
