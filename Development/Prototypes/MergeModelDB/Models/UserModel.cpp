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
: ModelDBInterface("User")
{
    preferences.includePriorityInSchedule = true;
    preferences.includeMinorPriorityInSchedule = true;
    preferences.userLetterForMajorPriority = true;
    preferences.separateMajorAndMinorWithDot = false;
    preferences.startTime = "8:30 AM";
    preferences.endTime = "5:00 PM";
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

    if (isMissingDateAdded())
    {
        created = std::chrono::system_clock::now();
    }
    std::string insertStatement = boost::mysql::format_sql(format_opts.value(),
        "INSERT INTO UserProfile (LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
        "HashedPassWord, UserAdded, LastLogin, Preferences) VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8})",
        lastName, firstName, middleInitial, email, loginName, password,
        optionalDateTimeConversion(created),
        optionalDateTimeConversion(lastLogin), buildPreferenceText()
    );

    return insertStatement;
}

std::string UserModel::formatUpdateStatement()
{
    initFormatOptions();

    std::string updateStatement = boost::mysql::format_sql(format_opts.value(),
        "UPDATE UserProfile SET"
            " UserProfile.LastName = {0},"
            " UserProfile.FirstName = {1},"
            " UserProfile.MiddleInitial = {2},"
            " UserProfile.EmailAddress = {3}," 
            " UserProfile.LoginName = {4},"
            " UserProfile.HashedPassWord = {5},"
            " UserProfile.Preferences = {6},"
            " UserProfile.LastLogin = {7}"
        " WHERE UserProfile.UserID = {8}",
            lastName, firstName, middleInitial, email, loginName,password, 
            buildPreferenceText(), optionalDateTimeConversion(lastLogin), primaryKey);
        
    return updateStatement;
}

std::string UserModel::formatSelectStatement()
{
    initFormatOptions();

    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, baseQuery);
    boost::mysql::format_sql_to(fctx, " WHERE UserID = {}", primaryKey);

    return std::move(fctx).get().value();
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

void UserModel::processResultRow(boost::mysql::row_view rv)
{
    primaryKey = rv.at(UserIdIdx).as_uint64();
    lastName = rv.at(LastNameIdx).as_string();
    firstName = rv.at(FirstNameIdx).as_string();
    middleInitial = rv.at(MiddleInitialIdx).as_string();
    email = rv.at(EmailAddressIdx).as_string();
    loginName = rv.at(LoginNameIdx).as_string();
    password = rv.at(PasswordIdx).as_string();
    created = (boostMysqlDateTimeToChronoTimePoint(rv.at(UserAddedIdx).as_datetime()));
    if (!rv.at(LastLoginIdx).is_null())
    {
        lastLogin = boostMysqlDateTimeToChronoTimePoint(rv.at(LastLoginIdx).as_datetime());
    }
    std::string preferences = rv.at(PreferencesIdx).as_string();
    parsePrefenceText(preferences);
}

void UserModel::parsePrefenceText(std::string preferences) noexcept
{
    std::vector<std::string> subfields = explodeTextField(preferences);

    setStartTime(subfields[PrefDayStartIdx]);
    setEndTime(subfields[PrefDayEndIdx]);
    setPriorityInSchedule(std::stoi(subfields[PrefMajorPriorityIdx]));
    setMinorPriorityInSchedule(std::stoi(subfields[PrefMinorPriorityIdx]));
    setUsingLettersForMaorPriority(std::stoi(subfields[PrefUsingLetterIdx]));
    setSeparatingPriorityWithDot(std::stoi(subfields[PrefUsingDotIdx]));
    clearModified();
}

bool UserModel::selectByLoginName(const std::string_view &loginName) noexcept
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, baseQuery);
        boost::mysql::format_sql_to(fctx, " WHERE LoginName = {}", loginName);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserModel::selectByLoginName : {}", e.what()));
        return false;
    }
}

bool UserModel::selectByEmail(const std::string_view &emailAddress) noexcept
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, baseQuery);
        boost::mysql::format_sql_to(fctx, " WHERE EmailAddress = {}", emailAddress);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserModel::selectByEmail : {}", e.what()));
        return false;
    }
}

bool UserModel::selectByLoginAndPassword(const std::string_view &loginName, const std::string_view &password) noexcept
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, baseQuery);
        boost::mysql::format_sql_to(fctx, " WHERE LoginName = {} AND HashedPassWord = {}", loginName, password);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserModel::selectByLoginAndPassword : {}", e.what()));
        return false;
    }
}

bool UserModel::selectByFullName(const std::string_view &lastName, const std::string_view &firstName, const std::string_view &middleI) noexcept
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, baseQuery);
        boost::mysql::format_sql_to(fctx, " WHERE LastName = {} AND FirstName = {} AND MiddleInitial = {}", lastName, firstName, middleI);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserModel::selectByFullName : {}", e.what()));
        return false;
    }
}

std::string UserModel::formatGetAllUsersQuery() noexcept
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "SELECT UserID FROM UserProfile ");

        return std::move(fctx).get().value();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserModel::formatGetAllUsersQuery : {}", e.what()));
        return std::string();
    }
}

bool UserModel::selectByUserID(std::size_t UserID) noexcept
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, baseQuery);
        boost::mysql::format_sql_to(fctx, " WHERE UserID = {}", UserID);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserModel::selectByUserID : {}", e.what()));
        return false;
    }
}


