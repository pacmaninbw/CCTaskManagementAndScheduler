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

/*
 * The database connection information will not change during the entire
 * execution of the program.
 *
 * To limit the includes for Boost/mysql.hpp and Boost/asio.hpp to this file
 * all co-routines are static functions. The boost mysql and asio header files
 * are very large and incur major build times. This file takes over 14 seconds
 * to compile.
 */

static boost::mysql::connect_params dbConnectionParameters;
static std::size_t lastInsertValue = 0;

DBInterface::DBInterface()
:   errorMessages{""}
{
    dbConnectionParameters.server_address.emplace_host_and_port(HostURL, MySQLPort);
    dbConnectionParameters.username = MySQLAdminUser;
    dbConnectionParameters.password = MySQLAdminPassword;
    dbConnectionParameters.database = PlannerDB;
}

static boost::asio::awaitable<void> coroutine_sqlstatement(std::string sql)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    boost::mysql::results result;

    std::cout << "Attempted SQL statement:\t" << sql << "\n";

    co_await conn.async_execute(sql, result);

    co_await conn.async_close();
}

void DBInterface::asyncExecutionSqlStatment(std::string sqlStmt)
{
    boost::asio::io_context ctx;

    boost::asio::co_spawn(ctx,
        [sqlStmt] { return coroutine_sqlstatement(sqlStmt); },
        [](std::exception_ptr ptr) {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
        }
    );

    ctx.run();
}

static boost::mysql::date convertChronoDateToBoostMySQLDate(std::chrono::year_month_day source)
{
    std::chrono::sys_days tp = source;
    boost::mysql::date boostDate(tp);
    return boostDate;
}

static boost::asio::awaitable<void> coro_insert_task(TaskModel task)
{
    boost::mysql::date createdOn = convertChronoDateToBoostMySQLDate(task.getCreationDate());
    boost::mysql::date dueDate = convertChronoDateToBoostMySQLDate(task.getDueDate());
    boost::mysql::date scheduledStart = convertChronoDateToBoostMySQLDate(task.getScheduledStart());
    std::optional<std::size_t> parentTaskID;
    std::optional<unsigned int> status;
    std::optional<boost::mysql::date> actualStart;
    std::optional<boost::mysql::date> estimatedCompleteDate;
    std::optional<boost::mysql::date> completeDate;

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
        actualStart = convertChronoDateToBoostMySQLDate(task.getEstimatedCompletion());
    }

    if (task.hasOptionalFieldCompletionDate())
    {
        actualStart = convertChronoDateToBoostMySQLDate(task.getCompletionDate());
    }

    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    std::string sqlStatement = boost::mysql::format_sql(
    conn.format_opts().value(),
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

    boost::mysql::results result;
    co_await conn.async_execute(sqlStatement, result);

    std::cout << "Successfully created task with ID: " << result.last_insert_id() << std::endl;

    co_await conn.async_close();
}

bool DBInterface::insertIntoDataBase(TaskModel& task)
{
    clearPreviousErrors();

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
        [=] { return coro_insert_task(task); },
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
static boost::asio::awaitable<void> coro_insert_user(UserModel user)
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

    std::cout << "Successfully created task with ID: " << result.last_insert_id() << std::endl;
    lastInsertValue = result.last_insert_id();

    co_await conn.async_close();
}

bool DBInterface::insertIntoDataBase(UserModel &user)
{
    clearPreviousErrors();
    
    lastInsertValue = 0;

    if (user.isInDataBase())
    {
        appendErrorMessage("The task is already in the database.\n");
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
        [=] { return coro_insert_user(user); },
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

    user.setPrimaryKey(lastInsertValue);

    return true;
}
