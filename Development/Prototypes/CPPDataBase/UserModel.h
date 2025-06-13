#ifndef USERMODEL_H_
#define USERMODEL_H_

#include <iostream>
#include "ModelBase.h"
#include <string>

class UserModel : public ModelBase
{
public:
    UserModel();
    /*
     * Add new user from GUI
     */
    UserModel(const char* lastIn, const char* firstIn, const char* middleIIn, const char* email="");
    /*
     * Create user from database storeage, used on login for existing user.
     */
    UserModel(std::size_t userId, const char* dbLoginName, const char* dbPassWord, const char* dbLastName,
        const char* dbFirstName, const char* dbMiddleInit, const char* dbEmail, const char* dbDayStart,
        const char* dbDayEnd, int dbPriorityinSched, int dbMinorPriorityInSched, int dbLettersForMajorP,
        int dbUseDot);
    ~UserModel();
    std::string getLastName() const { return getStringFieldValue("LastName"); };
    std::string getFirstName() const { return getStringFieldValue("FirstName");  };
    std::string getMiddleInitial() const { return getStringFieldValue("MiddleInitial"); };
    std::string getEmail() const { return getStringFieldValue("emailAddress"); };
    std::string getLoginName() const {return getStringFieldValue("LoginName"); };
    std::string getPassword() const { return getStringFieldValue("HashedPassWord");  };
    std::string getStartTime() const { return "scheduleDayStart"; };
    std::string getEndTime() const { return "scheduleDayEnd"; };
    std::size_t getUserID() const { return getPrimaryKey(); };
    bool isPriorityInSchedule() const { return getBoolFieldValue("IncludePriorityInSchedule"); };
    bool isMinorPriorityInSchedule() const { return getBoolFieldValue("IncludeMinorPriorityInSchedule"); };
    bool isUsingLettersForMaorPriority() const { return getBoolFieldValue("UseLettersForMajorPriority"); };
    bool isSeparatingPriorityWithDot() const { return getBoolFieldValue("SeparatePriorityWithDot"); };
//    void dbSetUserId(std::size_t uID) { userID = uID; };

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
            PTS_DataField* currentField = value;
            os << currentField->fieldInfo();
            os << "\n";
        }

        return os;
    };

private:
    std::string convertTo24hours(std::string sechduledTime);
    void createLoginBasedOnUserName(std::string lastName, std::string firstName, std::string middleInitial);
};

#endif // USERMODEL_H_


