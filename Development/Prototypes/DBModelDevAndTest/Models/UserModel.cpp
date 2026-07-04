// Project Header Files
#include "UserModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <vector>

UserModel::UserModel()
: ModelDBInterface("User", "UserID")
{
    m_preferences.includePriorityInSchedule = true;
    m_preferences.includeMinorPriorityInSchedule = true;
    m_preferences.userLetterForMajorPriority = true;
    m_preferences.separateMajorAndMinorWithDot = false;
    m_preferences.startTime = "8:30 AM";
    m_preferences.endTime = "5:00 PM";
}

UserModel::UserModel(
    std::size_t userID,
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
)
: UserModel()
{
    m_primaryKey = userID;
    m_lastName = lastName;
    m_firstName = firstName;
    m_middleInitial = middleinit;
    m_email = email;
    m_loginName = loginName;
    m_password = password;
    parsePrefenceText(preferences);
    m_created = dateAdded;
    m_lastLogin = lastLogin;
    m_organizationId = orgId;
}



void UserModel::autoGenerateLoginAndPassword() noexcept
{
    if (m_loginName.empty() && m_password.empty())
    {
        createLoginBasedOnUserName(m_lastName, m_firstName, m_middleInitial);
    }
}

void UserModel::createLoginBasedOnUserName(
    const std::string &lastName, const std::string &firstName, const std::string &middleInitial)  noexcept
{
    std::string tempLoginName(lastName);
    tempLoginName += firstName;
    if (middleInitial.size())
    {
        tempLoginName += middleInitial[0];
    }

    setLoginName(tempLoginName);
    setPassword(tempLoginName);
}

void UserModel::setLastName(const std::string &lastName) noexcept
{
    m_modified = true;
    m_lastName = lastName;
}

void UserModel::setFirstName(const std::string &firstName) noexcept
{
    m_modified = true;
    m_firstName = firstName;
}

void UserModel::setMiddleInitial(const std::string &middleinit) noexcept
{
    m_modified = true;
    m_middleInitial = middleinit;
}

void UserModel::setEmail(const std::string &email) noexcept
{
    m_modified = true;
    m_email = email;
}

void UserModel::setLoginName(const std::string &loginName) noexcept
{
    m_modified = true;
    m_loginName = loginName;
}

void UserModel::setPassword(const std::string &password) noexcept
{
    m_modified = true;
    m_password = password;
}

void UserModel::setStartTime(const std::string &startTime) noexcept
{
    m_modified = true;
    m_preferences.startTime = startTime;
}

void UserModel::setEndTime(const std::string &endTime) noexcept
{
    m_modified = true;
    m_preferences.endTime = endTime;
}

void UserModel::setPriorityInSchedule(bool inSchedule) noexcept
{
    m_modified = true;
    m_preferences.includePriorityInSchedule = inSchedule;
}

void UserModel::setMinorPriorityInSchedule(bool inSchedule) noexcept
{
    m_modified = true;
    m_preferences.includeMinorPriorityInSchedule = inSchedule;
}

void UserModel::setUsingLettersForMaorPriority(bool usingLetters) noexcept
{
    m_modified = true;
    m_preferences.userLetterForMajorPriority = usingLetters;
}

void UserModel::setSeparatingPriorityWithDot(bool separate) noexcept
{
    m_modified = true;
    m_preferences.separateMajorAndMinorWithDot = separate;
}

void UserModel::setUserID(std::size_t UserID) noexcept
{
    m_modified = true;
    m_primaryKey = UserID;
}

void UserModel::setCreationDate(std::chrono::system_clock::time_point dateAndTime) noexcept
{
    m_modified = true;
    m_created = dateAndTime;
}

void UserModel::setLastLogin(std::chrono::system_clock::time_point dateAndTime) noexcept
{
    m_modified = true;
    m_lastLogin = dateAndTime;
}

bool UserModel::isMissingLastName() const noexcept
{
    return (m_lastName.empty() || m_lastName.length() < minNameLenght);
}

bool UserModel::isMissingFirstName() const noexcept
{
    return (m_firstName.empty() || m_firstName.length() < minNameLenght);
}

bool UserModel::isMissingLoginName()const noexcept
{
    return (m_loginName.empty() || m_loginName.length() < (2 * minNameLenght));
}

bool UserModel::isMissingPassword() const noexcept
{
    return (m_password.empty() || m_password.length() < minPasswordLenght);;
}

bool UserModel::isMissingEmail() const noexcept
{
    return (m_email.empty() || m_email.length() < minEmailLength);
}

bool UserModel::diffUser(UserModel &other) const noexcept
{
    // Ignore user preferences
    return (m_primaryKey == other.m_primaryKey && m_loginName == other.m_loginName && m_password == other.m_password &&
        m_lastName == other.m_lastName && m_firstName == other.m_firstName &&m_middleInitial == other.m_middleInitial);
}

void UserModel::initRequiredFields() noexcept
{
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingLastName, this), "Last Name"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingFirstName, this), "First Name"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingEmail, this), "Email"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingLoginName, this), "Login Name"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingPassword, this), "Password"});
}

std::string UserModel::formatInsertStatement()
{
    initFormatOptions();

    std::string insertStatement = boost::mysql::format_sql(m_formatOpts.value(),
        "CALL AddUser({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7})",
        m_organizationId, m_lastName, m_firstName, m_middleInitial, m_email, m_loginName, m_password, buildPreferenceText()
    );

    return insertStatement;
}

std::string UserModel::formatUpdateStatement()
{
    initFormatOptions();

    std::string updateStatement = boost::mysql::format_sql(m_formatOpts.value(),
        "CALL UpdateUserAllFields({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9})",
            m_primaryKey, m_organizationId, m_lastName, m_firstName, m_middleInitial, m_email, m_loginName, m_password, 
            buildPreferenceText(), optionalDateTimeConversion(m_lastLogin));
        
    return updateStatement;
}

std::string UserModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(m_formatOpts.value(), "CALL HideUser({})", m_primaryKey);
}

std::string UserModel::buildPreferenceText() noexcept
{
    std::vector<std::string> preferences;

    preferences.push_back(getStartTime());
    preferences.push_back(getEndTime());
    preferences.push_back(std::to_string(static_cast<int>(isPriorityInSchedule())));
    preferences.push_back(std::to_string(static_cast<int>(isMinorPriorityInSchedule())));
    preferences.push_back(std::to_string(static_cast<int>(isUsingLettersForMaorPriority())));
    preferences.push_back(std::to_string(static_cast<int>(isSeparatingPriorityWithDot())));

    return implodeTextField(preferences);
}

void UserModel::parsePrefenceText(std::string preferences) noexcept
{
    // If user data was altered using SQL in a seperate process then preferences may not
    // have values or enough values.
    if (preferences.empty())
    {
        return;
    }
    std::vector<std::string> subfields = explodeTextField(preferences);
    if (subfields.size() <= PrefUsingDotIdx)
    {
        return;
    }

    setStartTime(subfields[PrefDayStartIdx]);
    setEndTime(subfields[PrefDayEndIdx]);
    setPriorityInSchedule(std::stoi(subfields[PrefMajorPriorityIdx]));
    setMinorPriorityInSchedule(std::stoi(subfields[PrefMinorPriorityIdx]));
    setUsingLettersForMaorPriority(std::stoi(subfields[PrefUsingLetterIdx]));
    setSeparatingPriorityWithDot(std::stoi(subfields[PrefUsingDotIdx]));

    // Since we are using the setter functions we have to clear the modified flag.
    clearModified();
}


