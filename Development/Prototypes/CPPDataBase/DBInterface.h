#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include <initializer_list>
#include "ModelBase.h"
#include "PTS_DataField.h"
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

 using WhereArg = std::pair<std::string, PTS_DataField>;

class DBInterface
{
public:
    DBInterface(ProgramOptions& programOptions);
    virtual ~DBInterface() = default;
    std::string getAllErrorMessages() const { return errorMessages; };
    bool insertIntoDataBase(ModelBase& model);
    bool getUniqueModelFromDB(ModelShp model, std::vector<WhereArg> whereArgs);
    bool getUniqueModelFromDB(ModelShp model, std::initializer_list<WhereArg> whereArgs)
    {
        std::vector<WhereArg> vWhereArgs{whereArgs};
        return getUniqueModelFromDB(model, vWhereArgs);
    };
    UserList getAllUsers();
    TaskList getAllTasksForUser(UserModel_shp user);
    TaskList getAllTasksForUser(UserModel& user);

private:
    void clearPreviousErrors() { errorMessages.clear(); };
    void appendErrorMessage(std::string newError) { errorMessages.append(newError); };
    std::string getTableNameFrom(ModelBase& model);
    std::string formatInsert(ModelBase& model);
    std::string formatSelect(std::string tableName, std::vector<WhereArg> whereArgs);
    boost::asio::awaitable<boost::mysql::results> executeSqlStatementsCoRoutine(std::string selectSqlStatement);
    boost::mysql::results runAnyMySQLstatementsAsynchronously(std::string selectSqlStatement);
    bool convertResultsToModel(boost::mysql::row_view& sourceFromDB, std::vector<std::string>& columnNames, ModelShp destination);
    void convertScalarFieldValue(boost::mysql::field_view sourceField, PTS_DataField_shp currentFieldPtr);
    bool executeSimpleQueryProcessResults(std::string sqlStatements, ModelShp destination);
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

    boost::mysql::connect_params dbConnectionParameters;
    std::string errorMessages;
    std::string databaseName;
    bool verboseOutput;
};

#endif // DBINTERFACE_H_


