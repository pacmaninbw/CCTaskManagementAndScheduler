#ifndef BOOSTMYSQLDBINTERFACECORE_H_
#define BOOSTMYSQLDBINTERFACECORE_H_

#include <any>
#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include <functional>
#include <string>
#include <string_view>

namespace NSBA = boost::asio;
namespace NSBM = boost::mysql;

class BoostDBInterfaceCore
{
public:
    BoostDBInterfaceCore();
    virtual ~BoostDBInterfaceCore() = default;
    std::string getAllErrorMessages() const { return errorMessages; };

protected:
    std::string errorMessages;
/*
 * Design decision. While putting the arguments for each select statement into a vector of std::any()
 * is not the most maintainable or safest way to program, it reduces the number of implementations of
 * runQueryAsync() where only the function signature and one line of code to call the select function
 * co-routine differ. This removed more than 200 lines of repetitive code.
 * Until I can figure out how to pass the results from boost::mysql::with_params() into a function,
 * this is necessary.
 */
    std::vector<std::any> selectStatementWhatArgs;
    void prepareForRunQueryAsync()
    {
        errorMessages.clear();
        selectStatementWhatArgs.clear();
    };
    void appendErrorMessage(std::string newError) { errorMessages.append(newError); };

/*
 * All calls to runQueryAsync should be implemented within try blocks.
 */
    NSBM::results runQueryAsync(std::function<NSBA::awaitable<NSBM::results>(void)>queryFunc);
/*
 * Special case, for functions called within another runQueryAsync() execution.
 */
    NSBM::results runQueryAsync(std::function<NSBA::awaitable<NSBM::results>(std::size_t)>queryFunc, std::size_t id);

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
    bool verboseOutput;
};

#endif // BOOSTMYSQLDBINTERFACECORE_H_


