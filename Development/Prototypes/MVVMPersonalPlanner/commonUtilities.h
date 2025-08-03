#ifndef COMMONUTILITIES_H_
#define COMMONUTILITIES_H_
#include <chrono>

extern std::chrono::year_month_day getTodaysDate();
extern std::chrono::year_month_day getTodaysDatePlus(unsigned int offset);
extern std::chrono::year_month_day getTodaysDateMinus(unsigned int offset);

#endif // COMMONUTILITIES_H_
