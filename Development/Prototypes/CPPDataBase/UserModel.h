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

#endif // USERMODEL_H_


