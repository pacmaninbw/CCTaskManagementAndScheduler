#ifndef COMMONTESTVALUE_H_
#define COMMONTESTVALUE_H_

// Project Header Files
#include "TestStatus.h"

// Standard C++ Header Files
#include <chrono>
#include <string>

extern std::chrono::system_clock::time_point commonTestTimeStampValue;
extern std::chrono::year_month_day commonTestDateValue;
extern std::chrono::year_month_day commonTestDateRangeStartValue;
extern std::chrono::year_month_day commonTestDateRangeEndValue;

extern void initCommonTestValues(std::string testTimeStampStr);

#endif	// COMMONTESTVALUE_H_


