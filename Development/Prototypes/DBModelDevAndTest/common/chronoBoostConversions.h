#ifndef CHRONOBOOSTCONVERSIONS_H_
#define CHRONOBOOSTCONVERSIONS_H_

// Project Header Files

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>

/*
 * Utility functions to perform conversions from C++ class fields to 
 * database fields and back. 
 */
/*
 * Conversions from std::chrono to boost::mysql
 */
namespace common {
    extern boost::mysql::date toBoostMySQLDate(const std::chrono::year_month_day& source) noexcept;
    extern std::chrono::year_month_day toChronoDate(const boost::mysql::date& source) noexcept;
    extern boost::mysql::datetime toBoostDateTime(std::chrono::system_clock::time_point source) noexcept;
    extern std::chrono::system_clock::time_point toChronoTimePoint(boost::mysql::datetime dbDateTime);
}

#endif // CHRONOBOOSTCONVERSIONS_H_
