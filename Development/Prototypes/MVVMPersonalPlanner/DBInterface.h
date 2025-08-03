#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include <functional>
#include <string>
#include <string_view>

namespace NSBA = boost::asio;
namespace NSBM = boost::mysql;

class DBInterface
{
public:
    DBInterface();
    virtual ~DBInterface() = default;
    std::string getAllErrorMessages() const { return errorMessages; };

protected:
    void clearPreviousErrors() { errorMessages.clear(); };
    void appendErrorMessage(std::string newError) { errorMessages.append(newError); };

/*
 * All calls to runQueryAsync should be implemented within try blocks.
 * Find the proper signature for the query, if one doesn't exist
 * add new signatures as necessary to allow for reuse.
 */
    NSBM::results runQueryAsync(
        std::function<NSBA::awaitable<NSBM::results>(std::size_t, std::chrono::year_month_day)>queryFunc,
        std::size_t id, std::chrono::year_month_day searchDate);
    NSBM::results runQueryAsync(
        std::function<NSBA::awaitable<NSBM::results>(std::size_t)>queryFunc, std::size_t id);
    NSBM::results runQueryAsync(
        std::function<NSBA::awaitable<NSBM::results>(std::string_view, std::string_view, std::string_view)>queryFunc,
        std::string_view searchStr1, std::string_view searchStr2, std::string_view searchStr3);
    NSBM::results runQueryAsync(
        std::function<NSBA::awaitable<NSBM::results>(std::string_view, std::string_view)>queryFunc,
        std::string_view searchStr1, std::string_view searchStr2);
    NSBM::results runQueryAsync(
        std::function<NSBA::awaitable<NSBM::results>(std::string_view)>queryFunc,
        std::string_view searchString);
    NSBM::results runQueryAsync(std::function<NSBA::awaitable<NSBM::results>(void)>queryFunc);
    NSBM::results runQueryAsync(
        std::function<NSBA::awaitable<NSBM::results>(std::string_view, std::size_t)>queryFunc,
        std::string_view searchStr, std::size_t id);

/*
 * Date converters are located here because they will be used by multiple dependent classes.
 */
    NSBM::date convertChronoDateToBoostMySQLDate(std::chrono::year_month_day source)
    {
        std::chrono::sys_days tp = source;
        NSBM::date boostDate(tp);
        return boostDate;
    };
    std::chrono::year_month_day convertBoostMySQLDateToChornoDate(NSBM::date source)
    {
        const std::chrono::year year{source.year()};
        const std::chrono::month month{source.month()};
        const std::chrono::day day{source.day()};
        std::chrono::year_month_day converted{year, month, day};
        return converted;
    };

protected:
    NSBM::connect_params dbConnectionParameters;
    std::string errorMessages;
    std::string databaseName;
    bool verboseOutput;
};

#endif // DBINTERFACE_H_


