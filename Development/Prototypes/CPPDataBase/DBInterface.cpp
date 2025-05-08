#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "dbConnectionParameters.h"
#include "DBInterface.h"
#include "UserModel.h"
#include "TaskModel.h"
#include <iostream>
#include <exception>
#include <string>
#include <string_view>

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
: tableName{table}, storedProcedureToAddToTable{addStoredProcedure}
{
    dbConnectionParameters.server_address.emplace_host_and_port("127.0.0.1", 3306);
    dbConnectionParameters.username = MySQLRootUser;
    dbConnectionParameters.password = MySQLRootPassword;
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

void DBInterface::addUser(UserModel &user)
{
    startAddStmt();

    appendArgToSqlStmt(user.getLastName(), true);
    appendArgToSqlStmt(user.getFirstName(), true);
    appendArgToSqlStmt(user.getMiddleInitial(), true);
    appendArgToSqlStmt(user.getLoginName(), true);
    appendArgToSqlStmt(user.getPassword());
    
    sqlStatement += ")";

    asyncExecutionSqlStatment(sqlStatement.c_str());
}

void DBInterface::addTask(TaskModel &task)
{
    std::size_t priorityInAllTasks{1};

    startAddStmt();

    appendArgToSqlStmt(task.getCreatorID(), true);
    appendArgToSqlStmt(task.getDescription(), true);
    appendArgToSqlStmt(task.getParentTaskID(), true);
    appendArgToSqlStmt(task.getEstimatedEffort(), true);
    appendArgToSqlStmt(priorityInAllTasks, true);
    appendArgToSqlStmt(task.getDueDate(), true);
    appendArgToSqlStmt(task.getScheduledStart());

    sqlStatement += ")";

    asyncExecutionSqlStatment(sqlStatement.c_str());

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
        sqlStatement = "INSERT INTO " + std::string(PlannerDB) + "." + tableName + " (";
    }
}
