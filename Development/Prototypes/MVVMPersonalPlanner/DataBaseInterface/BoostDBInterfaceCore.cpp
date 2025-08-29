#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "../CommandLineParser.h"
#include "./BoostDBInterfaceCore.h"
#include <iostream>
#include <sstream>
#include <string>
#include <utility>


BoostDBInterfaceCore::BoostDBInterfaceCore()
: errorMessages{""},
  verboseOutput{programOptions.verboseOutput},
  delimiter{';'}
{
    dbConnectionParameters.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    dbConnectionParameters.username = programOptions.mySqlUser;
    dbConnectionParameters.password = programOptions.mySqlPassword;
    dbConnectionParameters.database = programOptions.mySqlDBName;
}

void BoostDBInterfaceCore::initFormatOptions()
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

/*
 * All calls to runQueryAsync should be implemented within try blocks.
 */
NSBM::results BoostDBInterfaceCore::runQueryAsync(const std::string& query)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, coRoutineExecuteSqlStatement(query),
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

NSBA::awaitable<NSBM::results> BoostDBInterfaceCore::coRoutineExecuteSqlStatement(const std::string& query)
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

void BoostDBInterfaceCore::prepareForRunQueryAsync()
{
    errorMessages.clear();
    initFormatOptions();
};

std::vector<std::string> BoostDBInterfaceCore::explodeTextField(std::string const& textField) noexcept
{
    std::vector<std::string> subFields;
    std::istringstream iss(textField);

    for (std::string token; std::getline(iss, token, delimiter); )
    {
        subFields.push_back(std::move(token));
    }
    return subFields;
}

std::string BoostDBInterfaceCore::implodeTextField(std::vector<std::string> &fields) noexcept
{
    std::string textField;

    for (auto field: fields)
    {
        textField.append(field);
        textField += delimiter;
    }

    return textField;
}

NSBM::format_options BoostDBInterfaceCore::getConnectionFormatOptsAsync()
{
    NSBM::format_options options;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, coRoGetFormatOptions(),
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
    return NSBM::format_options();
}

NSBA::awaitable<NSBM::format_options> BoostDBInterfaceCore::coRoGetFormatOptions()
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::format_options options = conn.format_opts().value();

    co_await conn.async_close();

    co_return options;
}

