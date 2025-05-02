#ifndef DBINTERFACE_H_
#define DBINTERFACE_H_

#include <string_view>

class DBInterface
{
public:
    DBInterface();
    ~DBInterface() = default;
    void addUser(std::string_view lastName, std::string_view firstName,
        std::string_view middleInitial, std::string_view loginName, std::string_view hashedPassword);
    void addUserTaskWithUserID();
    void addUserTaskWithLoginName();

private:
    void stringViewValidationOrErrorMsg(std::string_view& contents,
        std::string_view& strName, std::string& eMsg, unsigned int& errorCount) const;
};

#endif // DBINTERFACE_H_