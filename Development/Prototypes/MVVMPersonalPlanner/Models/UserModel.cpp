#include <exception>
#include "UserModel.h"
#include <stdexcept>
#include <string>

UserModel::UserModel()
: userID{0}, modified{false}
{
    preferences.includePriorityInSchedule = true;
    preferences.includeMinorPriorityInSchedule = true;
    preferences.userLetterForMajorPriority = true;
    preferences.separateMajorAndMinorWithDot = false;
    preferences.startTime = "8:30 AM";
    preferences.endTime = "5:00 PM";
}

UserModel::UserModel(
    std::string lastIn, std::string firstIn, std::string middleIIn, std::string emailIn,
    std::size_t uID, std::chrono::year_month_day dateAdded)
: UserModel()
{
    if (uID > 0)
    {
        setUserID(uID);
    }
    setLastName(lastIn);
    setFirstName(firstIn);
    setMiddleInitial(middleIIn);
    setEmail(emailIn);
    setCreationDate(dateAdded);
}

static constexpr std::size_t minNameLenght = 2;
static constexpr std::size_t minPasswordLenght = 8;

bool UserModel::hasRequiredValues() const noexcept
{
    // Each condition is an error, if any condition is true, then the fuction
    // should return false. If you add an error condition make sure to add it
    // to reportMissingFields() as well.
    return !((lastName.empty() || lastName.length() < minNameLenght) ||
        (firstName.empty() || firstName.length() < minNameLenght) ||
        (loginName.empty() || loginName.length() < (2 * minNameLenght)) ||
        (password.empty() || password.length() < minPasswordLenght) ||
        (!created.ok()));
}

std::string UserModel::reportMissingFields() const noexcept
{
    std::string missingFields;

    if (lastName.empty() || lastName.length() < minNameLenght)
    {
        missingFields.append("Missing Users Last Name.\n");
    }

    if (firstName.empty() || firstName.length() < minNameLenght)
    {
        missingFields.append("Missing Users First Name.\n");
    }

    if (loginName.empty() || loginName.length() < (2 * minNameLenght))
    {
        missingFields.append("Missing Users Login Name.\n");
    }

    if (password.empty() || password.length() < minPasswordLenght)
    {
        missingFields.append("Missing Users Password.\n");
    }

    if (!created.ok())
    {
        missingFields.append("Missing Users creation date.\n");
    }

    return missingFields;
}

void UserModel::autoGenerateLoginAndPassword()
{
    if (loginName.empty() && password.empty())
    {
        createLoginBasedOnUserName(lastName, firstName, middleInitial);
    }
}

void UserModel::createLoginBasedOnUserName(
    const std::string& lastName, const std::string& firstName, const std::string& middleInitial)
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

void UserModel::setLastName(const std::string &lastNameP)
{
    modified = true;
    lastName = lastNameP;
}

void UserModel::setFirstName(const std::string &firstNameP)
{
    modified = true;
    firstName = firstNameP;
}

void UserModel::setMiddleInitial(const std::string &middleinitP)
{
    modified = true;
    middleInitial = middleinitP;
}

void UserModel::setEmail(const std::string &emailP)
{
    modified = true;
    email = emailP;
}

void UserModel::setLoginName(const std::string &loginNameP)
{
    modified = true;
    loginName = loginNameP;
}

void UserModel::setPassword(const std::string &passwordP)
{
    modified = true;
    password = passwordP;
}

void UserModel::setStartTime(const std::string &startTime)
{
    modified = true;
    preferences.startTime = startTime;
}

void UserModel::setEndTime(const std::string &endTime)
{
    modified = true;
    preferences.endTime = endTime;
}

void UserModel::setPriorityInSchedule(bool inSchedule)
{
    modified = true;
    preferences.includePriorityInSchedule = inSchedule;
}

void UserModel::setMinorPriorityInSchedule(bool inSchedule)
{
    modified = true;
    preferences.includeMinorPriorityInSchedule = inSchedule;
}

void UserModel::setUsingLettersForMaorPriority(bool usingLetters)
{
    modified = true;
    preferences.userLetterForMajorPriority = usingLetters;
}

void UserModel::setSeparatingPriorityWithDot(bool separate)
{
    modified = true;
    preferences.separateMajorAndMinorWithDot = separate;
}

void UserModel::setUserID(std::size_t UserID)
{
    modified = true;
    userID = UserID;
}

void UserModel::setCreationDate(std::chrono::year_month_day dateIn)
{
    modified = true;
    created = dateIn;
}

void UserModel::setLastLogin(std::chrono::system_clock::time_point dateAndTime)
{
    modified = true;
    lastLogin = dateAndTime;
}

bool UserModel::diffUser(UserModel &other)
{
    // Ignore user preferences
    return (userID == other.userID && loginName == other.loginName && password == other.password &&
        lastName == other.lastName && firstName == other.firstName &&middleInitial == other.middleInitial &&
        created == other.created);
}

