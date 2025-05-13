#ifndef DATEDATA_H_
#define DATEDATA_H_

#include <ctime>
#include <string>

class DateData
{
public:
    DateData();
    DateData(unsigned int yearIn, unsigned int monthIn, unsigned int dayIn)
        : year{yearIn}, month{monthIn}, day{dayIn} {};
    DateData(std::string dateString);
    DateData(std::time_t currentTime);
    ~DateData() = default;
    std::string getDBDateValue();

private:
    std::string noSingleDigitInDate(unsigned int valueToConvert);

    unsigned int year;
    unsigned int month;
    unsigned int day;
};

#endif  // DATEDATA_H_