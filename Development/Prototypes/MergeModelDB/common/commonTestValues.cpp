// Project Header Files
#include "commonTestValues.h"
#include "commonUtilities.h"

// Standard C++ Header Files
#include <chrono>
#include <string>

std::chrono::system_clock::time_point commonTestTimeStampValue;
std::chrono::year_month_day commonTestDateValue;
std::chrono::year_month_day commonTestDateRangeStartValue;
std::chrono::year_month_day commonTestDateRangeEndValue;

static void initTestDateAndTimeStamp(std::string timeStampStr)
{
    commonTestTimeStampValue = constantStringToChronoTimePoint(timeStampStr);

    commonTestDateValue = std::chrono::floor<std::chrono::days>(commonTestTimeStampValue);

    std::chrono::time_point<std::chrono::system_clock> pastDate = commonTestTimeStampValue;
    pastDate -= std::chrono::days(OneWeek);
    commonTestDateRangeStartValue = std::chrono::floor<std::chrono::days>(pastDate);

    std::chrono::time_point<std::chrono::system_clock> futureDate = commonTestTimeStampValue;
    futureDate += std::chrono::days(OneWeek);
    commonTestDateRangeEndValue = std::chrono::floor<std::chrono::days>(futureDate);
}

void initCommonTestValues(std::string timeStampStr)
{
    initTestDateAndTimeStamp(timeStampStr);
    
}
