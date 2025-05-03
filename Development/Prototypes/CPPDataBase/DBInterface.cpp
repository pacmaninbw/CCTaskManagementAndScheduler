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
#include <cstdint>
#include "dbConnectionParameters.h"
#include "DBInterface.h"
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
static boost::asio::awaitable<void> coro_addUser(
    std::string_view lastName,
    std::string_view firstName,
    std::string_view middleInitial,
    std::string_view loginName,
    std::string_view hashedPassword
)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    boost::mysql::results result;
    co_await conn.async_execute(
        boost::mysql::with_params("CALL addNewUser({}, {}, {}, {}, {})", 
            lastName, firstName, middleInitial, loginName, hashedPassword),
        result
    );

    co_await conn.async_close();
}

void DBInterface::addUser(
    std::string_view& lastName,
    std::string_view& firstName,
    std::string_view& middleInitial,
    std::string_view& loginName,
    std::string_view& hashedPassword
)
{
    throwExceptionIfNotValidAddUser(lastName, firstName, loginName, hashedPassword);

    boost::asio::io_context ctx;

    boost::asio::co_spawn(ctx,
        [lastName, firstName, middleInitial, loginName, hashedPassword] {
            return coro_addUser(lastName, firstName, middleInitial, loginName, hashedPassword); },
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
    KeyType userID,
    std::string_view description,
    KeyType parentTask,
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
    KeyType userID,
    std::string_view description,
    KeyType parentTask,
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
    std::string_view& contents,
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

void DBInterface::throwExceptionIfNotValidAddUser(
    std::string_view &lastName,
    std::string_view &firstName,
    std::string_view &loginName,
    std::string_view &hashedPassword
)
{
    unsigned int errorCount = 0;
    std::string addUserErrorMessage("Error(s) in DBInterface::addUser() ");
    std::string_view strName;

    strName = "Last Name";
    stringViewValidationOrErrorMsg(lastName, strName, addUserErrorMessage, errorCount);

    strName = "First Name";
    stringViewValidationOrErrorMsg(firstName, strName, addUserErrorMessage, errorCount);

    strName = "User Login";
    stringViewValidationOrErrorMsg(loginName, strName, addUserErrorMessage, errorCount);

    strName = "Password";
    stringViewValidationOrErrorMsg(hashedPassword, strName, addUserErrorMessage, errorCount);

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
