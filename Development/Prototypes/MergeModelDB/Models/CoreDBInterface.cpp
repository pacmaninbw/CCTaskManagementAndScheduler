// Project Header Files
#include "CommandLineParser.h"
#include "CoreDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

CoreDBInterface::CoreDBInterface()
:   errorMessages{""},
    verboseOutput{programOptions.verboseOutput},
    forceError{programOptions.forceErrors},
    forceException{programOptions.forceExceptions},
    inSelfTest{false}
{
    dbConnectionParameters.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    dbConnectionParameters.username = programOptions.mySqlUser;
    dbConnectionParameters.password = programOptions.mySqlPassword;
    dbConnectionParameters.database = programOptions.mySqlDBName;
}

void CoreDBInterface::debugShowVariables(std::string functionName) const noexcept
{
    std::cout << "In " << functionName << ":\n";
    std::cout << "\tformat_opts: " << (format_opts.has_value()? "TRUE" : "FALSE") << "\n";
    std::cout << "\tinSelfTest: " << (inSelfTest? "TRUE" : "FALSE") << "\n";
    std::cout << "\tforceException: " << (forceException? "TRUE" : "FALSE") << "\n";
    std::cout << std::endl;
}

void CoreDBInterface::initFormatOptions()
{
    if (!format_opts.has_value())
    {
        format_opts = getConnectionFormatOptsAsync();
    }
}

/*
 * All calls to runQueryAsync should be implemented within try blocks.
 */
boost::mysql::results CoreDBInterface::runQueryAsync(const std::string& query)
{
    boost::mysql::results localResult;
    boost::asio::io_context ctx;

    boost::asio::co_spawn(ctx, coRoutineExecuteSqlStatement(query),
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

boost::asio::awaitable<boost::mysql::results> CoreDBInterface::coRoutineExecuteSqlStatement(const std::string& query)
{
    if (forceException)
    {
        std::string forcingException("Forcing Exception in CoreDBInterface::coRoutineExecuteSqlStatement");
        std::domain_error forcedException(forcingException);
        throw forcedException;
    }

    if (inSelfTest)
    {
        if (verboseOutput)
        {
            std::cout << "In Self Test Query is: \n\t" << query << std::endl;
        }
        boost::mysql::results selectResult;
        co_return selectResult;
    }

    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);
    
    boost::mysql::results selectResult;

    if (verboseOutput)
    {
        std::cout << "Running: \n\t" << query << std::endl;
    }

    co_await conn.async_execute(query, selectResult);

    co_await conn.async_close();

    co_return selectResult;
}

boost::mysql::format_options CoreDBInterface::getConnectionFormatOptsAsync()
{
    boost::mysql::format_options options;
    boost::asio::io_context ctx;

    boost::asio::co_spawn(ctx, coRoutineGetFormatOptions(),
        [&options, this](std::exception_ptr getFormatOptsExceptionPtr, boost::mysql::format_options result)
        {
            if (getFormatOptsExceptionPtr)
            {
                std::rethrow_exception(getFormatOptsExceptionPtr);
            }
            options = std::move(result);
        }
    );

    ctx.run();

    return options;
}

boost::asio::awaitable<boost::mysql::format_options> CoreDBInterface::coRoutineGetFormatOptions()
{
    if (forceException)
    {
        std::string forcingException("Forcing Exception in CoreDBInterface::coRoutineGetFormatOptions");
        std::domain_error forcedException(forcingException);
        throw forcedException;
    }

    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    boost::mysql::format_options options = conn.format_opts().value();

    co_await conn.async_close();

    co_return options;
}

