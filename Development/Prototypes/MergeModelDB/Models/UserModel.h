#ifndef USERMODEL_H_
#define USERMODEL_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
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
    ~UserModel() = default;

    void autoGenerateLoginAndPassword() noexcept;
    std::string getLastName() const noexcept { return lastName;};
    std::string getFirstName() const noexcept { return firstName; };
    std::string getMiddleInitial() const noexcept { return middleInitial; };
    std::string getEmail() const noexcept { return email; };
    std::string getLoginName() const noexcept { return loginName; };
    std::string getPassword() const noexcept { return password; };
    std::string getStartTime() const noexcept { return preferences.startTime; };
    std::string getEndTime() const noexcept { return preferences.endTime; };
    std::size_t getUserID() const noexcept { return primaryKey; };
    std::chrono::system_clock::time_point getCreationDate() const noexcept { return created.value(); };
    std::optional<std::chrono::system_clock::time_point> getLastLogin() const noexcept { return lastLogin; };
    bool isPriorityInSchedule() const noexcept { return preferences.includePriorityInSchedule; };
    bool isMinorPriorityInSchedule() const noexcept { return preferences.includeMinorPriorityInSchedule; };
    bool isUsingLettersForMaorPriority() const noexcept { return preferences.userLetterForMajorPriority; };
    bool isSeparatingPriorityWithDot() const noexcept { return preferences.separateMajorAndMinorWithDot; };

    void setLastName(const std::string& lastNameP) noexcept;
    void setFirstName(const std::string& firstNameP) noexcept;
    void setMiddleInitial(const std::string& middleinitP) noexcept;
    void setEmail(const std::string& emailP) noexcept;
    void setLoginName(const std::string& loginNameP) noexcept;
    void setPassword(const std::string& passwordP) noexcept;
    void setStartTime(const std::string& startTime) noexcept;
    void setEndTime(const std::string& endTime) noexcept;
    void setPriorityInSchedule(bool inSchedule) noexcept;
    void setMinorPriorityInSchedule(bool inSchedule) noexcept;
    void setUsingLettersForMaorPriority(bool usingLetters) noexcept;
    void setSeparatingPriorityWithDot(bool separate) noexcept;
    void setUserID(std::size_t UserID) noexcept;
    void setCreationDate(std::chrono::system_clock::time_point dateIn) noexcept;
    void setLastLogin(std::chrono::system_clock::time_point dateAndTime) noexcept;

/*
 * Select with arguments
 */
    bool selectByLoginName(const std::string_view& loginName) noexcept;
    bool selectByEmail(const std::string_view& emailAddress) noexcept;
    bool selectByLoginAndPassword(const std::string_view& loginName, const std::string_view& password) noexcept;
    bool selectByFullName(const std::string_view& lastName, const std::string_view& firstName,
        const std::string_view& middleI) noexcept;
    std::string formatGetAllUsersQuery() noexcept;
    bool selectByUserID(std::size_t UserID)  noexcept;

/*
 * Required fields.
 */
    bool isMissingLastName() const noexcept;
    bool isMissingFirstName() const noexcept;
    bool isMissingLoginName() const noexcept;
    bool isMissingPassword() const noexcept;
    bool isMissingDateAdded() const noexcept;
    bool isMissingEmail() const noexcept;
    void initRequiredFields() noexcept override;

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
        if (user.inSelfTest)
        {
            os << std::format(outFmtStr, "PassWord", user.password);
        }
        if (user.created.has_value())
        {
            os << std::format(outFmtStr, "User Added", user.created.value());
        }
        if (user.lastLogin.has_value())
        {
            os << std::format(outFmtStr, "Last Login", user.lastLogin.value());
        }

        return os;
    };

protected:
    void createLoginBasedOnUserName(const std::string& lastName,
        const std::string& firstName,const std::string& middleInitial) noexcept;
    bool diffUser(UserModel& other) const noexcept;
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatSelectStatement() override;

    std::string buildPreferenceText() noexcept;
    void parsePrefenceText(std::string preferences) noexcept;
    void processResultRow(boost::mysql::row_view rv) override;
    
    std::string lastName;
    std::string firstName;
    std::string middleInitial;
    std::string email;
    std::string loginName;
    std::string password;
    UserPreferences preferences;
    std::optional<std::chrono::system_clock::time_point> created;
    std::optional<std::chrono::system_clock::time_point> lastLogin;

    const std::size_t minNameLenght = 2;
    const std::size_t minPasswordLenght = 8;
    const std::size_t minEmailLength = 10;

private:
/*
 * The indexes below are based on the following select statement, maintain this order.
 * baseQuery could be SELECT * FROM UserProfile, but this way the order of the columns
 * returned are known.
 */
    boost::mysql::constant_string_view baseQuery = 
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
