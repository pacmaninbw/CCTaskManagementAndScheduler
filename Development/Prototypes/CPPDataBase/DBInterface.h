#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include "InMemUser.h"
#include "InMemUserTask.h"
#include <string>

class DBInterface
{
public:
    DBInterface();
    ~DBInterface() = default;
    void addUser(InMemUser &user);
    void addTask(InMemUserTask &task);

private:
    void appendStringConstantToSqlStmt(std::string &sqlStmt, std::string strConstant, bool addComma);
};

#endif // DBINTERFACE_H_