#ifndef USERMODEL_H_
#define USERMODEL_H_

#include <iostream>
#include "ModelBase.h"
#include <string>

class UserModel : public ModelBase
{
public:
    UserModel();
    UserModel(std::string lastIn, std::string firstIn, std::string middleIIn, std::string email="");
    ~UserModel() = default;

    void autoGenerateLoginAndPassword();
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

    void setLastName(const std::string& lastName);
    void setFirstName(const std::string& firstName);
    void setMiddleInitial(const std::string& middleinit);
    void setEmail(const std::string& email);
    void setLoginName(const std::string& loginName);
    void setPassword(const std::string& password);
    void setStartTime(const std::string& startTime);
    void setEndTime(const std::string& endTime);
    void setPriorityInSchedule(bool inSchedule);
    void setMinorPriorityInSchedule(bool inSchedule);
    void setUsingLettersForMaorPriority(bool usingLetters);
    void setSeparatingPriorityWithDot(bool separate);


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
    void createLoginBasedOnUserName(const std::string& lastName,
        const std::string& firstName,const std::string& middleInitial);
};

using UserModel_shp = std::shared_ptr<UserModel>;
using UserList = std::vector<UserModel_shp>;

#endif // USERMODEL_H_


