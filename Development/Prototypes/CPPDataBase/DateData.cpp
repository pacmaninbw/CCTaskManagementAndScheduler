#include "DateData.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>

DateData::DateData(std::string dateString)
: year{0}, month{0}, day{0}
{
    std::locale usEnglish("en_US.UTF-8");
    std::vector<std::string> legalFormats = {
        {"%B %d, %Y"},
        {"%m/%d/%Y"},
        {"%m-%d-%Y"},
        {"%Y-%m-%d"}
    };

    for (auto legalFormat: legalFormats)
    {
        std::tm dateFromString{};
        std::istringstream ss(dateString);
        ss.imbue(usEnglish);
        ss >> std::get_time(&dateFromString, legalFormat.c_str());
        if (!ss.fail())
        {
            translateFromStdTM(dateFromString);
            return;
        }
    }

}

DateData::DateData(std::time_t currentTime)
{
    std::tm* now = std::localtime(&currentTime);
    translateFromStdTM(*now);
}

void DateData::translateFromStdTM(std::tm someDate)
{
    year = someDate.tm_year + 1900;
    month = someDate.tm_mon + 1;
    day = someDate.tm_mday;
}

std::tm DateData::translateToStdTM()
{
    std::tm LinuxDate{};
    LinuxDate.tm_year = year - 1900;
    LinuxDate.tm_mon = month -1;
    LinuxDate.tm_mday = day;
    return LinuxDate;
}

std::string DateData::USEnglishDateUsingFormat(const char *format)
{
    std::locale usEnglish("en_US.UTF-8");
    std::tm dateConverter = translateToStdTM();
    std::stringstream ss;
    ss.imbue(usEnglish);
    ss << std::put_time(&dateConverter, format);

    return ss.str();
}
