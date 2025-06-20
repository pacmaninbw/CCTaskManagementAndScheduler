#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
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

private:
    void clearPreviousErrors() { errorMessages.clear(); };
    void appendErrorMessage(std::string newError) { errorMessages.append(newError); };
    boost::mysql::date convertChronoDateToBoostMySQLDate(std::chrono::year_month_day source);
    void getOptionalTaskFields(TaskModel& task,std::optional<std::size_t>& parentTaskID, 
        std::optional<unsigned int>& status, std::optional<boost::mysql::date>& actualStart,
        std::optional<boost::mysql::date>& estimatedCompleteDate,
        std::optional<boost::mysql::date>& completeDate);
    boost::asio::awaitable<void> coro_insert_task(TaskModel& task);
    boost::asio::awaitable<void> coro_insert_user(UserModel& user);
    std::string formatInsertTask(TaskModel& task);
    boost::asio::awaitable<void> getFormatOptionsFromDB();
    boost::mysql::format_options getDBFormatOptions();
    bool firstFormattedSqlStatement();

    boost::mysql::connect_params dbConnectionParameters;
    boost::mysql::format_options dbFormatOptions;
    bool dbFormatOptionsAreSet = false;
    std::string errorMessages;
};

#endif // DBINTERFACE_H_


