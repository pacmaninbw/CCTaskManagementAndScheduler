#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include "BoostDBInterfaceCore.h"
#include <functional>
#include <iostream>

BoostDBInterfaceCore::BoostDBInterfaceCore()
: errorMessages{""},
  verboseOutput{programOptions.verboseOutput}
{
    dbConnectionParameters.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    dbConnectionParameters.username = programOptions.mySqlUser;
    dbConnectionParameters.password = programOptions.mySqlPassword;
    dbConnectionParameters.database = programOptions.mySqlDBName;
}

/*
 * All calls to runQueryAsync should be implemented within try blocks.
 */
NSBM::results BoostDBInterfaceCore::runQueryAsync(std::function<NSBA::awaitable<NSBM::results>(void)> queryFunc)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, queryFunc(),
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

NSBM::results BoostDBInterfaceCore::runQueryAsync(
    std::function<NSBA::awaitable<NSBM::results>(std::size_t)> queryFunc, std::size_t id)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, queryFunc(id),
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


