#ifndef COMMONTESTVALUE_H_
#define COMMONTESTVALUE_H_

// Project Header Files
#include "TestStatus.h"

// Standard C++ Header Files
#include <chrono>
#include <string>

namespace common {
    extern std::chrono::system_clock::time_point TestTimeStampValue;
    extern std::chrono::year_month_day TestDateValue;
    extern std::chrono::year_month_day TestDateRangeStartValue;
    extern std::chrono::year_month_day TestDateRangeEndValue;
    extern std::chrono::year_month_day ProductionTestDataAddedDate;

    extern void initCommonTestValues(std::string testTimeStampStr);
}

#endif	// COMMONTESTVALUE_H_


