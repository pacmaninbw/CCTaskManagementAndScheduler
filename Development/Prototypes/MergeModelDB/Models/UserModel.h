#ifndef USERMODEL_H_
#define USERMODEL_H_

#include <chrono>
#include "commonUtilities.h"
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include "ModelDBInterface.h"
#include <optional>
#include <string>
#include <vector>

class UserModel : public ModelDBInterface
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
        std::size_t uID=0, std::chrono::year_month_day dateAdded=getTodaysDate()
    );
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
    std::size_t getUserID() const { return primaryKey; };
    std::chrono::year_month_day getCreationDate() const { return created; };
    std::optional<std::chrono::system_clock::time_point> getLastLogin() const { return lastLogin; };
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
    void setUserID(std::size_t UserID);
    void setCreationDate(std::chrono::year_month_day dateIn);
    void setLastLogin(std::chrono::system_clock::time_point dateAndTime);

/*
 * Select with arguments
 */
    bool selectByLoginName(const std::string_view& loginName);
    bool selectByEmail(const std::string_view& emailAddress);
    bool selectByLoginAndPassword(const std::string_view& loginName, const std::string_view& password);
    bool selectByFullName(const std::string_view& lastName, const std::string_view& firstName,
        const std::string_view& middleI);
    std::string formatGetAllUsersQuery();
    bool selectByUserID(std::size_t UserID);

/*
 * Required fields.
 */
    bool isMissingLastName();
    bool isMissingFirstName();
    bool isMissingLoginName();
    bool isMissingPassword();
    bool isMissingDateAdded();
    void initRequiredFields() override;

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
        os << std::format(outFmtStr, "User ID", user.primaryKey);
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
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatSelectStatement() override;

    std::string buildPreferenceText() noexcept;
    void parsePrefenceText(std::string preferences) noexcept;
    void processResultRow(NSBM::row_view rv) override;
    
    std::string lastName;
    std::string firstName;
    std::string middleInitial;
    std::string email;
    std::string loginName;
    std::string password;
    UserPreferences preferences;
    std::chrono::year_month_day created;
    std::optional<std::chrono::system_clock::time_point> lastLogin;

    const std::size_t minNameLenght = 2;
    const std::size_t minPasswordLenght = 8;

private:
/*
 * The indexes below are based on the following select statement, maintain this order.
 * baseQuery could be SELECT * FROM UserProfile, but this way the order of the columns
 * returned are known.
 */
    NSBM::constant_string_view baseQuery = 
        "SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, UserAdded, LastLogin, Preferences FROM UserProfile ";

    const std::size_t UserIdIdx = 0;
    const std::size_t LastNameIdx = 1;
    const std::size_t FirstNameIdx = 2;
    const std::size_t MiddleInitialIdx = 3;
    const std::size_t EmailAddressIdx = 4;
    const std::size_t LoginNameIdx = 5;
    const std::size_t PasswordIdx = 6;
    const std::size_t UserAddedIdx = 7;
    const std::size_t LastLoginIdx = 8;
    const std::size_t PreferencesIdx = 9;
// Preference subfield indexes
    const std::size_t PrefDayStartIdx = 0;
    const std::size_t PrefDayEndIdx = 1;
    const std::size_t PrefMajorPriorityIdx = 2;
    const std::size_t PrefMinorPriorityIdx = 3;
    const std::size_t PrefUsingLetterIdx = 4;
    const std::size_t PrefUsingDotIdx = 5;
};

using UserModel_shp = std::shared_ptr<UserModel>;

#endif // USERMODEL_H_
