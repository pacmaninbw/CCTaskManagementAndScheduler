#ifndef COREDBINTERFACECORE_H_
#define COREDBINTERFACECORE_H_

// Project Header Files

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <iostream>
#include <optional>
#include <string>

class CoreDBInterface
{
public:
    CoreDBInterface();
    virtual ~CoreDBInterface() = default;
    std::string getAllErrorMessages() const noexcept { return m_ErrorMessages; };
    inline void clearErrorMessages() { m_ErrorMessages.clear(); };
    void setForceExceptions(bool forceIt) { m_ForceException = forceIt; };
    void setSelfTest(bool isSelfTest) { m_SelfTest = isSelfTest; };
    void testResetFormatOpts() { format_opts.reset(); };
    void debugShowVariables(std::string functionName) const noexcept;

protected:
    void initFormatOptions();
    void appendErrorMessage(const std::string& newError) { m_ErrorMessages.append(newError); m_ErrorMessages.append("\n");};
/*
 * Utility functions to perform conversions from C++ class fields to 
 * database fields and back. 
 */
/*
 * Conversions from std::chrono to boost::mysql
 */
    boost::mysql::date stdchronoDateToBoostMySQLDate(const std::chrono::year_month_day& source) noexcept;
    std::chrono::year_month_day boostMysqlDateToChronoDate(const boost::mysql::date& source) noexcept;
    boost::mysql::datetime stdChronoTimePointToBoostDateTime(std::chrono::system_clock::time_point source) noexcept;
    std::chrono::system_clock::time_point boostMysqlDateTimeToChronoTimePoint(boost::mysql::datetime dbDateTime);
    std::optional<boost::mysql::date> optionalDateConversion(std::optional<std::chrono::year_month_day> optDate);
    std::optional<boost::mysql::datetime> optionalDateTimeConversion(std::optional<std::chrono::system_clock::time_point> optDateTime);
/*
 * for MySQL pattern match of string
 */
    std::string wrapSearchContentSQLPatternMatch(std::string searchString) noexcept;

/*
 * All calls to runQueryAsync and getConnectionFormatOptsAsync should be
 * implemented within try blocks.
 */
    boost::mysql::results runQueryAsync(const std::string& query);
    boost::mysql::format_options getConnectionFormatOptsAsync();
    boost::asio::awaitable<boost::mysql::results> coRoutineExecuteSqlStatement(const std::string& query);
    boost::asio::awaitable<boost::mysql::format_options> coRoutineGetFormatOptions();

    std::string m_ErrorMessages;
    boost::mysql::connect_params m_DBConnection;
    bool m_VerboseOutput;
    bool m_ForceError;
    bool m_ForceException;
    bool m_SelfTest;
    std::optional<boost::mysql::format_options> format_opts;
};

#endif // COREDBINTERFACECORE_H_


