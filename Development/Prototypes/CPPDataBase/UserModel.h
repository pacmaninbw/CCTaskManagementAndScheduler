#ifndef USERMODEL_H_
#define USERMODEL_H_

#include <iostream>
#include "ModelBase.h"
#include <string>

/*
 * UserSqlData is used as temporary transfer of data from the database.
 */
struct UserSqlData
{
    std::size_t UserID;
    std::string LastName;
    std::string FirstName;
    std::string MiddleInitial;
    std::string EmailAddress;
    std::string LoginName;
    std::string HashedPassWord;
    std::string ScheduleDayStart;
    std::string ScheduleDayEnd;
    bool IncludePriorityInSchedule;
    bool IncludeMinorPriorityInSchedule;
    bool UseLettersForMajorPriority;
    bool SeparatePriorityWithDot;
    friend std::ostream& operator<<(std::ostream& os, const UserSqlData& user)
    {
        os << "UserSqlData User\n";
        os << "\tUserID: " << user.UserID << "\n";
        os << "\tLast Name " << user.LastName << "\n";
        os << "\tFirst Name: " << user.FirstName << "\n";
        os << "\tMiddle Initial: " << user.MiddleInitial << "\n";
        os << "\tEMail Address: " << user.EmailAddress << "\n";
        os << "\tLoginName: " << user.LoginName << "\n";
        os << "\tHashed PassWord: " << user.HashedPassWord << "\n";
        os << "\tScheduleDayStart: " << user.ScheduleDayStart << "\n";
        os << "\tScheduleDayEnd: " << user.ScheduleDayEnd << "\n";
        os << "\tIncludePriorityInSchedule: " << user.IncludePriorityInSchedule << "\n";
        os << "\tIncludeMinorPriorityInSchedule: " << user.IncludeMinorPriorityInSchedule << "\n";
        os << "\tUseLettersForMajorPriority: " << user.UseLettersForMajorPriority << "\n";
        os << "\tSeparatePriorityWithDot: " << user.SeparatePriorityWithDot << "\n";

        return os;
    };
};

class UserModel : public ModelBase
{
public:
    UserModel();
    UserModel(const char* lastIn, const char* firstIn, const char* middleIIn, const char* email="");
    UserModel(const UserModel& original);
    UserModel(UserSqlData sqlData);
    ~UserModel() = default;
    std::string getLastName() const;
    std::string getFirstName() const;
    std::string getMiddleInitial() const;
    std::string getEmail() const;
    std::string getLoginName() const;
    std::string getPassword() const;
    std::string getStartTime() const;
    std::string getEndTime() const;
    std::size_t getUserID() const;
    bool isPriorityInSchedule() const;
    bool isMinorPriorityInSchedule() const;
    bool isUsingLettersForMaorPriority() const;
    bool isSeparatingPriorityWithDot() const;

    friend std::ostream& operator<<(std::ostream& os, const UserModel& obj)
    {
        os << obj.primaryKeyFieldName;
        os << "Column Names: ";
        for (const auto& [key, value] : obj.dataFields)
        {
            os << key;
            os << ", ";
        }
        os << "\n";
        for (const auto& [key, value] : obj.dataFields)
        {
            PTS_DataField_shp currentField = value;
            os << currentField->fieldInfo();
            os << "\n";
        }

        return os;
    };

private:
    void createLoginBasedOnUserName(std::string lastName, std::string firstName, std::string middleInitial);
};

using UserModel_shp = std::shared_ptr<UserModel>;
using UserList = std::vector<UserModel_shp>;

#endif // USERMODEL_H_


