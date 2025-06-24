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
    if (!validateObjectAndSetUp(task))
    {
        return false;
    }

    std::string sqlStatement = formatInsert(task);
    std::size_t taskID = 0;

    if (runAsyncSQLInsertion(sqlStatement, taskID))
    {
        task.setPrimaryKey(taskID);
        return true;
    }

    return false;
}

bool DBInterface::insertIntoDataBase(UserModel &user)
{
    if (!validateObjectAndSetUp(user))
    {
        return false;
    }

    std::string sqlStatement = formatInsert(user);
    std::size_t userID = 0;

    if (runAsyncSQLInsertion(sqlStatement, userID))
    {
        user.setPrimaryKey(userID);
        return true;
    }

    return false;
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

boost::asio::awaitable<std::size_t> DBInterface::genericInsertCoRoutine(std::string& sqlStatement)
{
//    std::cout << "Attempting to execute formatted statement " << sqlStatement << "\n";

    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    boost::mysql::results result;
    co_await conn.async_execute(sqlStatement, result);

    co_await conn.async_close();

    co_return static_cast<std::size_t>(result.last_insert_id());
}

std::string DBInterface::formatInsert(TaskModel &task)
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

std::string DBInterface::formatInsert(UserModel &user)
{
    std::string sqlStatement = boost::mysql::format_sql(
    dbFormatOptions,
    R"sql(INSERT INTO PlannerTaskScheduleDB.UserProfile (
            LastName, FirstName, MiddleInitial, EmailAddress, LoginName, HashedPassWord, ScheduleDayStart, ScheduleDayEnd
            ) VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}))sql",
            user.getLastName(),
            user.getFirstName(),
            user.getMiddleInitial(),
            user.getEmail(),
            user.getLoginName(),
            user.getPassword(),
            user.getStartTime(),
            user.getEndTime()
    );

    return sqlStatement;
}

/*
 * retrieve the format options from the connection. Should only be called
 * once per instantiation of the class, but it can't be called from the constructor.
 */
boost::asio::awaitable<void> DBInterface::getFormatOptionsFromDB()
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    dbFormatOptions = conn.format_opts().value();
    dbFormatOptionsAreSet = true;

    co_await conn.async_close();
}

/*
 * Co-routines can't be called from constructors, so the first formatted
 * statement need to retrieve the connection details. 
 */
bool DBInterface::firstFormattedSqlStatement()
{
    if (dbFormatOptionsAreSet)
    {
        return true;
    }

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

bool DBInterface::validateObjectAndSetUp(ModelBase &model)
{
    clearPreviousErrors();
    if (!dbFormatOptionsAreSet)
    {
        if (!firstFormattedSqlStatement())
        {
            return false;
        }
    }
    
    if (model.isInDataBase())
    {
        appendErrorMessage("The model object is already in the database.\n");
        return false;
    }

    if (!model.allRequiredFieldsHaveData())
    {
        appendErrorMessage(model.reportMissingRequiredFields());
        return false;
    }

    return true;
}

bool DBInterface::runAsyncSQLInsertion(std::string &sqlStatement, std::size_t &newEntryID)
{
    boost::asio::io_context ctx;

    boost::asio::co_spawn(
        ctx,
        genericInsertCoRoutine(sqlStatement),
        [&newEntryID](std::exception_ptr ptr, std::size_t newId)
        {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
            newEntryID = newId;
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

UserModel_shp DBInterface::getUserByLogin(std::string loginName)
{
    if (!firstFormattedSqlStatement())
    {
        return nullptr;
    }

    std::string sqlStatement = boost::mysql::format_sql(
    dbFormatOptions,
    R"sql(SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, HashedPassWord, ScheduleDayStart, ScheduleDayEnd
        IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, UseLettersForMajorPriority, SeparatePriorityWithDot
        FROM PlannerTaskScheduleDB.UserProfile WHERE LoginName = {})sql", loginName);

    try
    {
        return std::make_shared<UserModel>(UserModel(runAsyncUserSqlQuery(sqlStatement)));
    }
    catch(const std::exception& e)
    {
        std::cerr << "In DBInterface::getUserByLogin " << e.what() << '\n';
        return nullptr;
    }
}

boost::asio::awaitable<boost::mysql::results> DBInterface::getUserFromDBCoRoutine(std::string selectSqlStatement)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    std::cout << "Executing " << selectSqlStatement << "\n"; 
    boost::mysql::results result;
    co_await conn.async_execute(selectSqlStatement, result);

    co_await conn.async_close();

    co_return result;
}

UserSqlData DBInterface::runAsyncUserSqlQuery(std::string selectSqlStatement)
{
    boost::asio::io_context ctx;
    UserSqlData localUser ={0, "", "", "", "", "", "", "", "", true, true, true, false};

    boost::asio::co_spawn(
        ctx,
        getUserFromDBCoRoutine(selectSqlStatement),
        [&localUser, this](std::exception_ptr ptr, boost::mysql::results result)
        {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
            if (result.rows().empty())
            {
                std::domain_error noData("User not found!");
                throw noData;
            }
            else
            {
                boost::mysql::row_view foundUser = result.rows().at(0);
                localUser = convertRowViewToUserSqlData(foundUser);
            }
        }
    );

    ctx.run();

    return localUser;
}

UserSqlData DBInterface::convertRowViewToUserSqlData(boost::mysql::row_view& queryRestultData)
{
    UserSqlData localUser = {0, "", "", "", "", "", "", "", "", true, true, true, false};

    for (std::size_t i = 0; i < queryRestultData.size(); ++i)
    {
        switch (i)
        {
            case 0:
                localUser.UserID = queryRestultData.at(i).as_uint64();
                break;
            case 1:
                localUser.LastName = queryRestultData.at(i).as_string();
                break;
            case 2:
                localUser.FirstName = queryRestultData.at(i).as_string();
                break;
            case 3:
                localUser.MiddleInitial = queryRestultData.at(i).as_string();
                break;
            case 4:
                localUser.EmailAddress = queryRestultData.at(i).as_string();
                break;
            case 5: 
                localUser.LoginName = queryRestultData.at(i).as_string();
                break;
            case 6: 
                localUser.HashedPassWord = queryRestultData.at(i).as_string();
                break;
            case 7: 
                localUser.ScheduleDayStart = queryRestultData.at(i).as_string();
                break;
            case 8: 
                localUser.ScheduleDayEnd = queryRestultData.at(i).as_string();
                break;
            case 9: 
                localUser.IncludePriorityInSchedule = static_cast<bool>(queryRestultData.at(i).as_int64());
                break;
            case 10: 
                localUser.IncludeMinorPriorityInSchedule = static_cast<bool>(queryRestultData.at(i).as_int64());
                break;
            case 11: 
                localUser.UseLettersForMajorPriority = static_cast<bool>(queryRestultData.at(i).as_int64());
                break;
            case 12:
                localUser.SeparatePriorityWithDot = static_cast<bool>(queryRestultData.at(i).as_int64());
                break;
            default: 
                std::string ooEMsg("convertRowViewToUserSqlData i out of range for UserSqlData");
                std::out_of_range oor(ooEMsg);
                throw oor;
                break;
        }
    }

    return localUser;
}
