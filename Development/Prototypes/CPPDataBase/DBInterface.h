#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include "TaskModel.h"
#include "UserModel.h"
#include <string>
#include <string_view>

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
    void asyncExecutionSqlStatment(std::string sqlStmt);
    void clearPreviousErrors() { errorMessages.clear(); };
    void appendErrorMessage(std::string newError) { errorMessages.append(newError); };

    std::string errorMessages;
};

#endif // DBINTERFACE_H_


