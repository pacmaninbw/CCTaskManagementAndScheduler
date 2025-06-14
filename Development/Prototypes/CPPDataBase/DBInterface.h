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
    bool updateDatabaseTables(ModelBase* modelObject);
    std::string getAllErrorMessages() const { return errorMessages; };

private:
    bool ModelHasAllRequiredFields(ModelBase* modelObject);
    std::string generateSQLInsertStatement(ModelBase* modelObject);
    std::string generateSQLUpdateStatement(ModelBase* modelObject);
    void asyncExecutionSqlStatment(std::string sqlStmt);
    void clearPreviousErrors() { errorMessages.clear(); };
    void appendErrorMessage(std::string newError) { errorMessages.append(newError); };
    std::string booleanToString(bool boolArg) const { return boolArg? "TRUE" : "FALSE"; };
    std::string tableNameBasedonModelType(ModelBase *modelObject);

    std::string errorMessages;
    std::string tableName;
};

#endif // DBINTERFACE_H_


