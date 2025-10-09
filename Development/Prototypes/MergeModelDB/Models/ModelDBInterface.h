#ifndef MODELDBINTERFACECORE_H_
#define MODELDBINTERFACECORE_H_

// Project Header Files
#include "CoreDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class ModelDBInterface : public CoreDBInterface
{
public:
    ModelDBInterface(std::string_view modelNameIn);
    virtual ~ModelDBInterface() = default;
    bool save();
    bool insert();
    bool update();
    bool retrieve();    // Only select object by object ID.
    bool isInDataBase() const noexcept { return (primaryKey > 0); };
    bool isModified() const noexcept { return modified; };
    void clearModified() { modified = false; };
    bool hasRequiredValues();
    void reportMissingFields() noexcept;
    std::string_view getModelName() const { return modelName; };
    virtual std::size_t runSelfTest();

protected:
/*
 * Each model will have 1 or more required fields, the model must specify what those
 * fields are.
 */
    virtual void initRequiredFields() = 0;
/*
 * Each model must provide formating for Insert, Update and Select by object ID.
 * Additional select statements will be handled by each model as necessary.
 */
    virtual std::string formatInsertStatement() = 0;
    virtual std::string formatUpdateStatement() = 0;
    virtual std::string formatSelectStatement() = 0;
    virtual bool processResult(boost::mysql::results& results);
/*
 * Each model must provide the process by which the database information will
 * be translated into the specific model.
 */
    virtual void processResultRow(boost::mysql::row_view rv) = 0;

/*
 * To process TEXT fields that contain model fields.
 */
    std::vector<std::string> explodeTextField(std::string const& textField) noexcept;
    std::string implodeTextField(std::vector<std::string>& fields) noexcept;

    boost::mysql::date stdchronoDateToBoostMySQLDate(const std::chrono::year_month_day& source) noexcept
    {
        std::chrono::sys_days tp = source;
        return boost::mysql::date(tp);
    };

    std::chrono::year_month_day boostMysqlDateToChronoDate(const boost::mysql::date& source) noexcept
    {
        const std::chrono::year year{source.year()};
        const std::chrono::month month{source.month()};
        const std::chrono::day day{source.day()};
        return std::chrono::year_month_day{year, month, day};
    };

    boost::mysql::datetime stdChronoTimePointToBoostDateTime(std::chrono::system_clock::time_point source) noexcept
    {
        return boost::mysql::datetime(std::chrono::time_point_cast<boost::mysql::datetime::time_point::duration>(source));
    };

    std::chrono::system_clock::time_point boostMysqlDateTimeToChronoTimePoint(boost::mysql::datetime dbDateTime)
    {
        return std::chrono::time_point_cast<std::chrono::system_clock::time_point::duration>(dbDateTime.as_time_point());
    }

    std::optional<boost::mysql::date> optionalDateConversion(std::optional<std::chrono::year_month_day> optDate)
    {
        std::optional<boost::mysql::date> mySqlDate;

        if (optDate.has_value())
        {
            mySqlDate = stdchronoDateToBoostMySQLDate(optDate.value());
        }
        return mySqlDate;
    };

    std::optional<boost::mysql::datetime> optionalDateTimeConversion(std::optional<std::chrono::system_clock::time_point> optDateTime)
    {
        std::optional<boost::mysql::datetime> timeStamp;

        if (optDateTime.has_value())
        {
            timeStamp = stdChronoTimePointToBoostDateTime(optDateTime.value());
        }
        return timeStamp;
    };

protected:
    std::size_t primaryKey;
    std::string_view modelName;
    std::size_t failureCount;
    bool modified;
    char delimiter;
    struct RequireField
    {
        std::function<bool(void)>errorCondition;
        std::string fieldName;
    };
    std::vector<RequireField> missingRequiredFieldsTests;
};

#endif // MODELDBINTERFACECORE_H_


