#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include <cstdint>
#include <string_view>

using KeyType = uint64_t;

class DBInterface
{
public:
    DBInterface();
    ~DBInterface() = default;
    void addUser(std::string_view& lastName, std::string_view& firstName,
        std::string_view& middleInitial, std::string_view& loginName, std::string_view& hashedPassword);
    void addUserTaskWithUserID(KeyType userID, std::string_view description, KeyType parentTask,
        unsigned int estimatedEffortHours, unsigned int priorityInAllTasks, std::string_view requiredDelivery,
        std::string_view scheduledStart);
    void addUserTaskWithLoginName();

private:
    void stringViewValidationOrErrorMsg(std::string_view& contents,
        std::string_view& strName, std::string& eMsg, unsigned int& errorCount) const;
    void throwExceptionIfNotValidAddUser(std::string_view& lastName, std::string_view& firstName,
        std::string_view& loginName, std::string_view& hashedPassword);
};

#endif // DBINTERFACE_H_