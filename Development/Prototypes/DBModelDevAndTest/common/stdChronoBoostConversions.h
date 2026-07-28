#ifndef STDCHRONOBOOSTCONVERSIONS_H_
#define STDCHRONOBOOSTCONVERSIONS_H_

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
extern boost::mysql::date stdchronoDateToBoostMySQLDate(const std::chrono::year_month_day& source) noexcept;
extern std::chrono::year_month_day boostMysqlDateToChronoDate(const boost::mysql::date& source) noexcept;
extern boost::mysql::datetime stdChronoTimePointToBoostDateTime(std::chrono::system_clock::time_point source) noexcept;
extern std::chrono::system_clock::time_point boostMysqlDateTimeToChronoTimePoint(boost::mysql::datetime dbDateTime);

#endif // STDCHRONOBOOSTCONVERSIONS_H_
