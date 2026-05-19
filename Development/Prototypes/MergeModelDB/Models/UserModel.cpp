// Project Header Files
#include "commonUtilities.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

UserModel::UserModel()
: ModelDBInterface("User", "UserID")
{
    preferences.includePriorityInSchedule = true;
    preferences.includeMinorPriorityInSchedule = true;
    preferences.userLetterForMajorPriority = true;
    preferences.separateMajorAndMinorWithDot = false;
    preferences.startTime = "8:30 AM";
    preferences.endTime = "5:00 PM";
}

UserModel::UserModel(
    std::size_t userID,
    std::string lastNameP,
    std::string firstNameP,
    std::string middleinitP,
    std::string emailP,
    std::string loginNameP,
    std::string passwordP,
    std::string preferences,
    std::chrono::system_clock::time_point dateAdded,
    std::chrono::system_clock::time_point lastLoginP,
    std::size_t orgId
)
: UserModel()
{
    primaryKey = userID;
    lastName = lastNameP;
    firstName = firstNameP;
    middleInitial = middleinitP;
    email = emailP;
    loginName = loginNameP;
    password = passwordP;
    parsePrefenceText(preferences);
    created = dateAdded;
    lastLogin = lastLoginP;
    organizationId = orgId;
}



void UserModel::autoGenerateLoginAndPassword() noexcept
{
    if (loginName.empty() && password.empty())
    {
        createLoginBasedOnUserName(lastName, firstName, middleInitial);
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

void UserModel::setLastName(const std::string &lastNameP) noexcept
{
    modified = true;
    lastName = lastNameP;
}

void UserModel::setFirstName(const std::string &firstNameP) noexcept
{
    modified = true;
    firstName = firstNameP;
}

void UserModel::setMiddleInitial(const std::string &middleinitP) noexcept
{
    modified = true;
    middleInitial = middleinitP;
}

void UserModel::setEmail(const std::string &emailP) noexcept
{
    modified = true;
    email = emailP;
}

void UserModel::setLoginName(const std::string &loginNameP) noexcept
{
    modified = true;
    loginName = loginNameP;
}

void UserModel::setPassword(const std::string &passwordP) noexcept
{
    modified = true;
    password = passwordP;
}

void UserModel::setStartTime(const std::string &startTime) noexcept
{
    modified = true;
    preferences.startTime = startTime;
}

void UserModel::setEndTime(const std::string &endTime) noexcept
{
    modified = true;
    preferences.endTime = endTime;
}

void UserModel::setPriorityInSchedule(bool inSchedule) noexcept
{
    modified = true;
    preferences.includePriorityInSchedule = inSchedule;
}

void UserModel::setMinorPriorityInSchedule(bool inSchedule) noexcept
{
    modified = true;
    preferences.includeMinorPriorityInSchedule = inSchedule;
}

void UserModel::setUsingLettersForMaorPriority(bool usingLetters) noexcept
{
    modified = true;
    preferences.userLetterForMajorPriority = usingLetters;
}

void UserModel::setSeparatingPriorityWithDot(bool separate) noexcept
{
    modified = true;
    preferences.separateMajorAndMinorWithDot = separate;
}

void UserModel::setUserID(std::size_t UserID) noexcept
{
    modified = true;
    primaryKey = UserID;
}

void UserModel::setCreationDate(std::chrono::system_clock::time_point dateAndTime) noexcept
{
    modified = true;
    created = dateAndTime;
}

void UserModel::setLastLogin(std::chrono::system_clock::time_point dateAndTime) noexcept
{
    modified = true;
    lastLogin = dateAndTime;
}

bool UserModel::isMissingLastName() const noexcept
{
    return (lastName.empty() || lastName.length() < minNameLenght);
}

bool UserModel::isMissingFirstName() const noexcept
{
    return (firstName.empty() || firstName.length() < minNameLenght);
}

bool UserModel::isMissingLoginName()const noexcept
{
    return (loginName.empty() || loginName.length() < (2 * minNameLenght));
}

bool UserModel::isMissingPassword() const noexcept
{
    return (password.empty() || password.length() < minPasswordLenght);;
}

bool UserModel::isMissingDateAdded() const noexcept
{
    return !created.has_value();
}

bool UserModel::isMissingEmail() const noexcept
{
    return (email.empty() || email.length() < minEmailLength);
}

bool UserModel::diffUser(UserModel &other) const noexcept
{
    // Ignore user preferences
    return (primaryKey == other.primaryKey && loginName == other.loginName && password == other.password &&
        lastName == other.lastName && firstName == other.firstName &&middleInitial == other.middleInitial);
}

void UserModel::initRequiredFields() noexcept
{
    missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingLastName, this), "Last Name"});
    missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingFirstName, this), "First Name"});
    missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingEmail, this), "Email"});
    missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingLoginName, this), "Login Name"});
    missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingPassword, this), "Password"});
}

std::string UserModel::formatInsertStatement()
{
    initFormatOptions();

    std::string insertStatement = boost::mysql::format_sql(format_opts.value(),
        "CALL AddUser({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7})",
        organizationId, lastName, firstName, middleInitial, email, loginName, password, buildPreferenceText()
    );

    return insertStatement;
}

std::string UserModel::formatUpdateStatement()
{
    initFormatOptions();

    std::string updateStatement = boost::mysql::format_sql(format_opts.value(),
        "CALL UpdateUserAllFields({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9})",
            primaryKey, organizationId, lastName, firstName, middleInitial, email, loginName, password, 
            buildPreferenceText(), optionalDateTimeConversion(lastLogin));
        
    return updateStatement;
}

std::string UserModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(), "CALL HideUser({})", primaryKey);
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


