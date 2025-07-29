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

UserModel::UserModel(std::string lastIn, std::string firstIn, std::string middleIIn, std::string emailIn, std::size_t uID)
: UserModel()
{
    lastName = lastIn;
    firstName = firstIn;
    middleInitial = middleIIn;
    email = emailIn;
    userID = uID;
    if (!uID)
    {
        modified = true;
    }
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

bool UserModel::diffUser(UserModel &other)
{
    // Ignore user preferences and password
    return (userID == other.userID && loginName == other.loginName &&
        lastName == other.lastName && firstName == other.firstName && middleInitial == other.middleInitial);
}

