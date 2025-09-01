#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "../CommandLineParser.h"
#include "BoostDBInterfaceCore.h"
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include "UserDbInterface.h"
#include "../Models/UserModel.h"
#include <utility>

UserDbInterface::UserDbInterface()
: BoostDBInterfaceCore()
{
    delimiter = ';';
}

std::size_t UserDbInterface::insert(const UserModel &user)
{
    std::size_t uID = 0;
    errorMessages.clear();

    if (user.isInDataBase())
    {
        appendErrorMessage("User already in Database, use Update!");
        return uID;
    }

    if (!user.isModified())
    {
        appendErrorMessage("User not modified!");
        return uID;
    }

    std::string missingFieldErrors;
    if (!user.hasRequiredValues(missingFieldErrors))
    {
        appendErrorMessage("User is missing required values!");
        appendErrorMessage(missingFieldErrors);
        return uID;
    }

    prepareForRunQueryAsync();

    try
    {
        std::string insertStatement = NSBM::format_sql(format_opts.value(),
            "INSERT INTO UserProfile (LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, UserAdded, LastLogin, Preferences) VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8})",
            user.getLastName(), user.getFirstName(), user.getMiddleInitial(), user.getEmail(), user.getLoginName(),
            user.getPassword(), stdchronoDateToBoostMySQLDate(user.getCreationDate()),
            optionalDateTimeConversion(user.getLastLogin()), buildPreferenceText(user)
        );

        NSBM::results localResult = runQueryAsync(insertStatement);

        uID = localResult.last_insert_id();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::insert : {}", e.what()));
    }

    return uID;
}

UserModel_shp UserDbInterface::getUserByUserID(std::size_t userID)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE UserID = {}", userID);

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        newUser = processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::getUserByUserID : {}", e.what()));
    }
    
    return newUser;
}

UserModel_shp UserDbInterface::getUserByFullName(
    const std::string_view& lastName, const std::string_view& firstName, const std::string_view& middleI
)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE LastName = {} AND FirstName = {} AND MiddleInitial = {}", lastName, firstName, middleI);

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        newUser = processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::getUserByFullName : {}", e.what()));
    }

    return newUser;
}

UserModel_shp UserDbInterface::getUserByEmail(const std::string_view& emailAddress)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE EmailAddress = {}", emailAddress);

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        newUser = processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::getUserByEmail : {}", e.what()));
    }

    return newUser;
}

UserModel_shp UserDbInterface::getUserByLoginName(const std::string_view& loginName)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE LoginName = {}", loginName);

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        newUser = processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::getUserByLoginName : {}", e.what()));
    }

    return newUser;
}

UserModel_shp UserDbInterface::getUserByLoginAndPassword(const std::string_view& loginName, const std::string_view& password)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE LoginName = {} AND HashedPassWord = {}", loginName, password);

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        newUser =  processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::getUserByLoginAndPassword : {}", e.what()));
    }

    return newUser;
}


UserList UserDbInterface::getAllUsers()
{
    UserList userList;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " ORDER BY UserID");

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());
        userList = processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::getUserByLoginName : {}", e.what()));
    }

    return userList;
}

bool UserDbInterface::update(const UserModel &user)
{
    prepareForRunQueryAsync();

    if (!user.isModified())
    {
        appendErrorMessage("User not modified!");
        return false;
    }

    try
    {
        std::string updateStatement = NSBM::format_sql(format_opts.value(),
            "UPDATE UserProfile SET"
                " UserProfile.LastName = {0},"
                " UserProfile.FirstName = {1},"
                " UserProfile.MiddleInitial = {2},"
                " UserProfile.EmailAddress = {3}," 
                " UserProfile.LoginName = {4},"
                " UserProfile.HashedPassWord = {5},"
                " UserProfile.Preferences = {6}"
            " WHERE UserProfile.UserID = {7}",
             user.getLastName(), user.getFirstName(), user.getMiddleInitial(), user.getEmail(), user.getLoginName(),
             user.getPassword(), buildPreferenceText(user), user.getUserID());

        NSBM::results localResult = runQueryAsync(updateStatement);
            
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::update : {}", e.what()));
        return false;
    }
}

/*
 * Private or Protected methods.
 */
UserModel_shp UserDbInterface::processResult(NSBM::results& results)
{
    if (results.rows().empty())
    {
        appendErrorMessage("User not found!");
        return nullptr;
    }

    if (results.rows().size() > 1)
    {
        appendErrorMessage("Too many users found to process!");
        return nullptr;
    }

    UserModel_shp newUser = std::make_shared<UserModel>(UserModel());
    NSBM::row_view rv = results.rows().at(0);
    processResultRow(rv, newUser);

    return newUser;
}

UserList UserDbInterface::processResults(NSBM::results& results)
{
    UserList users;

    if (results.rows().empty())
    {
        appendErrorMessage("No users found!");
        return users;
    }

    for (auto row: results.rows())
    {
        UserModel_shp newUser = std::make_shared<UserModel>(UserModel());
        processResultRow(row, newUser);
        users.push_back(newUser);
\
    }
    return users;
}

void UserDbInterface::processResultRow(NSBM::row_view rv, UserModel_shp newUser)
{
    newUser->setUserID(rv.at(UserIdIdx).as_uint64());
    newUser->setLastName(rv.at(LastNameIdx).as_string());
    newUser->setFirstName(rv.at(FirstNameIdx).as_string());
    newUser->setMiddleInitial(rv.at(MiddleInitialIdx).as_string());
    newUser->setEmail(rv.at(EmailAddressIdx).as_string());
    newUser->setLoginName(rv.at(LoginNameIdx).as_string());
    newUser->setPassword(rv.at(PasswordIdx).as_string());
    newUser->setCreationDate(boostMysqlDateTimeToChronoTimePoint(rv.at(UserAddedIdx).as_date()));
    std::string preferences = rv.at(PasswordIdx).as_string();
    if (!rv.at(LastLoginIdx).is_null())
    {
        newUser->setLastLogin(boostMysqlDateTimeToChronoTimePoint(rv.at(LastLoginIdx).as_datetime()));
    }


    // All the set functions set modified, since this user is new in memory it is not modified.
    newUser->clearModified();
}

/*
 * Rather than store user preferences separately in fields in the table, all
 * user preferences will be stored in a single TEXT field in the table. This
 * will allow expansion of preferences as needed while not needing to modify
 * the database.
 */
std::string UserDbInterface::buildPreferenceText(const UserModel &user) noexcept
{
    std::vector<std::string> preferences;

    preferences.push_back(user.getStartTime());
    preferences.push_back(user.getEndTime());
    preferences.push_back(std::to_string(static_cast<int>(user.isPriorityInSchedule())));
    preferences.push_back(std::to_string(static_cast<int>(user.isMinorPriorityInSchedule())));
    preferences.push_back(std::to_string(static_cast<int>(user.isUsingLettersForMaorPriority())));
    preferences.push_back(std::to_string(static_cast<int>(user.isSeparatingPriorityWithDot())));

    return implodeTextField(preferences);
}

void UserDbInterface::parsePrefenceText(std::string preferences, UserModel_shp newUser) noexcept
{
    std::vector<std::string> subfields = explodeTextField(preferences);

    newUser->setStartTime(subfields[PrefDayStartIdx]);
    newUser->setEndTime(subfields[PrefDayEndIdx]);
    newUser->setPriorityInSchedule(std::stoi(subfields[PrefMajorPriorityIdx]));
    newUser->setMinorPriorityInSchedule(std::stoi(subfields[PrefMinorPriorityIdx]));
    newUser->setUsingLettersForMaorPriority(std::stoi(subfields[PrefUsingLetterIdx]));
    newUser->setSeparatingPriorityWithDot(std::stoi(subfields[PrefUsingDotIdx]));
}
