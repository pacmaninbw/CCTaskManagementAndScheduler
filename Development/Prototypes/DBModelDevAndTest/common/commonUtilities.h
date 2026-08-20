#ifndef COMMONUTILITIES_H_
#define COMMONUTILITIES_H_
#include <chrono>
#include <string>

namespace common
{
    extern unsigned int OneWeek;
    extern unsigned int TwoWeeks;

    extern std::chrono::year_month_day getTodaysDate();
    extern std::chrono::year_month_day getTodaysDatePlus(unsigned int offset);
    extern std::chrono::year_month_day getTodaysDateMinus(unsigned int offset);
    extern std::chrono::year_month_day constantStringToChronoDate(std::string dateStr);
    extern std::chrono::system_clock::time_point constantStringToChronoTimePoint(std::string dateTimeStr);
    extern std::chrono::system_clock::time_point getLocalMidnight(std::chrono::year_month_day scheduleDate);
    extern void getHourRangeForLocalDate(std::chrono::year_month_day dateIn,
        std::chrono::system_clock::time_point& startTime,
        std::chrono::system_clock::time_point& endTime);
}

#endif // COMMONUTILITIES_H_
