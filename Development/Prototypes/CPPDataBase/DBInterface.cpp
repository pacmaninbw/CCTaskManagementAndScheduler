#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "DBInterface.h"
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include "TaskModel.h"
#include "UserModel.h"
#include <utility>
#include <vector>


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

bool DBInterface::insertIntoDataBase(ModelBase& model)
{
    try
    {
        if (!validateObjectAndSetUp(model))
        {
            return false;
        }

        boost::mysql::results results = runAnyMySQLstatementsAsynchronously(formatInsert(model));
        model.setPrimaryKey(results.last_insert_id());
        model.onInsertionClearDirtyBits();

        return true;
    }
    catch(const std::exception& e)
    {
        std::string eMsg("In DBInterface::insertIntoDataBase(ModelBase &model) ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return false;
    }
}

bool DBInterface::getUniqueModelFromDB(ModelShp model, std::vector<WhereArg> whereArgs)
{
    if (!model)
    {
        std::invalid_argument missingModelPtr("NULL model pointer in DBInterface::getUniqueModelFromDB()!");
        throw missingModelPtr;
    }

    try
    {
        clearPreviousErrors();

        std::string sqlStatement = formatSelect(getTableNameFrom(*model), whereArgs);
        if (executeSimpleQueryProcessResults(sqlStatement, model))
        {
            return true;
        }
    }
    catch(const std::exception& e)
    {
        std::string eMsg("In DBInterface::getUniqueModelFromDB ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return false;
    }
    
    return false;
}

/*
 * Protected or private methods.
 */
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

std::string DBInterface::getTableNameFrom(ModelBase &model)
{
    std::string tableName;
    tableName = model.getTableName();
    return tableName;
}

std::string DBInterface::formatInsert(ModelBase &model)
{
    std::string insertFMT("INSERT INTO PlannerTaskScheduleDB." +  getTableNameFrom(model) + " (");
    PTS_DataField_vector allFieldsWithValue = model.getAllFieldsWithValue();
    bool notFirstValue = false;

    for (auto field: allFieldsWithValue)
    {
        if (notFirstValue)
        {
            insertFMT.append(", ");
        }
        insertFMT.append(field->getColumnName());
        notFirstValue = true;
    }
    insertFMT.append(") VALUES (");

    notFirstValue = false;
    for (auto field: allFieldsWithValue)
    {
        if (notFirstValue)
        {
            insertFMT.append(", ");
        }
        insertFMT.append(wrapInSingleQuotes(field->toString()));
        notFirstValue = true;
    }
    insertFMT.append(")");

    return insertFMT;
}

std::string DBInterface::formatSelect(std::string tableName, std::vector<WhereArg> whereArgs)
{
    std::string selectFMT("SELECT * FROM PlannerTaskScheduleDB." +  tableName + " WHERE ");
    bool notFirstTime = false;

    for (auto whereArg: whereArgs)
    {
        if (notFirstTime)
        {
            selectFMT += " AND " + whereArg.first + " = '" + whereArg.second + "'";
        }
        else
        {
            selectFMT += whereArg.first + " = '" + whereArg.second + "'";
            notFirstTime = true;
        }
    }

    return selectFMT;
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

#ifdef SQL4DEBUG
    std::cout << "Executing " << sqlStatement << std::endl; 
#endif

    boost::mysql::results result;
    co_await conn.async_execute(sqlStatement, result);

    co_await conn.async_close();

    co_return result;
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

bool DBInterface::convertResultsToModel(boost::mysql::row_view &sourceFromDB, std::vector<std::string> &columnNames, ModelShp destination)
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

bool DBInterface::executeSimpleQueryProcessResults(std::string sqlStatements, ModelShp destination)
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

