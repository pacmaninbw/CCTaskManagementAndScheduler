#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include "ModelDBInterface.h"
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


ModelDBInterface::ModelDBInterface(std::string_view modelNameIn)
: primaryKey{0},
  modelName{modelNameIn},
  errorMessages{""},
  modified{false},
  verboseOutput{programOptions.verboseOutput},
  delimiter{';'}  
{
    dbConnectionParameters.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    dbConnectionParameters.username = programOptions.mySqlUser;
    dbConnectionParameters.password = programOptions.mySqlPassword;
    dbConnectionParameters.database = programOptions.mySqlDBName;
}

bool ModelDBInterface::save()
{
    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified, no changes to save", modelName));
        return false;
    }

    if (isInDataBase())
    {
        return update();
    }
    else
    {
        return insert();
    }

}

bool ModelDBInterface::insert()
{
    errorMessages.clear();

    if (isInDataBase())
    {
        appendErrorMessage(std::format("{} already in Database, use Update!", modelName));
        return false;
    }

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", modelName));
        return false;
    }

    if (!hasRequiredValues())
    {
        appendErrorMessage(std::format("{} is missing required values!", modelName));
        reportMissingFields();
        return false;
    }

    prepareForRunQueryAsync();

    try
    {
        NSBM::results localResult = runQueryAsync(formatInsertStatement());
        primaryKey = localResult.last_insert_id();
        modified = false;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.insert : {}", modelName, e.what()));
        return false;
    }

    return true;
}

bool ModelDBInterface::update()
{
    prepareForRunQueryAsync();

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", modelName));
        return false;
    }

    try
    {

        NSBM::results localResult = runQueryAsync(formatUpdateStatement());
        modified = false;
            
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.update : {}", modelName, e.what()));
        return false;
    }
}

bool ModelDBInterface::retrieve()
{
    prepareForRunQueryAsync();

    try
    {
        NSBM::results localResult = runQueryAsync(formatSelectStatement());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.retrieve() : {}", modelName, e.what()));
        return false;
    }
}

bool ModelDBInterface::hasRequiredValues()
{
    initRequiredFields();
    for (auto fieldTest: missingRequiredFieldsTests)
    {
        if (fieldTest.errorCondition())
        {
            return false;
        }
    }
    
    return true;
}

void ModelDBInterface::reportMissingFields() noexcept
{
    for (auto testAndReport: missingRequiredFieldsTests)
    {
        if (testAndReport.errorCondition())
        {
            appendErrorMessage(std::format("Missing {} required {}!\n", modelName, testAndReport.fieldName));
        }
    }
}

void ModelDBInterface::initFormatOptions()
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

bool ModelDBInterface::processResult(NSBM::results& results)
{
    if (results.rows().empty())
    {
        appendErrorMessage(std::format("{} not found!", modelName));
        return false;
    }

    if (results.rows().size() > 1)
    {
        appendErrorMessage(std::format("Too many {}s found to process!", modelName));
        return false;
    }

    NSBM::row_view rv = results.rows().at(0);
    processResultRow(rv);

    return true;
}
/*
 * All calls to runQueryAsync should be implemented within try blocks.
 */
NSBM::results ModelDBInterface::runQueryAsync(const std::string& query)
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

NSBA::awaitable<NSBM::results> ModelDBInterface::coRoutineExecuteSqlStatement(const std::string& query)
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

void ModelDBInterface::prepareForRunQueryAsync()
{
    errorMessages.clear();
    initFormatOptions();
};

std::vector<std::string> ModelDBInterface::explodeTextField(std::string const& textField) noexcept
{
    std::vector<std::string> subFields;
    std::istringstream iss(textField);

    for (std::string token; std::getline(iss, token, delimiter); )
    {
        subFields.push_back(std::move(token));
    }
    return subFields;
}

std::string ModelDBInterface::implodeTextField(std::vector<std::string> &fields) noexcept
{
    std::string textField;

    for (auto field: fields)
    {
        textField.append(field);
        textField += delimiter;
    }

    return textField;
}

NSBM::format_options ModelDBInterface::getConnectionFormatOptsAsync()
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
    return NSBM::format_options();
}

NSBA::awaitable<NSBM::format_options> ModelDBInterface::coRoutineGetFormatOptions()
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::format_options options = conn.format_opts().value();

    co_await conn.async_close();

    co_return options;
}

