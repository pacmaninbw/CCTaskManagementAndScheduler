#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include "ModelBase.h"
#include <string>
#include <string_view>

class DBInterface
{
public:
    DBInterface(std::string table, std::string addStoredProcedure="");
    virtual ~DBInterface() = default;
    virtual bool addToDatabaseTable(ModelBase* modelObject);

protected:
    virtual bool ModelObjectHasAllRequiredFields(ModelBase* modelObject) = 0;
    virtual void asyncExecutionSqlStatment(std::string sqlStmt);
    void startAddStmt();
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

/*
 * Protected variables.
 */
    std::string sqlStatement;


private:
    std::string tableName;
    std::string storedProcedureToAddToTable;
};

#endif // DBINTERFACE_H_