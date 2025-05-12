#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

/*
 * The model base header file is included to allow as generic a database
 * interface as possible. All SQL statements will be created and executed by
 * functions in this file. The derived model to database interface classes
 * are responsible for providing data verification and appending the data to
 * the SQL statements generated here.
 */
#include "ModelBase.h"
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
    DBInterface(std::string table, std::string addStoredProcedure="");
    virtual ~DBInterface() = default;
    virtual bool addToDatabaseTable(ModelBase* modelObject);
    std::string getAllErrorMessages() const { return errorMessages; };

protected:
    virtual bool ModelObjectHasAllRequiredFields(ModelBase* modelObject) = 0;
    virtual bool addDataToSqlStatement(ModelBase* modelObject) = 0;
    virtual void asyncExecutionSqlStatment(std::string sqlStmt);
    void reportIfError(bool condition, std::string errorMessage, bool &isValid)
    {
        if (condition)
        {
            appendErrorMessage(errorMessage);
            isValid = false;
        }
    }
    void startAddStmt();
    void clearPreviousErrors() { errorMessages = ""; };
    void appendErrorMessage(std::string newError) { errorMessages.append(newError); };
/*
 * To prevent SQL injection attacks all data input will be embedded between
 * single quotes.
 */
    void appendArgToSqlStmt(std::string arg, bool addComma)
    {
        sqlStatement += "'" + arg + "'";
        if (addComma)
        {
            sqlStatement += ", ";
        }
    };
    void appendArgToSqlStmt(int arg, bool addComma=false)
        {appendArgToSqlStmt(std::to_string(arg), addComma); };
    void appendArgToSqlStmt(unsigned int arg, bool addComma=false)
        {appendArgToSqlStmt(std::to_string(arg), addComma); };
    void appendArgToSqlStmt(double arg, bool addComma=false)
        {appendArgToSqlStmt(std::to_string(arg), addComma); };
    void appendArgToSqlStmt(std::size_t arg, bool addComma=false)
        {appendArgToSqlStmt(std::to_string(arg), addComma); };
    void appendArgToSqlStmt(std::string_view arg, bool addComma=false)
        {appendArgToSqlStmt(std::string(arg), addComma); };
    bool requiredFieldHaseData(std::string arg) const { return (!arg.empty() && arg.size() > 0); };
    bool requiredKeyHasValue(std::size_t key) const { return key != 0; };

    void clearSqlStmt() { sqlStatement = ""; };

/*
 * Protected variables.
 */
    std::string sqlStatement;
    std::string errorMessages;


private:
    std::string tableName;
    std::string storedProcedureToAddToTable;
};

#endif // DBINTERFACE_H_


