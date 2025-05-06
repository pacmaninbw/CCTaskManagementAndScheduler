#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include "UserModel.h"
#include "TaskModel.h"
#include <string>

class DBInterface
{
public:
    DBInterface();
    ~DBInterface() = default;
    void addUser(UserModel &user);
    void addTask(TaskModel &task);

private:
    void appendStringConstantToSqlStmt(std::string &sqlStmt, std::string strConstant, bool addComma);
};

#endif // DBINTERFACE_H_