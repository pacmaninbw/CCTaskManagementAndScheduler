#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "ModelBase.h"
#include <string>
#include <string_view>
#include "TaskModel.h"
#include "UserModel.h"

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
    DBInterface();
    virtual ~DBInterface() = default;
    std::string getAllErrorMessages() const { return errorMessages; };
    bool insertIntoDataBase(TaskModel &task);
    bool insertIntoDataBase(UserModel &user);
    UserModel_shp getUserByLogin(std::string loginName);
    UserModel_shp getUserByFullName(std::string LastName, std::string firstName, std::string middleInitial);
    TaskModel_shp getTaskByDescription(std::string description);
    UserList getAllUsers();
    TaskList getAllTasksForUser(UserModel_shp user);
    TaskList getAllTasksForUser(UserModel& user);

private:
    void clearPreviousErrors() { errorMessages.clear(); };
    void appendErrorMessage(std::string newError) { errorMessages.append(newError); };
    boost::mysql::date convertChronoDateToBoostMySQLDate(std::chrono::year_month_day source);
    std::chrono::year_month_day convertBoostMySQLDateToChornoDate(boost::mysql::date source);
    boost::asio::awaitable<void> getFormatOptionsFromDB();
    boost::asio::awaitable<boost::mysql::results> executeSqlStatementsCoRoutine(std::string selectSqlStatement);
    boost::mysql::results runAnyMySQLstatementsAsynchronously(std::string selectSqlStatement);
    void getOptionalTaskFields(TaskModel& task,std::optional<std::size_t>& parentTaskID, 
        std::optional<unsigned int>& status, std::optional<boost::mysql::date>& actualStart,
        std::optional<boost::mysql::date>& estimatedCompleteDate,
        std::optional<boost::mysql::date>& completeDate);
    std::string formatInsert(TaskModel& task);
    std::string formatInsert(UserModel& user);
    bool getFormatOptionsOnFirstFormatting();
    bool validateObjectAndSetUp(ModelBase& model);
    bool convertResultsToModel(boost::mysql::row_view& sourceFromDB, std::vector<std::string>& columnNames, Modelshp destination);
    bool executeSimpleQueryProcessResults(std::string sqlStatements, Modelshp destination);
    void convertScalarFieldValue(boost::mysql::field_view sourceField, PTS_DataField_shp currentFieldPtr);

    boost::mysql::connect_params dbConnectionParameters;
    boost::mysql::format_options dbFormatOptions;
    bool dbFormatOptionsAreSet = false;
    std::string errorMessages;
};

#endif // DBINTERFACE_H_


