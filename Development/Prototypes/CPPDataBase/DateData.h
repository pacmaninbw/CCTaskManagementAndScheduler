#ifndef DATEDATA_H_
#define DATEDATA_H_

/*
 * Contains Date data as year, month and day. Dates are stored as actual dates 
 * and not as an offset from January 1, 1900.
 * Provides various formats for input and output. The primary form of output is
 * a MySQL date format. Currently only supports USA English formating of dates 
 * for both input and output. 
 */

#include <ctime>
#include <string>

class DateData
{
public:
    DateData() : year{0}, month{0}, day{0} {};
    DateData(unsigned int yearIn, unsigned int monthIn, unsigned int dayIn)
        : year{yearIn}, month{monthIn}, day{dayIn} {};
    DateData(std::string dateString);
    DateData(std::time_t currentTime);
    ~DateData() = default;
    std::string getDBDateValue() { return USEnglishDateUsingFormat("%Y-%Om-%Od"); };
    std::string getFormalUSEnglishDate() { return USEnglishDateUsingFormat("%B %d, %Y"); };
    std::string getUsEnglishDateWithSlashes() { return USEnglishDateUsingFormat("%m/%d/%Y"); };
    std::string getUsEnglishDateWithDashes() { return USEnglishDateUsingFormat("%m-%d-%Y"); };

private:
    void translateFromStdTM(std::tm someDate);
    std::tm translateToStdTM();
    std::string USEnglishDateUsingFormat(const char* format);

    unsigned int year;
    unsigned int month;
    unsigned int day;
};

#endif  // DATEDATA_H_