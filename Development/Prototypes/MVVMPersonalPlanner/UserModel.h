#ifndef USERMODEL_H_
#define USERMODEL_H_

#include <iostream>
#include <memory>
#include "ModelBase.h"
#include <string>

class UserModel : public ModelBase
{
public:
    struct UserPreferences
    {
        std::string startTime;
        std::string endTime;
        bool includePriorityInSchedule;
        bool includeMinorPriorityInSchedule;
        bool userLetterForMajorPriority;
        bool separateMajorAndMinorWithDot;
    };

    UserModel();
    UserModel(std::string lastIn, std::string firstIn, std::string middleIIn, std::string email="");
    ~UserModel() = default;

    void autoGenerateLoginAndPassword();
    std::string getLastName() const { return lastName;};
    std::string getFirstName() const { return firstName; };
    std::string getMiddleInitial() const { return middleInitial; };
    std::string getEmail() const { return email; };
    std::string getLoginName() const { return loginName; };
    std::string getPassword() const { return password; };
    std::string getStartTime() const { return preferences.startTime; };
    std::string getEndTime() const { return preferences.endTime; };
    std::size_t getUserID() const { return userID; };
    bool isPriorityInSchedule() const { return preferences.includePriorityInSchedule; };
    bool isMinorPriorityInSchedule() const { return preferences.includeMinorPriorityInSchedule; };
    bool isUsingLettersForMaorPriority() const { return preferences.userLetterForMajorPriority; };
    bool isSeparatingPriorityWithDot() const { return preferences.separateMajorAndMinorWithDot; };

    void setLastName(const std::string& lastNameP);
    void setFirstName(const std::string& firstNameP);
    void setMiddleInitial(const std::string& middleinitP);
    void setEmail(const std::string& emailP);
    void setLoginName(const std::string& loginNameP);
    void setPassword(const std::string& passwordP);
    void setStartTime(const std::string& startTime);
    void setEndTime(const std::string& endTime);
    void setPriorityInSchedule(bool inSchedule);
    void setMinorPriorityInSchedule(bool inSchedule);
    void setUsingLettersForMaorPriority(bool usingLetters);
    void setSeparatingPriorityWithDot(bool separate);


    friend std::ostream& operator<<(std::ostream& os, const UserModel& obj)
    {

        return os;
    };

private:
    void createLoginBasedOnUserName(const std::string& lastName,
        const std::string& firstName,const std::string& middleInitial);
    
    std::size_t userID;
    std::string lastName;
    std::string firstName;
    std::string middleInitial;
    std::string email;
    std::string loginName;
    std::string password;
    UserPreferences preferences;
    bool modified;
};

using UserModel_shp = std::shared_ptr<UserModel>;
using UserList = std::vector<UserModel_shp>;

#endif // USERMODEL_H_


