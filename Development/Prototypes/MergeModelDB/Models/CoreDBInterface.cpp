#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include "CoreDBInterface.h"
#include <iostream>
#include <string>

CoreDBInterface::CoreDBInterface()
:   errorMessages{""},
    verboseOutput{programOptions.verboseOutput}
{
    dbConnectionParameters.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    dbConnectionParameters.username = programOptions.mySqlUser;
    dbConnectionParameters.password = programOptions.mySqlPassword;
    dbConnectionParameters.database = programOptions.mySqlDBName;
}

void CoreDBInterface::initFormatOptions()
{
    try {
        if (!format_opts.has_value())
        {
            format_opts = getConnectionFormatOptsAsync();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR: initFormatOptions() FAILED: " << e.what() << "\n";
    }
}

void CoreDBInterface::prepareForRunQueryAsync()
{
    errorMessages.clear();
    initFormatOptions();
};

/*
 * All calls to runQueryAsync should be implemented within try blocks.
 */
NSBM::results CoreDBInterface::runQueryAsync(const std::string& query)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(ctx, coRoutineExecuteSqlStatement(query),
        [&localResult, this](std::exception_ptr ptr, NSBM::results result)
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

NSBA::awaitable<NSBM::results> CoreDBInterface::coRoutineExecuteSqlStatement(const std::string& query)
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);
    
    NSBM::results selectResult;

    if (verboseOutput)
    {
        std::clog << "Running: \n\t" << query << "\n";
    }

    co_await conn.async_execute(query, selectResult);

    co_await conn.async_close();

    co_return selectResult;
}

NSBM::format_options CoreDBInterface::getConnectionFormatOptsAsync()
{
    NSBM::format_options options;
    NSBA::io_context ctx;

    NSBA::co_spawn(ctx, coRoutineGetFormatOptions(),
        [&options, this](std::exception_ptr ptr, NSBM::format_options result)
        {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
            options = std::move(result);
        }
    );

    ctx.run();

    return options;
}

NSBA::awaitable<NSBM::format_options> CoreDBInterface::coRoutineGetFormatOptions()
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::format_options options = conn.format_opts().value();

    co_await conn.async_close();

    co_return options;
}


