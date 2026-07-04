#ifndef USERMODEL_H_
#define USERMODEL_H_

// Project Header Files
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

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
    UserModel(std::size_t userID,
        std::string lastName,
        std::string firstName,
        std::string middleinit,
        std::string email,
        std::string loginName,
        std::string password,
        std::string preferences,
        std::chrono::system_clock::time_point dateAdded,
        std::chrono::system_clock::time_point lastLogin,
        std::size_t orgId
    );
    ~UserModel() = default;

    void autoGenerateLoginAndPassword() noexcept;
    std::string getLastName() const noexcept { return m_lastName;};
    std::string getFirstName() const noexcept { return m_firstName; };
    std::string getMiddleInitial() const noexcept { return m_middleInitial; };
    std::string getEmail() const noexcept { return m_email; };
    std::string getLoginName() const noexcept { return m_loginName; };
    std::string getPassword() const noexcept { return m_password; };
    std::string getStartTime() const noexcept { return m_preferences.startTime; };
    std::string getEndTime() const noexcept { return m_preferences.endTime; };
    std::size_t getUserID() const noexcept { return m_primaryKey; };
    std::chrono::system_clock::time_point getCreationDate() const noexcept { return m_created.value(); };
    std::optional<std::chrono::system_clock::time_point> getLastLogin() const noexcept { return m_lastLogin; };
    bool isPriorityInSchedule() const noexcept { return m_preferences.includePriorityInSchedule; };
    bool isMinorPriorityInSchedule() const noexcept { return m_preferences.includeMinorPriorityInSchedule; };
    bool isUsingLettersForMaorPriority() const noexcept { return m_preferences.userLetterForMajorPriority; };
    bool isSeparatingPriorityWithDot() const noexcept { return m_preferences.separateMajorAndMinorWithDot; };

    void setLastName(const std::string& lastName) noexcept;
    void setFirstName(const std::string& firstName) noexcept;
    void setMiddleInitial(const std::string& middleinit) noexcept;
    void setEmail(const std::string& email) noexcept;
    void setLoginName(const std::string& loginName) noexcept;
    void setPassword(const std::string& password) noexcept;
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
 * Required fields.
 */
    bool isMissingLastName() const noexcept;
    bool isMissingFirstName() const noexcept;
    bool isMissingLoginName() const noexcept;
    bool isMissingPassword() const noexcept;
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
        os << std::format(outFmtStr, "User ID", user.m_primaryKey);
        os << std::format(outFmtStr, "Last Name", user.m_lastName);
        os << std::format(outFmtStr, "First Name", user.m_firstName);
        os << std::format(outFmtStr, "Middle Initial", user.m_middleInitial);
        os << std::format(outFmtStr, "Email", user.m_email);
        os << std::format(outFmtStr, "Login Name", user.m_loginName);
        if (user.m_selfTest)
        {
            os << std::format(outFmtStr, "PassWord", user.m_password);
        }
        if (user.m_created.has_value())
        {
            os << std::format(outFmtStr, "User Added", user.m_created.value());
        }
        if (user.m_lastLogin.has_value())
        {
            os << std::format(outFmtStr, "Last Login", user.m_lastLogin.value());
        }

        return os;
    };

protected:
    void createLoginBasedOnUserName(const std::string& lastName,
        const std::string& firstName,const std::string& middleInitial) noexcept;
    bool diffUser(UserModel& other) const noexcept;
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatDeleteStatement() override;

    std::string buildPreferenceText() noexcept;
    void parsePrefenceText(std::string preferences) noexcept;
    
    static const std::size_t minNameLenght = 2;
    static const std::size_t minPasswordLenght = 8;
    static const std::size_t minEmailLength = 10;

    std::string m_lastName;
    std::string m_firstName;
    std::string m_middleInitial;
    std::string m_email;
    std::string m_loginName;
    std::string m_password;
    UserPreferences m_preferences;
    std::optional<std::chrono::system_clock::time_point> m_created;
    std::optional<std::chrono::system_clock::time_point> m_lastLogin;
    std::optional<std::size_t> m_organizationId;

private:
// Preference subfield indexes
    static const std::size_t PrefDayStartIdx = 0;
    static const std::size_t PrefDayEndIdx = 1;
    static const std::size_t PrefMajorPriorityIdx = 2;
    static const std::size_t PrefMinorPriorityIdx = 3;
    static const std::size_t PrefUsingLetterIdx = 4;
    static const std::size_t PrefUsingDotIdx = 5;
};

using UserModel_shp = std::shared_ptr<UserModel>;

#endif // USERMODEL_H_
