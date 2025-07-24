#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include <initializer_list>
#include <string>
#include "TaskModel.h"
#include "UserModel.h"
#include <utility>
#include <vector>

/*
 * Error Handling: 
 *
 * Prevent SQL errors if possible. 
 * 
 * Any errors in the content of a model object should be reported by appending to
 * the errorMessages string. Collect as many errors as possible. The user
 * interface will be responsible for the actual reporting of errors.
 * 
 * Errors from previous attemps to generate a SQL statement should be cleared
 * when starting a fresh SQL statement.
 * 
 * Exceptions should not be used if at all possible.
 * 
 */


class DBInterface
{
public:
    DBInterface(ProgramOptions& programOptions);
    virtual ~DBInterface() = default;
    std::string getAllErrorMessages() const { return errorMessages; };

protected:
    void clearPreviousErrors() { errorMessages.clear(); };
    void appendErrorMessage(std::string newError) { errorMessages.append(newError); };
    boost::asio::awaitable<boost::mysql::results> executeSqlStatementsCoRoutine(std::string selectSqlStatement);
    boost::mysql::results runAnyMySQLstatementsAsynchronously(std::string selectSqlStatement);
    boost::mysql::date convertChronoDateToBoostMySQLDate(std::chrono::year_month_day source)
    {
        std::chrono::sys_days tp = source;
        boost::mysql::date boostDate(tp);
        return boostDate;
    };
    std::chrono::year_month_day convertBoostMySQLDateToChornoDate(boost::mysql::date source)
    {
        const std::chrono::year year{source.year()};
        const std::chrono::month month{source.month()};
        const std::chrono::day day{source.day()};
        std::chrono::year_month_day converted{year, month, day};
        return converted;
    };

private:
    boost::mysql::connect_params dbConnectionParameters;
    std::string errorMessages;
    std::string databaseName;
    bool verboseOutput;
};

#endif // DBINTERFACE_H_


