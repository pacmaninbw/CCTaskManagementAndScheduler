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
    m_forceError{programOptions.forceErrors},
    m_forceException{programOptions.forceExceptions},
    m_selfTest{false},
    testProductionConnection{false}
{
}

void TestDBConnection::debugShowVariables(std::string functionName) const noexcept
{
    std::cerr << "In " << functionName << ":\n";
    std::cerr << "\tformat_opts: " << (m_formatOpts.has_value()? "TRUE" : "FALSE") << "\n";
    std::cerr << "\tinSelfTest: " << (m_selfTest? "TRUE" : "FALSE") << "\n";
    std::cerr << "\tforceException: " << (m_forceException? "TRUE" : "FALSE") << "\n";
    std::cerr << std::endl;
}

TestStatus TestDBConnection::runTestConnect()
{
    try
    {
        initConnectionData(programOptions);
        m_formatOpts.reset();
        m_formatOpts = getConnectionFormatOptsAsync();

        // Test connecting to the default Test Database.
        if (!m_formatOpts.has_value())
        {
            std::cerr << std::format("Failed to get format options from {} : TEST FAILED", programOptions.mySqlDBName) << std::endl;
            return TESTFAILED;
        }
        else
        {
            std::cerr << std::format("Connected to {}", programOptions.mySqlDBName) << std::endl;
        }

        if (testProductionConnection)
        {
            // Test connecting to the real database
            programOptions.mySqlDBName = "PlannerTaskScheduleDB";
            initConnectionData(programOptions);
            m_formatOpts.reset();
            m_formatOpts = getConnectionFormatOptsAsync();

            // Test connecting to the default Test Database.
            if (!m_formatOpts.has_value())
            {
                std::cerr << std::format("Failed to get format options from {} : TEST FAILED", programOptions.mySqlDBName) << std::endl;
                return TESTFAILED;
            }
            else
            {
                std::cerr << std::format("Connected to {}", programOptions.mySqlDBName) << std::endl;
            }
        }

        programOptions.canConnectToDatabase = true;
        
        return TESTPASSED;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        debugShowVariables("");
        return TESTFAILED;
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
    if (m_forceException)
    {
        std::string forcingException("Forcing Exception in CoreDBInterface::coRoutineGetFormatOptions");
        std::domain_error forcedException(forcingException);
        throw forcedException;
    }

    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(m_dbConnection);

    boost::mysql::format_options options = conn.format_opts().value();

    co_await conn.async_close();

    co_return options;
}

void TestDBConnection::initConnectionData(ProgramOptions &po)
{
    m_dbConnection.server_address.emplace_host_and_port(po.mySqlUrl, po.mySqlPort);
    m_dbConnection.username = po.mySqlUser;
    m_dbConnection.password = po.mySqlPassword;
    m_dbConnection.database = po.mySqlDBName;
}
