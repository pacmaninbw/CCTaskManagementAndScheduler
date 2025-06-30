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

        std::string sqlStatement = boost::mysql::format_sql(dbFormatOptions,
            R"sql(SELECT * FROM PlannerTaskScheduleDB.UserProfile WHERE LoginName = {})sql", loginName);

        UserModel_shp newUser = std::make_shared<UserModel>(UserModel());
        if (executeSimpleQueryProcessResults(sqlStatement, newUser))
        {
            return newUser;
        }

        return nullptr;
    }
    catch(const std::exception& e)
    {
        std::string eMsg("In DBInterface::getUserByLogin ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return nullptr;
    }
}

UserModel_shp DBInterface::getUserByFullName(std::string lastName, std::string firstName, std::string middleInitial)
{
    try
    {
        if (!getFormatOptionsOnFirstFormatting())
        {
            return nullptr;
        }

        std::string sqlStatement = boost::mysql::format_sql(dbFormatOptions,
        R"sql(SELECT * FROM PlannerTaskScheduleDB.UserProfile WHERE LastName = {} AND FirstName = {} AND MiddleInitial = {} )sql",
        lastName, firstName, middleInitial);

        UserModel_shp newUser = std::make_shared<UserModel>(UserModel());
        if (executeSimpleQueryProcessResults(sqlStatement, newUser))
        {
            return newUser;
        }

        return nullptr;
    }
    catch(const std::exception& e)
    {
        std::string eMsg("In DBInterface::getUserByFullName ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return nullptr;
    }
}

TaskModel_shp DBInterface::getTaskByDescription(std::string description)
{
    try
    {
        if (!getFormatOptionsOnFirstFormatting())
        {
            return nullptr;
        }

        std::string sqlStatement = boost::mysql::format_sql(dbFormatOptions,
        R"sql(SELECT * FROM PlannerTaskScheduleDB.Tasks WHERE Description = {})sql", description);

        TaskModel_shp newTask = std::make_shared<TaskModel>(TaskModel());
        if (executeSimpleQueryProcessResults(sqlStatement, newTask))
        {
            return newTask;
        }

        return nullptr;
    }
    catch(const std::exception& e)
    {
        std::string eMsg("In DBInterface::getTaskByDescription ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return nullptr;
    }
}
/*
 * Protected or private methods.
 */
boost::mysql::date DBInterface::convertChronoDateToBoostMySQLDate(std::chrono::year_month_day source)
{
    std::chrono::sys_days tp = source;
    boost::mysql::date boostDate(tp);
    return boostDate;
}

std::chrono::year_month_day DBInterface::convertBoostMySQLDateToChornoDate(boost::mysql::date source)
{
    const std::chrono::year year{source.year()};
    const std::chrono::month month{source.month()};
    const std::chrono::day day{source.day()};
    std::chrono::year_month_day converted{year, month, day};

    return converted;
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
boost::asio::awaitable<boost::mysql::results> DBInterface::executeSqlStatementsCoRoutine(std::string sqlStatement)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);
/*
 * To properly process the select statements we need the column names in the metadata.
 */
    size_t foundPos = sqlStatement.find("SELECT");
    if (foundPos != std::string::npos) {
        conn.set_meta_mode(boost::mysql::metadata_mode::full);
    }
    else
    {
        conn.set_meta_mode(boost::mysql::metadata_mode::minimal);
    }

// std::cout << "Executing " << sqlStatement << std::endl; 
    boost::mysql::results result;
    co_await conn.async_execute(sqlStatement, result);

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
    if (task.hasOptionalFieldParentTaskID())
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

bool DBInterface::convertResultsToModel(boost::mysql::row_view &sourceFromDB, std::vector<std::string> &columnNames, Modelshp destination)
{
    bool success = true;
    auto sourceField = sourceFromDB.begin();

    for (auto columnName: columnNames)
    {
        std::string conversionError("In DBInterface::convertResultsToMode(): to " + destination->getModelName() + " ");
        PTS_DataField_shp currentFieldPtr = destination->findFieldInDataFields(columnName);
        if (!currentFieldPtr)
        {
            conversionError += " does not contain field: " + columnName;
            appendErrorMessage(conversionError);
            return false;
        }
        if (!sourceField->is_null())
        {
            if (currentFieldPtr->isStringType())
            {
                currentFieldPtr->dbSetValue(sourceField->as_string());
            }
            else
            {
                try
                {
                    convertScalarFieldValue(*sourceField, currentFieldPtr);
                }
                catch(const std::exception& e)
                {
                    conversionError += e.what();
                    appendErrorMessage(conversionError);
                    success = false;
                }
                
            }
        }
        ++sourceField;
    }

    return success;
}

void DBInterface::convertScalarFieldValue(boost::mysql::field_view sourceField, PTS_DataField_shp currentFieldPtr)
{
    switch (currentFieldPtr->getFieldType())
    {
        case PTS_DataField::PTS_DB_FieldType::Boolean :
            currentFieldPtr->dbSetValue(static_cast<bool>(sourceField.as_int64()));
            break;

        case PTS_DataField::PTS_DB_FieldType::Date :
            currentFieldPtr->dbSetValue(convertBoostMySQLDateToChornoDate(sourceField.as_date()));
            break;

        case PTS_DataField::PTS_DB_FieldType::DateTime :
        case PTS_DataField::PTS_DB_FieldType::TimeStamp :
            currentFieldPtr->dbSetValue(sourceField.as_datetime().as_time_point());
            break;

        case PTS_DataField::PTS_DB_FieldType::Int :
            currentFieldPtr->dbSetValue(static_cast<int>(sourceField.as_int64()));
            break;

        case PTS_DataField::PTS_DB_FieldType::Key :
        case PTS_DataField::PTS_DB_FieldType::Size_T :
            currentFieldPtr->dbSetValue(sourceField.as_uint64());
            break;

        case PTS_DataField::PTS_DB_FieldType::UnsignedInt :
            currentFieldPtr->dbSetValue(static_cast<unsigned int>(sourceField.as_uint64()));
            break;

        case PTS_DataField::PTS_DB_FieldType::Double :
            currentFieldPtr->dbSetValue(sourceField.as_double());
            break;

        default:
            std::string typeError("Column " + currentFieldPtr->getColumnName() +
                "Unknown column type " + std::to_string(static_cast<int>(currentFieldPtr->getFieldType())));
            std::domain_error ex(typeError);
            throw ex;
            break;
    }
}

bool DBInterface::executeSimpleQueryProcessResults(std::string sqlStatements, Modelshp destination)
{
    boost::mysql::results results = runAnyMySQLstatementsAsynchronously(sqlStatements);

    if (results.rows().empty())
    {
        std::string eMsg("No results from query, object not found in database!");
        appendErrorMessage(eMsg);
        return false;
    }

    std::vector<std::string> columnNames;
    for (auto metaIter: results.meta())
    {
        columnNames.push_back(metaIter.column_name());
    }

    boost::mysql::row_view sourceFromDB = results.rows().at(0);
    if (convertResultsToModel(sourceFromDB, columnNames, destination))
    {
        return true;
    }

    return false;
}

