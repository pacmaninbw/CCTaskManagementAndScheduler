#ifndef USERMODEL_H_
#define USERMODEL_H_

#include <chrono>
#include "commonUtilities.h"
#include <iostream>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class UserModel
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
    UserModel(
        std::string lastIn, std::string firstIn, std::string middleIIn, std::string emailIn="",
        std::size_t uID=0, std::chrono::year_month_day dateAdded=getTodaysDate());
    ~UserModel() = default;

    bool isInDataBase() const noexcept { return (userID > 0); };
    bool isModified() const noexcept { return modified; };
    bool hasRequiredValues() const noexcept;
    std::string reportMissingFields() const noexcept;
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
    std::chrono::year_month_day getCreationDate() const { return created; };
    std::optional<std::chrono::system_clock::time_point> getLastLogin() const { return lastLogin; };
    bool isPriorityInSchedule() const { return preferences.includePriorityInSchedule; };
    bool isMinorPriorityInSchedule() const { return preferences.includeMinorPriorityInSchedule; };
    bool isUsingLettersForMaorPriority() const { return preferences.userLetterForMajorPriority; };
    bool isSeparatingPriorityWithDot() const { return preferences.separateMajorAndMinorWithDot; };

    void clearModified() { modified = false; };
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
    void setUserID(std::size_t UserID);
    void setCreationDate(std::chrono::year_month_day dateIn);
    void setLastLogin(std::chrono::system_clock::time_point dateAndTime);

    bool operator==(UserModel& other)
    {
        return diffUser(other);
    };
    bool operator==(std::shared_ptr<UserModel> other)
    {
        return diffUser(*other);
    }

    friend std::ostream& operator<<(std::ostream& os, const UserModel& user)
    {
        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << std::format(outFmtStr, "User ID", user.userID);
        os << std::format(outFmtStr, "Last Name", user.lastName);
        os << std::format(outFmtStr, "First Name", user.firstName);
        os << std::format(outFmtStr, "Middle Initial", user.middleInitial);
        os << std::format(outFmtStr, "Email", user.email);
        os << std::format(outFmtStr, "Login Name", user.loginName);
        os << std::format(outFmtStr, "User Added", user.created);
        if (user.lastLogin.has_value())
        {
            os << std::format(outFmtStr, "Last Login", user.lastLogin.value());
        }

        return os;
    };

private:
    void createLoginBasedOnUserName(const std::string& lastName,
        const std::string& firstName,const std::string& middleInitial);
    bool diffUser(UserModel& other);
    
    std::size_t userID;
    std::string lastName;
    std::string firstName;
    std::string middleInitial;
    std::string email;
    std::string loginName;
    std::string password;
    UserPreferences preferences;
    std::chrono::year_month_day created;
    std::optional<std::chrono::system_clock::time_point> lastLogin;
    bool modified;
};

using UserModel_shp = std::shared_ptr<UserModel>;
using UserList = std::vector<UserModel_shp>;

#endif // USERMODEL_H_


