#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "dbConnectionParameters.h"
#include "DBInterface.h"
#include "InMemUser.h"
#include "InMemUserTask.h"
#include <iostream>
#include <exception>
#include <string>
#include <string_view>

/*
 * The database connection information will not change during the entire
 * execution of the program.
 */
static boost::mysql::connect_params dbConnectionParameters;

DBInterface::DBInterface()
{
    dbConnectionParameters.server_address.emplace_host_and_port("127.0.0.1", 3306);
    dbConnectionParameters.username = MySQLRootUser;
    dbConnectionParameters.password = MySQLRootPassword;
    dbConnectionParameters.database = PlannerDB;
}

/*
 * To limit the includes for Boost/mysql.hpp and Boost/asio.hpp to this file
 * all co-routines are static functions. The boost mysql and asio header files
 * are very large and incur major build times.
 */
static boost::asio::awaitable<void> coro_addUser(const char* sql)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    boost::mysql::results result;

    std::cout << "Attempted SQL statement:\t" << sql << "\n";

    co_await conn.async_execute(sql, result);

    co_await conn.async_close();
}

void DBInterface::addUser(InMemUser &user)
{
    std::string sqlStatmentPrep("CALL addNewUser(");
    appendStringConstantToSqlStmt(sqlStatmentPrep, user.getLastName(), true);
    appendStringConstantToSqlStmt(sqlStatmentPrep, user.getFirstName(), true);
    appendStringConstantToSqlStmt(sqlStatmentPrep, user.getMiddleInitial(), true);
    appendStringConstantToSqlStmt(sqlStatmentPrep, user.getLoginName(), true);
    appendStringConstantToSqlStmt(sqlStatmentPrep, user.getPassword(), false);
    sqlStatmentPrep += ")";

    const char* sqlStmt = sqlStatmentPrep.c_str();

    boost::asio::io_context ctx;

    boost::asio::co_spawn(ctx,
        [sqlStmt] { return coro_addUser(sqlStmt); },
        [](std::exception_ptr ptr) {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
        }
    );

    ctx.run();
}


static boost::asio::awaitable<void> coro_addUserTask(
    std::size_t userID,
    std::string_view description,
    std::size_t parentTask,
    unsigned int estimatedEffortHours,
    unsigned int priorityInAllTasks,
    std::string_view requiredDelivery,
    std::string_view scheduledStart
)
{

    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    boost::mysql::results result;
    co_await conn.async_execute(
        boost::mysql::with_params("CALL createTask({}, {}, {}, {}, {}, {}. {})", 
            userID, description, parentTask, estimatedEffortHours, priorityInAllTasks,
            requiredDelivery, scheduledStart),
        result
    );

    co_await conn.async_close();
}

void DBInterface::addTask(InMemUserTask &task)
{
    std::size_t userID = task.getCreatorID();
    std::string description{""};
    std::size_t parentTask{0};
    unsigned int estimatedEffortHours{0};
    unsigned int priorityInAllTasks{1};
    std::string requiredDelivery("");
    std::string scheduledStart("");

    boost::asio::io_context ctx;

    boost::asio::co_spawn(ctx,
        [userID, description, parentTask, estimatedEffortHours, priorityInAllTasks, requiredDelivery, scheduledStart] {
            return coro_addUserTask(userID, description, parentTask, estimatedEffortHours, priorityInAllTasks, requiredDelivery, scheduledStart); },
        [](std::exception_ptr ptr) {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
        }
    );

    ctx.run();
}



void DBInterface::appendStringConstantToSqlStmt(std::string &sqlStmt, std::string strConstant, bool addComma)
{
    sqlStmt += "'" + strConstant + "'";
    if (addComma)
    {
        sqlStmt += ", ";
    }
}
