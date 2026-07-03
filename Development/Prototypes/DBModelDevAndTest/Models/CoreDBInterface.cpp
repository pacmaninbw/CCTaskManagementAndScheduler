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
:   m_ErrorMessages{""},
    m_VerboseOutput{programOptions.verboseOutput},
    m_ForceError{programOptions.forceErrors},
    m_ForceException{programOptions.forceExceptions},
    m_SelfTest{false}
{
    m_DBConnection.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    m_DBConnection.username = programOptions.mySqlUser;
    m_DBConnection.password = programOptions.mySqlPassword;
    m_DBConnection.database = programOptions.mySqlDBName;
}

void CoreDBInterface::debugShowVariables(std::string functionName) const noexcept
{
    std::cout << "In " << functionName << ":\n";
    std::cout << "\tformat_opts: " << (format_opts.has_value()? "TRUE" : "FALSE") << "\n";
    std::cout << "\tinSelfTest: " << (m_SelfTest? "TRUE" : "FALSE") << "\n";
    std::cout << "\tforceException: " << (m_ForceException? "TRUE" : "FALSE") << "\n";
    std::cout << std::endl;
}

boost::mysql::date CoreDBInterface::stdchronoDateToBoostMySQLDate(const std::chrono::year_month_day &source) noexcept
{
    std::chrono::sys_days tp = source;
    return boost::mysql::date(tp);
}

std::chrono::year_month_day CoreDBInterface::boostMysqlDateToChronoDate(const boost::mysql::date &source) noexcept
{
    const std::chrono::year year{source.year()};
    const std::chrono::month month{source.month()};
    const std::chrono::day day{source.day()};
    return std::chrono::year_month_day{year, month, day};
}

boost::mysql::datetime CoreDBInterface::stdChronoTimePointToBoostDateTime(std::chrono::system_clock::time_point source) noexcept
{
    return boost::mysql::datetime(std::chrono::time_point_cast<boost::mysql::datetime::time_point::duration>(source));
}

std::chrono::system_clock::time_point CoreDBInterface::boostMysqlDateTimeToChronoTimePoint(boost::mysql::datetime dbDateTime)
{
    return std::chrono::time_point_cast<std::chrono::system_clock::time_point::duration>(dbDateTime.as_time_point());
}

std::optional<boost::mysql::date> CoreDBInterface::optionalDateConversion(std::optional<std::chrono::year_month_day> optDate)
{
    std::optional<boost::mysql::date> mySqlDate;

    if (optDate.has_value())
    {
        mySqlDate = stdchronoDateToBoostMySQLDate(optDate.value());
    }

    return mySqlDate;
}
std::optional<boost::mysql::datetime> CoreDBInterface::optionalDateTimeConversion(std::optional<std::chrono::system_clock::time_point> optDateTime)
{
    std::optional<boost::mysql::datetime> timeStamp;

    if (optDateTime.has_value())
    {
        timeStamp = stdChronoTimePointToBoostDateTime(optDateTime.value());
    }

    return timeStamp;
};

std::string CoreDBInterface::wrapSearchContentSQLPatternMatch(std::string searchString) noexcept
{
    std::string patternMatchString("%");
    patternMatchString.append(searchString);
    patternMatchString.append("%");

    return patternMatchString;
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
    if (m_ForceException)
    {
        std::string forcingException("Forcing Exception in CoreDBInterface::coRoutineExecuteSqlStatement");
        std::domain_error forcedException(forcingException);
        throw forcedException;
    }

    if (m_SelfTest)
    {
        if (m_VerboseOutput)
        {
            std::cout << "In Self Test Query is: \n\t" << query << std::endl;
        }
        boost::mysql::results selectResult;
        co_return selectResult;
    }

    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);
    conn.set_meta_mode(boost::mysql::metadata_mode::full);

    co_await conn.async_connect(m_DBConnection);
    
    boost::mysql::results selectResult;

    if (m_VerboseOutput)
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
    if (m_ForceException)
    {
        std::string forcingException("Forcing Exception in CoreDBInterface::coRoutineGetFormatOptions");
        std::domain_error forcedException(forcingException);
        throw forcedException;
    }

    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);
    conn.set_meta_mode(boost::mysql::metadata_mode::full);


    co_await conn.async_connect(m_DBConnection);

    boost::mysql::format_options options = conn.format_opts().value();

    co_await conn.async_close();

    co_return options;
}

