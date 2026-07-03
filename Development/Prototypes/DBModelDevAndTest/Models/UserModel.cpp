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
    m_Preferences.includePriorityInSchedule = true;
    m_Preferences.includeMinorPriorityInSchedule = true;
    m_Preferences.userLetterForMajorPriority = true;
    m_Preferences.separateMajorAndMinorWithDot = false;
    m_Preferences.startTime = "8:30 AM";
    m_Preferences.endTime = "5:00 PM";
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
    m_PrimaryKey = userID;
    m_LastName = lastName;
    m_FirstName = firstName;
    m_MiddleInitial = middleinit;
    m_Email = email;
    m_LoginName = loginName;
    m_Password = password;
    parsePrefenceText(preferences);
    m_Created = dateAdded;
    m_LastLogin = lastLogin;
    m_OrganizationId = orgId;
}



void UserModel::autoGenerateLoginAndPassword() noexcept
{
    if (m_LoginName.empty() && m_Password.empty())
    {
        createLoginBasedOnUserName(m_LastName, m_FirstName, m_MiddleInitial);
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
    m_Modified = true;
    m_LastName = lastName;
}

void UserModel::setFirstName(const std::string &firstName) noexcept
{
    m_Modified = true;
    m_FirstName = firstName;
}

void UserModel::setMiddleInitial(const std::string &middleinit) noexcept
{
    m_Modified = true;
    m_MiddleInitial = middleinit;
}

void UserModel::setEmail(const std::string &email) noexcept
{
    m_Modified = true;
    m_Email = email;
}

void UserModel::setLoginName(const std::string &loginName) noexcept
{
    m_Modified = true;
    m_LoginName = loginName;
}

void UserModel::setPassword(const std::string &password) noexcept
{
    m_Modified = true;
    m_Password = password;
}

void UserModel::setStartTime(const std::string &startTime) noexcept
{
    m_Modified = true;
    m_Preferences.startTime = startTime;
}

void UserModel::setEndTime(const std::string &endTime) noexcept
{
    m_Modified = true;
    m_Preferences.endTime = endTime;
}

void UserModel::setPriorityInSchedule(bool inSchedule) noexcept
{
    m_Modified = true;
    m_Preferences.includePriorityInSchedule = inSchedule;
}

void UserModel::setMinorPriorityInSchedule(bool inSchedule) noexcept
{
    m_Modified = true;
    m_Preferences.includeMinorPriorityInSchedule = inSchedule;
}

void UserModel::setUsingLettersForMaorPriority(bool usingLetters) noexcept
{
    m_Modified = true;
    m_Preferences.userLetterForMajorPriority = usingLetters;
}

void UserModel::setSeparatingPriorityWithDot(bool separate) noexcept
{
    m_Modified = true;
    m_Preferences.separateMajorAndMinorWithDot = separate;
}

void UserModel::setUserID(std::size_t UserID) noexcept
{
    m_Modified = true;
    m_PrimaryKey = UserID;
}

void UserModel::setCreationDate(std::chrono::system_clock::time_point dateAndTime) noexcept
{
    m_Modified = true;
    m_Created = dateAndTime;
}

void UserModel::setLastLogin(std::chrono::system_clock::time_point dateAndTime) noexcept
{
    m_Modified = true;
    m_LastLogin = dateAndTime;
}

bool UserModel::isMissingLastName() const noexcept
{
    return (m_LastName.empty() || m_LastName.length() < minNameLenght);
}

bool UserModel::isMissingFirstName() const noexcept
{
    return (m_FirstName.empty() || m_FirstName.length() < minNameLenght);
}

bool UserModel::isMissingLoginName()const noexcept
{
    return (m_LoginName.empty() || m_LoginName.length() < (2 * minNameLenght));
}

bool UserModel::isMissingPassword() const noexcept
{
    return (m_Password.empty() || m_Password.length() < minPasswordLenght);;
}

bool UserModel::isMissingEmail() const noexcept
{
    return (m_Email.empty() || m_Email.length() < minEmailLength);
}

bool UserModel::diffUser(UserModel &other) const noexcept
{
    // Ignore user preferences
    return (m_PrimaryKey == other.m_PrimaryKey && m_LoginName == other.m_LoginName && m_Password == other.m_Password &&
        m_LastName == other.m_LastName && m_FirstName == other.m_FirstName &&m_MiddleInitial == other.m_MiddleInitial);
}

void UserModel::initRequiredFields() noexcept
{
    m_MissingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingLastName, this), "Last Name"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingFirstName, this), "First Name"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingEmail, this), "Email"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingLoginName, this), "Login Name"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingPassword, this), "Password"});
}

std::string UserModel::formatInsertStatement()
{
    initFormatOptions();

    std::string insertStatement = boost::mysql::format_sql(format_opts.value(),
        "CALL AddUser({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7})",
        m_OrganizationId, m_LastName, m_FirstName, m_MiddleInitial, m_Email, m_LoginName, m_Password, buildPreferenceText()
    );

    return insertStatement;
}

std::string UserModel::formatUpdateStatement()
{
    initFormatOptions();

    std::string updateStatement = boost::mysql::format_sql(format_opts.value(),
        "CALL UpdateUserAllFields({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9})",
            m_PrimaryKey, m_OrganizationId, m_LastName, m_FirstName, m_MiddleInitial, m_Email, m_LoginName, m_Password, 
            buildPreferenceText(), optionalDateTimeConversion(m_LastLogin));
        
    return updateStatement;
}

std::string UserModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(), "CALL HideUser({})", m_PrimaryKey);
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


