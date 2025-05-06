#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include "InMemUser.h"
#include <string_view>

class InMemUser;

class DBInterface
{
public:
    DBInterface();
    ~DBInterface() = default;
    void addUser(InMemUser &user);
    void addUserTaskWithUserID(std::size_t userID, std::string_view description, std::size_t parentTask,
        unsigned int estimatedEffortHours, unsigned int priorityInAllTasks, std::string_view requiredDelivery,
        std::string_view scheduledStart);
    void addUserTaskWithLoginName();

private:
    void stringViewValidationOrErrorMsg(std::string_view contents,
        std::string_view& strName, std::string& eMsg, unsigned int& errorCount) const;
    void throwExceptionIfNotValidAddUser(InMemUser& user);
    void appendStringConstantToSqlStmt(std::string &sqlStmt, std::string strConstant, bool addComma);
};

#endif // DBINTERFACE_H_