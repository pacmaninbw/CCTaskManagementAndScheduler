/*
 * Utility functions to perform conversions from C++ class fields to 
 * database fields and back. 
 */

 // Project Header Files
#include "chronoBoostConversions.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>

namespace common {

/*
 * Conversions from std::chrono to boost::mysql and back again.
 */

    boost::mysql::date toBoostMySQLDate(const std::chrono::year_month_day &source) noexcept
    {
        std::chrono::sys_days tp = source;
        return boost::mysql::date(tp);
    }

    std::chrono::year_month_day toChronoDate(const boost::mysql::date &source) noexcept
    {
        const std::chrono::year year{source.year()};
        const std::chrono::month month{source.month()};
        const std::chrono::day day{source.day()};
        return std::chrono::year_month_day{year, month, day};
    }

    boost::mysql::datetime toBoostDateTime(std::chrono::system_clock::time_point source) noexcept
    {
        return boost::mysql::datetime(std::chrono::time_point_cast<boost::mysql::datetime::time_point::duration>(source));
    }

    std::chrono::system_clock::time_point toChronoTimePoint(boost::mysql::datetime dbDateTime)
    {
        return std::chrono::time_point_cast<std::chrono::system_clock::time_point::duration>(dbDateTime.as_time_point());
    }
}
