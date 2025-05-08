#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include "UserModel.h"
#include "TaskModel.h"
#include <string>
#include <string_view>

class DBInterface
{
public:
    DBInterface(std::string table, std::string addStoredProcedure="");
    ~DBInterface() = default;
    void addUser(UserModel &user);
    void addTask(TaskModel &task);

protected:
    void asyncExecutionSqlStatment(std::string sqlStmt);
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
    std::string sqlStatement;


private:
    std::string tableName;
    std::string storedProcedureToAddToTable;
};

#endif // DBINTERFACE_H_