#ifndef MODELDBINTERFACECORE_H_
#define MODELDBINTERFACECORE_H_

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace NSBA = boost::asio;
namespace NSBM = boost::mysql;

class ModelDBInterface
{
public:
    ModelDBInterface(std::string_view modelNameIn);
    virtual ~ModelDBInterface() = default;
    std::string getAllErrorMessages() const noexcept { return errorMessages; };
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

protected:
    void initFormatOptions();
/*
 * Each model will have 1 or more required fields, the model must specify what those
 * fields are.
 */
    virtual void initRequiredFields() = 0;
    void prepareForRunQueryAsync();
    void appendErrorMessage(const std::string& newError) { errorMessages.append(newError); errorMessages.append("\n");};
/*
 * Each model must provide formating for Insert, Update and Select by object ID.
 * Additional select statements will be handled by each model as necessary.
 */
    virtual std::string formatInsertStatement() = 0;
    virtual std::string formatUpdateStatement() = 0;
    virtual std::string formatSelectStatement() = 0;
    bool selectWithArguments(std::string formattedSelectStatement);
    virtual bool processResult(NSBM::results& results);
/*
 * Each model must provide the process by which the database information will
 * be translated into the specific model.
 */
    virtual void processResultRow(NSBM::row_view rv) = 0;


/*
 * All calls to runQueryAsync and getConnectionFormatOptsAsync should be
 * implemented within try blocks.
 */
    NSBM::results runQueryAsync(const std::string& query);
    NSBM::format_options getConnectionFormatOptsAsync();
    NSBA::awaitable<NSBM::results> coRoutineExecuteSqlStatement(const std::string& query);
    NSBA::awaitable<NSBM::format_options> coRoutineGetFormatOptions();
/*
 * To process TEXT fields that contain model fields.
 */
    std::vector<std::string> explodeTextField(std::string const& textField) noexcept;
    std::string implodeTextField(std::vector<std::string>& fields) noexcept;

    NSBM::date stdchronoDateToBoostMySQLDate(const std::chrono::year_month_day& source) noexcept
    {
        std::chrono::sys_days tp = source;
        return NSBM::date(tp);
    };

    std::chrono::year_month_day boostMysqlDateToChronoDate(const NSBM::date& source) noexcept
    {
        const std::chrono::year year{source.year()};
        const std::chrono::month month{source.month()};
        const std::chrono::day day{source.day()};
        return std::chrono::year_month_day{year, month, day};
    };

    NSBM::datetime stdChronoTimePointToBoostDateTime(std::chrono::system_clock::time_point source) noexcept
    {
        return NSBM::datetime(std::chrono::time_point_cast<boost::mysql::datetime::time_point::duration>(source));
    };

    std::chrono::system_clock::time_point boostMysqlDateTimeToChronoTimePoint(NSBM::datetime dbDateTime)
    {
        return std::chrono::time_point_cast<std::chrono::system_clock::time_point::duration>(dbDateTime.as_time_point());
    }

    std::optional<NSBM::date> optionalDateConversion(std::optional<std::chrono::year_month_day> optDate)
    {
        std::optional<NSBM::date> mySqlDate;

        if (optDate.has_value())
        {
            mySqlDate = stdchronoDateToBoostMySQLDate(optDate.value());
        }
        return mySqlDate;
    };

    std::optional<NSBM::datetime> optionalDateTimeConversion(std::optional<std::chrono::system_clock::time_point> optDateTime)
    {
        std::optional<NSBM::datetime> timeStamp;

        if (optDateTime.has_value())
        {
            timeStamp = stdChronoTimePointToBoostDateTime(optDateTime.value());
        }
        return timeStamp;
    };

protected:
    std::size_t primaryKey;
    std::string_view modelName;
    std::string errorMessages;
    bool modified;
    NSBM::connect_params dbConnectionParameters;
    bool verboseOutput;
    char delimiter;
    std::optional<NSBM::format_options> format_opts;

    struct RequireField
    {
        std::function<bool(void)>errorCondition;
        std::string fieldName;
    };
    std::vector<RequireField> missingRequiredFieldsTests;

};

#endif // MODELDBINTERFACECORE_H_


