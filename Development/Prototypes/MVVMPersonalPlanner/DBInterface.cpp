#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include "DBInterface.h"
#include <functional>
#include <iostream>

DBInterface::DBInterface()
:   errorMessages{""}, databaseName{programOptions.mySqlDBName}, verboseOutput{programOptions.verboseOutput}
{
    dbConnectionParameters.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    dbConnectionParameters.username = programOptions.mySqlUser;
    dbConnectionParameters.password = programOptions.mySqlPassword;
    dbConnectionParameters.database = programOptions.mySqlDBName;
}

/*
 * All calls to runQueryAsync should be implemented within try blocks.
 */
NSBM::results DBInterface::runQueryAsync(
    std::function<NSBA::awaitable<NSBM::results>(std::size_t, std::chrono::year_month_day)>queryFunc, 
    std::size_t id, std::chrono::year_month_day searchDate)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, queryFunc(id, searchDate),
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

NSBM::results DBInterface::runQueryAsync(
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

NSBM::results DBInterface::runQueryAsync(
    std::function<NSBA::awaitable<NSBM::results>(std::string_view, std::string_view, std::string_view)> queryFunc,
    std::string_view searchStr1, std::string_view searchStr2, std::string_view searchStr3)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, queryFunc(searchStr1, searchStr2, searchStr3),
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

NSBM::results DBInterface::runQueryAsync(
    std::function<NSBA::awaitable<NSBM::results>(std::string_view, std::string_view)> queryFunc,
    std::string_view searchStr1, std::string_view searchStr2)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, queryFunc(searchStr1, searchStr2),
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

NSBM::results DBInterface::runQueryAsync(
    std::function<NSBA::awaitable<NSBM::results>(std::string_view)> queryFunc,
    std::string_view searchString)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, queryFunc(searchString),
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

NSBM::results DBInterface::runQueryAsync(std::function<NSBA::awaitable<NSBM::results>(void)> queryFunc)
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

NSBM::results DBInterface::runQueryAsync(
    std::function<NSBA::awaitable<NSBM::results>(std::string_view, std::size_t)> queryFunc,
    std::string_view searchStr, std::size_t id)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, queryFunc(searchStr, id),
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
