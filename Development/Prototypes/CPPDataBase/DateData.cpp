#include "DateData.h"
#include <iostream>

DateData::DateData()
: year{0}, month{0}, day{0}
{
}

DateData::DateData(std::string dateString)
: year{0}, month{0}, day{0}
{
    std::cerr << "\n\nDateData::DateData(" << dateString << ") NOT IMPLEMENTED!!!\n\n";
}

DateData::DateData(std::time_t currentTime)
{
    std::tm* now = std::localtime(&currentTime);

    year = now->tm_year + 1900;
    month = now->tm_mon + 1;
    day = now->tm_mday;
}

std::string DateData::getDBDateValue()
{
    if (year == 0 || month == 0 || day == 0)
    {
        std::string errString = "\n\nDate Not Set!!!\n\n";
        std::cerr << errString;
        return errString;
    }
    else
    {
        return std::to_string(year) + "-" + noSingleDigitInDate(month) +
            "-" + noSingleDigitInDate(day);
    }
}

std::string DateData::noSingleDigitInDate(unsigned int valueToConvert)
{
    return valueToConvert > 10? std::to_string(valueToConvert) :
        "0" + std::to_string(valueToConvert);
}

