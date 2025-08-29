#ifndef BOOSTMYSQLDBINTERFACECORE_H_
#define BOOSTMYSQLDBINTERFACECORE_H_

#include <any>
#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "../CommandLineParser.h"
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
    NSBM::date convertChronoDateToBoostMySQLDate(const std::chrono::year_month_day& source) noexcept
    {
        std::chrono::sys_days tp = source;
        NSBM::date boostDate(tp);
        return boostDate;
    };
    std::chrono::year_month_day convertBoostMySQLDateToChornoDate(const NSBM::date& source) noexcept
    {
        const std::chrono::year year{source.year()};
        const std::chrono::month month{source.month()};
        const std::chrono::day day{source.day()};
        std::chrono::year_month_day converted{year, month, day};
        return converted;
    };

protected:
    NSBM::connect_params dbConnectionParameters;
    bool verboseOutput;
    char delimiter;
    std::optional<NSBM::format_options> format_opts;
};

#endif // BOOSTMYSQLDBINTERFACECORE_H_


