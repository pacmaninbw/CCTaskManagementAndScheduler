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
std::chrono::year_month_day commonProductionTestDataAddedDate;

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

    std::string integratedValuesFromProduction{"2026-03-22 08:31:53"};
    std::chrono::system_clock::time_point productionTestIntegration = constantStringToChronoTimePoint(integratedValuesFromProduction);
    commonProductionTestDataAddedDate = std::chrono::floor<std::chrono::days>(productionTestIntegration);
}

void initCommonTestValues(std::string timeStampStr)
{
    initTestDateAndTimeStamp(timeStampStr);
    
}
