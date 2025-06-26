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
    try
    {
        if (!validateObjectAndSetUp(task))
        {
            return false;
        }

        boost::mysql::results results = runAnyMySQLstatementsAsynchronously(formatInsert(task));
        task.setPrimaryKey(results.last_insert_id());

        return true;
    }
    catch(const std::exception& e)
    {
        std::string eMsg("In DBInterface::insertIntoDataBase(TaskModel &task) ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return false;
    }
}

bool DBInterface::insertIntoDataBase(UserModel &user)
{
    try
    {
        if (!validateObjectAndSetUp(user))
        {
            return false;
        }

        boost::mysql::results results = runAnyMySQLstatementsAsynchronously(formatInsert(user));
        user.setPrimaryKey(results.last_insert_id());

        return true;
    }
    catch(const std::exception& e)
    {
        std::string eMsg("In DBInterface::insertIntoDataBase(UserModel &user) ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return false;
    }
}

UserModel_shp DBInterface::getUserByLogin(std::string loginName)
{
    try
    {
        if (!getFormatOptionsOnFirstFormatting())
        {
            return nullptr;
        }

        std::string sqlStatement = boost::mysql::format_sql(
        dbFormatOptions,
        R"sql(SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, HashedPassWord, ScheduleDayStart, ScheduleDayEnd,
            IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, UseLettersForMajorPriority, SeparatePriorityWithDot
            FROM PlannerTaskScheduleDB.UserProfile WHERE LoginName = {})sql", loginName);

        boost::mysql::results results = runAnyMySQLstatementsAsynchronously(sqlStatement);
        return std::make_shared<UserModel>(UserModel(convertResultsToUserSqlData(results)));
    }
    catch(const std::exception& e)
    {
        std::string eMsg("In DBInterface::getUserByLogin ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return nullptr;
    }
}

boost::mysql::date DBInterface::convertChronoDateToBoostMySQLDate(std::chrono::year_month_day source)
{
    std::chrono::sys_days tp = source;
    boost::mysql::date boostDate(tp);
    return boostDate;
}

/*
 * retrieve the format options from the connection. Should only be called
 * once per instantiation of the class, but it can't be called from the constructor.
 * No SQL is executed.
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
 * All boost::mysql executions return results. The results are processed at a higher
 * level as necessary where the knowlege about the results exists. This method only
 * executes the SQL statement(s) and returns all results.
 */
boost::asio::awaitable<boost::mysql::results> DBInterface::executeSqlStatementsCoRoutine(std::string selectSqlStatement)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

//    std::cout << "Executing " << selectSqlStatement << "\n"; 
    boost::mysql::results result;
    co_await conn.async_execute(selectSqlStatement, result);

    co_await conn.async_close();

    co_return result;
}

void DBInterface::getOptionalTaskFields(
    TaskModel &task,
    std::optional<std::size_t> &parentTaskID,
    std::optional<unsigned int> &status,
    std::optional<boost::mysql::date> &actualStart,
    std::optional<boost::mysql::date> &estimatedCompleteDate,
    std::optional<boost::mysql::date> &completeDate
) {
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
 * Co-routines can't be called from constructors, so the first formatted
 * statement need to retrieve the connection details. 
 */
bool DBInterface::getFormatOptionsOnFirstFormatting()
{
    if (dbFormatOptionsAreSet)
    {
        return true;
    }

    boost::asio::io_context ctx;

    boost::asio::co_spawn(
        ctx,
        [this] { return getFormatOptionsFromDB(); },
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
        return true;
    }
/*
 * Handle any errors here since this function is called by most methods that send
 * or receive data from the database.
 */
    catch (const std::exception& e)
    {
        std::string eMsg("Failed to connect to database to get formatting options. MySQL Server Error: ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return false;
    }
}

bool DBInterface::validateObjectAndSetUp(ModelBase &model)
{
    clearPreviousErrors();
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

    if (!dbFormatOptionsAreSet)
    {
        if (!getFormatOptionsOnFirstFormatting())
        {
            return false;
        }
    }
    
    return true;
}

boost::mysql::results DBInterface::runAnyMySQLstatementsAsynchronously(std::string selectSqlStatement)
{
    boost::asio::io_context ctx;
    boost::mysql::results localResult;

    boost::asio::co_spawn(
        ctx, executeSqlStatementsCoRoutine(selectSqlStatement),
        [&localResult, this](std::exception_ptr ptr, boost::mysql::results result)
        {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
            localResult = std::move(result);
        }
    );

    ctx.run();

    return localResult;
}

UserSqlData DBInterface::convertResultsToUserSqlData(boost::mysql::results& results)
{
    UserSqlData localUser = {0, "", "", "", "", "", "", "", "", true, true, true, false};
    if (results.rows().empty())
    {
        std::domain_error noData("User not found!");
        throw noData;
    }

    boost::mysql::row_view queryRestultData = results.rows().at(0);

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
