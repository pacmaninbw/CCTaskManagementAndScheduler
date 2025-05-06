#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/this_coro.hpp>
#include <boost/mysql.hpp>
#include <boost/mysql/any_address.hpp>
#include <boost/mysql/any_connection.hpp>
#include <boost/mysql/error_with_diagnostics.hpp>
#include <boost/mysql/results.hpp>
#include <boost/mysql/row_view.hpp>
#include <boost/mysql/with_params.hpp>
//#include <cstdint>
#include "dbConnectionParameters.h"
#include "DBInterface.h"
#include "InMemUser.h"
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
    throwExceptionIfNotValidAddUser(user);

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

void DBInterface::addUserTaskWithUserID(
    std::size_t userID,
    std::string_view description,
    std::size_t parentTask,
    unsigned int estimatedEffortHours,
    unsigned int priorityInAllTasks,
    std::string_view requiredDelivery,
    std::string_view scheduledStart
)
{
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

void DBInterface::addUserTaskWithLoginName()
{
}

void DBInterface::stringViewValidationOrErrorMsg(
    std::string_view contents,
    std::string_view& strName,
    std::string& eMsg,
    unsigned int& errorCount
) const
{
    if (contents.empty() || contents.size() == 0)
    {
        if (errorCount)
        {
            eMsg += ", ";;
        }

        eMsg += strName;
        errorCount++;
    }
}

void DBInterface::throwExceptionIfNotValidAddUser(InMemUser &user)
{
    unsigned int errorCount = 0;
    std::string addUserErrorMessage("Error(s) in DBInterface::addUser() ");
    std::string_view strName;

    strName = "Last Name";
    stringViewValidationOrErrorMsg(user.getLastName(), strName, addUserErrorMessage, errorCount);

    strName = "First Name";
    stringViewValidationOrErrorMsg(user.getFirstName(), strName, addUserErrorMessage, errorCount);

    strName = "User Login";
    stringViewValidationOrErrorMsg(user.getLoginName(), strName, addUserErrorMessage, errorCount);

    strName = "Password";
    stringViewValidationOrErrorMsg(user.getPassword(), strName, addUserErrorMessage, errorCount);

    if (errorCount)
    {
        if (errorCount > 1)
        {
            addUserErrorMessage += " are missing.";
        }
        else
        {
            addUserErrorMessage += " is missing.";
        }
        std::runtime_error addUserException(addUserErrorMessage);
        throw addUserException;
    }
}

void DBInterface::appendStringConstantToSqlStmt(std::string &sqlStmt, std::string strConstant, bool addComma)
{
    sqlStmt += "'" + strConstant + "'";
    if (addComma)
    {
        sqlStmt += ", ";
    }
}
