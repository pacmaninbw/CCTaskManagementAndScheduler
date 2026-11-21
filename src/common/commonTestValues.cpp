// Project Header Files
#include "commonTestValues.h"
#include "commonUtilities.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <string>

namespace common
{
    std::chrono::system_clock::time_point TestTimeStampValue;
    std::chrono::year_month_day TestDateValue;
    std::chrono::year_month_day TestDateRangeStartValue;
    std::chrono::year_month_day TestDateRangeEndValue;
    std::chrono::year_month_day ProductionTestDataAddedDate;

    static void initTestDateAndTimeStamp(std::string timeStampStr)
    {
        TestTimeStampValue = constantStringToChronoTimePoint(timeStampStr);

        TestDateValue = std::chrono::floor<std::chrono::days>(TestTimeStampValue);

        std::chrono::time_point<std::chrono::system_clock> pastDate = TestTimeStampValue;
        pastDate -= std::chrono::days(OneWeek);
        TestDateRangeStartValue = std::chrono::floor<std::chrono::days>(pastDate);

        std::chrono::time_point<std::chrono::system_clock> futureDate = TestTimeStampValue;
        futureDate += std::chrono::days(OneWeek);
        TestDateRangeEndValue = std::chrono::floor<std::chrono::days>(futureDate);

        std::string integratedValuesFromProduction{"2026-03-22 08:31:53"};
        std::chrono::system_clock::time_point productionTestIntegration = constantStringToChronoTimePoint(integratedValuesFromProduction);
        ProductionTestDataAddedDate = std::chrono::floor<std::chrono::days>(productionTestIntegration);
    }

    void initCommonTestValues(std::string timeStampStr)
    {
        initTestDateAndTimeStamp(timeStampStr);
        
    }

    void separateTestCaseOutput(std::string separatorString) noexcept
    {
        constexpr std::size_t lineWidth = 80;
        std::string testCaseSeparator;

        std::string horizontalLine;
        for (std::size_t i = 0; i <= lineWidth; ++i)
        {
            horizontalLine.append(separatorString);
        }

        testCaseSeparator = std::format("{}\n\n{}", horizontalLine, horizontalLine);

        std::cout << testCaseSeparator << std::endl;
    }
}
