#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "DBInterface.h"
#include <exception>
#include <format>
#include <iostream>
#include "ModelBase.h"
#include <optional>
#include "PTS_DataField.h"
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

DBInterface::DBInterface(ProgramOptions& programOptions)
:   errorMessages{""}, databaseName{programOptions.mySqlDBName}, verboseOutput{programOptions.verboseOutput}
{
    dbConnectionParameters.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    dbConnectionParameters.username = programOptions.mySqlUser;
    dbConnectionParameters.password = programOptions.mySqlPassword;
    dbConnectionParameters.database = programOptions.mySqlDBName;
}

bool DBInterface::insertIntoDataBase(ModelBase& model)
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

    try
    {
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
    clearPreviousErrors();

    if (!model)
    {
        std::invalid_argument missingModelPtr("NULL model pointer in DBInterface::getUniqueModelFromDB()!");
        throw missingModelPtr;
    }

    try
    {
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
std::string DBInterface::getTableNameFrom(ModelBase &model)
{
    std::string tableName;
    tableName = model.getTableName();
    return tableName;
}

std::string DBInterface::formatInsert(ModelBase &model)
{
    PTS_DataField_vector allFieldsWithValue = model.getAllFieldsWithValue();
    std::string argFmt;
    std::string valueFmt;

    bool noComma = true;
    for (auto field: allFieldsWithValue)
    {
        argFmt.append((noComma) ? std::format("{}", field->getColumnName()) : 
            std::format(", {}", field->getColumnName()));
        valueFmt.append((noComma) ? std::format("{}", field->toString()) :
            std::format(", '{}'", field->toString()));
        noComma = false;
    }

    std::string insertFMT(std::format("INSERT INTO {}.{} ({}) VALUES ({})", databaseName,
        getTableNameFrom(model), argFmt, valueFmt));

    return insertFMT;
}

std::string DBInterface::formatSelect(std::string tableName, std::vector<WhereArg> whereArgs)
{
    std::string selectFMT(std::format("SELECT * FROM {}.{} WHERE ", databaseName, tableName));

    bool noComma = true;
    for (auto whereArg: whereArgs)
    {
        selectFMT.append((noComma)? std::format("{} = '{}'", whereArg.first, whereArg.second.toString()) :
            std::format(" AND {} = '{}'", whereArg.first, whereArg.second.toString()));
        noComma = false;
    }

    return selectFMT;
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

    if (verboseOutput)
    {
        std::clog << "Executing " << sqlStatement << std::endl; 
    }

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

