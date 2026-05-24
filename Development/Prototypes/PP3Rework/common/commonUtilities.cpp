#include <chrono>
#include "commonUtilities.h"
#include <format>
#include <iostream>
#include <sstream>
#include <string>

unsigned int OneWeek = 7;
unsigned int TwoWeeks = 14;

std::chrono::year_month_day getTodaysDate()
{
    std::chrono::time_point<std::chrono::system_clock> today = std::chrono::system_clock::now();
    return std::chrono::floor<std::chrono::days>(today);
}

std::chrono::year_month_day getTodaysDatePlus(unsigned int offset)
{
    std::chrono::time_point<std::chrono::system_clock> futureDate = std::chrono::system_clock::now();
    futureDate += std::chrono::days(offset);
    return std::chrono::floor<std::chrono::days>(futureDate);
}

std::chrono::year_month_day getTodaysDateMinus(unsigned int offset)
{
    std::chrono::time_point<std::chrono::system_clock> pastDate = std::chrono::system_clock::now();
    pastDate -= std::chrono::days(offset);
    return std::chrono::floor<std::chrono::days>(pastDate);
}

std::chrono::year_month_day constantStringToChronoDate(std::string dateStr)
{
    std::chrono::year_month_day dateValue;
    std::istringstream ss(dateStr);

    ss >> std::chrono::parse("%Y-%m-%d", dateValue);
    if (ss.fail())
    {
        std::cerr << std::format("{} Failed to parse string {} to year_month_day.", __func__, dateStr) << std::endl;
    }

    return dateValue;
}

std::chrono::system_clock::time_point constantStringToChronoTimePoint(std::string dateTimeStr)
{
    std::chrono::system_clock::time_point dateTimeValue;

 
    std::stringstream ss(dateTimeStr);
    // Use std::chrono::from_stream with the desired format specifier
    ss >> std::chrono::parse("%Y-%m-%d %H:%M:%S", dateTimeValue);

    if (ss.fail()) {
        std::cerr << std::format("{} Failed to parse string {} to time_point.", __func__, dateTimeStr) << std::endl;
    }

    return dateTimeValue;
}

std::chrono::system_clock::time_point getLocalMidnight(std::chrono::year_month_day scheduleDate)
{
    const std::chrono::time_zone* localTimeZone = std::chrono::current_zone();
    const std::chrono::local_days local_midnight{scheduleDate};
    const std::chrono::zoned_time zt_midnight = localTimeZone->to_sys(local_midnight);
    return zt_midnight.get_sys_time();
}
