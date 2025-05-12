#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "DBInterface.h"
#include <iostream>
#include <exception>
#include <string>
#include <string_view>

static const std::string MySQLAdminUser{"REDACTED"};
static const std::string MySQLAdminPassword{"REDACTED"};
static const std::string PlannerDB{"PlannerTaskScheduleDB"};

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
: errorMessages{""}, tableName{table}, storedProcedureToAddToTable{addStoredProcedure}
{
    dbConnectionParameters.server_address.emplace_host_and_port("127.0.0.1", 3306);
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
        clearSqlStmt();
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
    }
}

