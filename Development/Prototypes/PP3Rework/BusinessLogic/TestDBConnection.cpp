// Project Header Files
#include "CommandLineParser.h"
#include "TestDBConnection.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

TestDBConnection::TestDBConnection()
:   errorMessages{""},
    verboseOutput{programOptions.verboseOutput},
    forceError{programOptions.forceErrors},
    forceException{programOptions.forceExceptions},
    inSelfTest{false}
{
}

void TestDBConnection::debugShowVariables(std::string functionName) const noexcept
{
    std::cerr << "In " << functionName << ":\n";
    std::cerr << "\tformat_opts: " << (format_opts.has_value()? "TRUE" : "FALSE") << "\n";
    std::cerr << "\tinSelfTest: " << (inSelfTest? "TRUE" : "FALSE") << "\n";
    std::cerr << "\tforceException: " << (forceException? "TRUE" : "FALSE") << "\n";
    std::cerr << std::endl;
}

bool TestDBConnection::runTestConnect()
{
    try
    {
        initConnectionData(programOptions);
        format_opts.reset();
        format_opts = getConnectionFormatOptsAsync();

        if (!format_opts.has_value())
        {
            std::string eMsg(std::format("Failed to connect to database {}", programOptions.mySqlDBName));
            errorMessages.append(eMsg);
            std::cerr << eMsg << std::endl;
            return false;
        }

        return true;
    }

    catch(const std::exception& e)
    {
        std::string eMsg(std::format("Failed to connect to database {}: ", programOptions.mySqlDBName));
        eMsg.append(e.what());
        errorMessages.append(eMsg);
        return false;
    }    
}

boost::mysql::format_options TestDBConnection::getConnectionFormatOptsAsync()
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

boost::asio::awaitable<boost::mysql::format_options> TestDBConnection::coRoutineGetFormatOptions()
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

void TestDBConnection::initConnectionData(ProgramOptions &po)
{
    dbConnectionParameters.server_address.emplace_host_and_port(po.mySqlUrl, po.mySqlPort);
    dbConnectionParameters.username = po.mySqlUser;
    dbConnectionParameters.password = po.mySqlPassword;
    dbConnectionParameters.database = po.mySqlDBName;
}
