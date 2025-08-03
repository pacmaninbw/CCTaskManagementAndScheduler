#include <chrono>
#include "commonUtilities.h"

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
