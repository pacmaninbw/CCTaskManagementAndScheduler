#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "DBInterface.h"
#include <exception>
#include <iostream>
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

DBInterface::DBInterface()
:   errorMessages{""}
{
    dbConnectionParameters.server_address.emplace_host_and_port(HostURL, MySQLPort);
    dbConnectionParameters.username = MySQLAdminUser;
    dbConnectionParameters.password = MySQLAdminPassword;
    dbConnectionParameters.database = PlannerDB;
}

bool DBInterface::updateDatabaseTables(ModelBase* modelObject)
{
    clearPreviousErrors();

    tableName = tableNameBasedonModelType(modelObject);

    if (!ModelHasAllRequiredFields(modelObject))
    {
        return false;
    }

    std::string sqlStatement("START TRANSACTION;\n");
    if (modelObject->isInDataBase())
    {
        sqlStatement += generateSQLUpdateStatement(modelObject);
    }
    else
    {
        sqlStatement += generateSQLInsertStatement(modelObject);
    }
    sqlStatement += "\nCOMMIT;\n";

    try {
        asyncExecutionSqlStatment(sqlStatement);
    }
    catch (const std::exception& err) {
//        std::cout << "Attempted:\n" << sqlStatement << "\n";
        std::cerr << "Error: " << err.what() << "\n";
//        std::cout << "The model object \n" << *modelObject << "\n\n";
        return false;
    }

    return true;
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

std::string DBInterface::generateSQLInsertStatement(ModelBase *modelObject)
{
    std::string fieldNames;
    std::string values;
    PTS_DataField_vector dataFieldsWithValue = modelObject->getAllFieldsWithValue();\
    bool firstLoopIteration = true;


    for (auto currentField: dataFieldsWithValue)
    {
        // If there are already field names or data in te string add the necessary comma operator.
        if (!firstLoopIteration)
        {
            fieldNames += ", ";
            values += ", ";
        }
        fieldNames += tableName + "." + currentField->getColumnName();
/*
 * To prevent SQL injection attacks all data input will be embedded between
 * single quotes.
 */
        values += "'" + currentField->toString() + "'";
        firstLoopIteration = false;
    }

    std::string insertStatement;
        insertStatement = "INSERT INTO " + PlannerDB + "." + tableName + " (";
        insertStatement += fieldNames;
        insertStatement += ") VALUES (";
        insertStatement += values;
        insertStatement += ");";

        return insertStatement;
}

bool DBInterface::ModelHasAllRequiredFields(ModelBase *modelObject)
{
    bool isValid = modelObject->allRequiredFieldsHaveData();

    if (!isValid)
    {
        appendErrorMessage(modelObject->reportMissingRequiredFields());
    }

    return isValid;
}

std::string DBInterface::generateSQLUpdateStatement(ModelBase *modelObject)
{
    std::string updateStatement;

    PTS_DataField_vector updatedFields = modelObject->getAllFieldsWithValue();

    if (updatedFields.size())
    {
        updateStatement = "UPDATE TABLE;";
    }

    return updateStatement;
}

std::string DBInterface::tableNameBasedonModelType(ModelBase *modelObject)
{
    TaskModel* task = dynamic_cast<TaskModel*>(modelObject);
    if (task)
    {
        return "Tasks";
    }
    
    UserModel* user = dynamic_cast<UserModel*>(modelObject);
    if (user)
    {
        return "UserProfile";
    }

    std::string eMsg("Unknown Model Type: " + modelObject->getModelName() + "in DBInterface code generation");
    std::runtime_error UnknownModelType(eMsg);
    throw UnknownModelType;
}
