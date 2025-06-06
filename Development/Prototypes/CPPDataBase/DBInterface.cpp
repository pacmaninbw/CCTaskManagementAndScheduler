#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "DBInterface.h"
#include <iostream>
#include <exception>
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

DBInterface::DBInterface(std::string table, std::string addStoredProcedure)
:   errorMessages{""},
    tableName{table},
    storedProcedureToAddToTable{addStoredProcedure}
{
    dbConnectionParameters.server_address.emplace_host_and_port(HostURL, MySQLPort);
    dbConnectionParameters.username = MySQLAdminUser;
    dbConnectionParameters.password = MySQLAdminPassword;
    dbConnectionParameters.database = PlannerDB;
}

bool DBInterface::addToDatabaseTable(ModelBase* modelObject)
{
    clearPreviousErrors();

    if (!ModelObjectHasAllRequiredFields(modelObject))
    {
        return false;
    }

    startAddStmt();

    if (!addDataToSqlStatement(modelObject))
    {
        sqlStatement.clear();
        return false;
    }
    
    sqlStatement += ")";

    asyncExecutionSqlStatment(sqlStatement);

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

/*
 * If the stored procedure to add the model to the database does not exist
 * override this function to provide the necessary field names
 */
void DBInterface::addRequiredFieldNames()
{
    if (storedProcedureToAddToTable.empty() || storedProcedureToAddToTable.size() == 0)
    {
        std::cerr << "???ModelToDBInterface::addRequiredFieldNames() not implemented !!!\n";
    }
    sqlStatement.append("No_Field_Name");
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

void DBInterface::startAddStmt()
{
    if (!storedProcedureToAddToTable.empty())
    {
        sqlStatement = "CALL " + storedProcedureToAddToTable + "(";
    }
    else
    {
        sqlStatement = "INSERT INTO " + PlannerDB + "." + tableName + " (";
        addRequiredFieldNames();
        sqlStatement += ") VALUES (";
    }
}

