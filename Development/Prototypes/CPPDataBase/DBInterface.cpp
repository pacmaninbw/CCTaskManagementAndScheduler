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

#include "dbConnectionParameters.h"
#include "DBInterface.h"
#include <exception>
#include <string>
#include <string_view>

static boost::mysql::connect_params dbConnectionParameters;

DBInterface::DBInterface()
{
    dbConnectionParameters.server_address.emplace_host_and_port("127.0.0.1", 3306);
    dbConnectionParameters.username = MySQLRootUser;
    dbConnectionParameters.password = MySQLRootPassword;
    dbConnectionParameters.database = PlannerDB;
}

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
    std::string_view lastName,
    std::string_view firstName,
    std::string_view middleInitial,
    std::string_view loginName,
    std::string_view hashedPassword
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

void DBInterface::addUserTaskWithUserID()
{
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
