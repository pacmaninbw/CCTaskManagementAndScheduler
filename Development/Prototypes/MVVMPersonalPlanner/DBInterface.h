#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include <functional>
#include <string>
#include <string_view>

namespace bAsio = boost::asio;
namespace bMysql = boost::mysql;

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
    bMysql::results runQueryAsync(
        std::function<bAsio::awaitable<bMysql::results>(std::size_t, std::chrono::year_month_day)>queryFunc,
        std::size_t id, std::chrono::year_month_day searchDate);
    bMysql::results runQueryAsync(
        std::function<bAsio::awaitable<bMysql::results>(std::size_t)>queryFunc, std::size_t id);
    bMysql::results runQueryAsync(
        std::function<bAsio::awaitable<bMysql::results>(std::string_view, std::string_view, std::string_view)>queryFunc,
        std::string_view searchStr1, std::string_view searchStr2, std::string_view searchStr3);
    bMysql::results runQueryAsync(
        std::function<bAsio::awaitable<bMysql::results>(std::string_view, std::string_view)>queryFunc,
        std::string_view searchStr1, std::string_view searchStr2);
    bMysql::results runQueryAsync(
        std::function<bAsio::awaitable<bMysql::results>(std::string_view)>queryFunc,
        std::string_view searchString);
    bMysql::results runQueryAsync(std::function<bAsio::awaitable<bMysql::results>(void)>queryFunc);
    bMysql::results runQueryAsync(
        std::function<bAsio::awaitable<bMysql::results>(std::string_view, std::size_t)>queryFunc,
        std::string_view searchStr, std::size_t id);

/*
 * Date converters are located here because they will be used by multiple dependent classes.
 */
    bMysql::date convertChronoDateToBoostMySQLDate(std::chrono::year_month_day source)
    {
        std::chrono::sys_days tp = source;
        bMysql::date boostDate(tp);
        return boostDate;
    };
    std::chrono::year_month_day convertBoostMySQLDateToChornoDate(bMysql::date source)
    {
        const std::chrono::year year{source.year()};
        const std::chrono::month month{source.month()};
        const std::chrono::day day{source.day()};
        std::chrono::year_month_day converted{year, month, day};
        return converted;
    };

protected:
    bMysql::connect_params dbConnectionParameters;
    std::string errorMessages;
    std::string databaseName;
    bool verboseOutput;
};

#endif // DBINTERFACE_H_


