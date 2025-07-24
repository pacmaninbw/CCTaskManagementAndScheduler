#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "DBInterface.h"
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

DBInterface::DBInterface(ProgramOptions& programOptions)
:   errorMessages{""}, databaseName{programOptions.mySqlDBName}, verboseOutput{programOptions.verboseOutput}
{
    dbConnectionParameters.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    dbConnectionParameters.username = programOptions.mySqlUser;
    dbConnectionParameters.password = programOptions.mySqlPassword;
    dbConnectionParameters.database = programOptions.mySqlDBName;
}

/*
 * All boost::mysql executions return results. The results are processed at a higher
 * level as necessary where the knowlege about the results exists. This method only
 * executes the SQL statement(s) and returns all results.
 */
boost::asio::awaitable<boost::mysql::results> DBInterface::executeSqlStatementsCoRoutine(std::string sqlStatement)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);
/*
 * To properly process the select statements we need the column names in the metadata.
 */
    size_t foundPos = sqlStatement.find("SELECT");
    if (foundPos != std::string::npos) {
        conn.set_meta_mode(boost::mysql::metadata_mode::full);
    }
    else
    {
        conn.set_meta_mode(boost::mysql::metadata_mode::minimal);
    }

    if (verboseOutput)
    {
        std::clog << "Executing " << sqlStatement << std::endl; 
    }

    boost::mysql::results result;
    co_await conn.async_execute(sqlStatement, result);

    co_await conn.async_close();

    co_return result;
}

boost::mysql::results DBInterface::runAnyMySQLstatementsAsynchronously(std::string selectSqlStatement)
{
    boost::asio::io_context ctx;
    boost::mysql::results localResult;

    boost::asio::co_spawn(
        ctx, executeSqlStatementsCoRoutine(selectSqlStatement),
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

