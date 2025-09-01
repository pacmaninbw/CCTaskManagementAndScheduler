#ifndef BOOSTMYSQLDBINTERFACECORE_H_
#define BOOSTMYSQLDBINTERFACECORE_H_

#include <any>
#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include <optional>
#include <string>
#include <vector>

namespace NSBA = boost::asio;
namespace NSBM = boost::mysql;

class BoostDBInterfaceCore
{
public:
    BoostDBInterfaceCore();
    virtual ~BoostDBInterfaceCore() = default;
    std::string getAllErrorMessages() const noexcept { return errorMessages; };
    void initFormatOptions();

protected:
    std::string errorMessages;
    void prepareForRunQueryAsync();
    void appendErrorMessage(const std::string& newError) { errorMessages.append(newError); };
    NSBM::format_options getConnectionFormatOptsAsync();
    NSBA::awaitable<NSBM::format_options> coRoGetFormatOptions();

/*
 * All calls to runQueryAsync should be implemented within try blocks.
 */
    NSBM::results runQueryAsync(const std::string& query);
    NSBA::awaitable<NSBM::results> coRoutineExecuteSqlStatement(const std::string& query);
/*
 * To process TEXT fields that contain model fields.
 */
std::vector<std::string> explodeTextField(std::string const& textField) noexcept;
std::string implodeTextField(std::vector<std::string>& fields) noexcept;

/*
 * Date converters are located here because they will be used by multiple dependent classes.
 */
    NSBM::date stdchronoDateToBoostMySQLDate(const std::chrono::year_month_day& source) noexcept
    {
        std::chrono::sys_days tp = source;
        return NSBM::date(tp);
    };

    std::chrono::year_month_day boostMysqlDateTimeToChronoTimePoint(const NSBM::date& source) noexcept
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
    NSBM::connect_params dbConnectionParameters;
    bool verboseOutput;
    char delimiter;
    std::optional<NSBM::format_options> format_opts;
};

#endif // BOOSTMYSQLDBINTERFACECORE_H_


