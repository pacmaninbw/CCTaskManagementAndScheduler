#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include "BoostDBInterfaceCore.h"
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include "UserDbInterface.h"
#include "UserModel.h"
#include <utility>

UserDbInterface::UserDbInterface()
: BoostDBInterfaceCore()
{
    delimiter = ';';
}

std::size_t UserDbInterface::insert(const UserModel &user)
{
    prepareForRunQueryAsync();

    try
    {
        NSBM::results localResult = runUpdateAsync(
            std::bind(&UserDbInterface::coRoInsertUser, this, std::placeholders::_1), user);

        return localResult.last_insert_id();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::insert : {}", e.what()));
        return 0;
    }
}

UserModel_shp UserDbInterface::getUserByUserID(std::size_t userID)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts);
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

UserModel_shp UserDbInterface::getUserByFullName(std::string_view lastName, std::string_view firstName, std::string_view middleI)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts);
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

UserModel_shp UserDbInterface::getUserByEmail(std::string_view emailAddress)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts);
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

UserModel_shp UserDbInterface::getUserByLoginName(std::string_view loginName)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts);
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

UserModel_shp UserDbInterface::getUserByLoginAndPassword(std::string_view loginName, std::string_view password)
{
    UserModel_shp newUser = nullptr;
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts);
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
        NSBM::format_context fctx(format_opts);
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

    try
    {
        NSBM::results localResult = runUpdateAsync(
            std::bind(&UserDbInterface::coRoUpdateUser, this, std::placeholders::_1), user);
            
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
NSBM::results UserDbInterface::runUpdateAsync(
    std::function<NSBA::awaitable<NSBM::results>(const UserModel &)> queryFunc,
    const UserModel &user
)
{
    NSBM::results localResult;
    NSBA::io_context ctx;

    NSBA::co_spawn(
        ctx, queryFunc(user),
        [&localResult, this](std::exception_ptr ptr, NSBM::results result)
        {
            if (ptr)
            {
                std::rethrow_exception(ptr);
            }
            localResult = std::move(result);
        }
    );

    ctx.run();

    return localResult;
}

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
    std::string preferences = rv.at(PasswordIdx).as_string();

    // All the set functions set modified, since this user is new in memory it is not modified.
    newUser->clearModified();
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoInsertUser(const UserModel& user)
{
    std::string preferences = buildPreferenceText(user);

    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results result;

    // Boolean values are stored as TINYINT and need to be converted.
    co_await conn.async_execute(
        NSBM::with_params("INSERT INTO UserProfile (LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, Preferences) VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6})",
             user.getLastName(), user.getFirstName(), user.getMiddleInitial(), user.getEmail(), user.getLoginName(),
             user.getPassword(), preferences),
        result
    );

    co_await conn.async_close();

    co_return result;
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoUpdateUser(const UserModel &user)
{
    std::string preferences = buildPreferenceText(user);

    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results result;

    // Boolean values are stored as TINYINT and need to be converted.
    co_await conn.async_execute(
        NSBM::with_params("UPDATE UserProfile SET"
                " UserProfile.LastName = {0},"
                " UserProfile.FirstName = {1},"
                " UserProfile.MiddleInitial = {2},"
                " UserProfile.EmailAddress = {3}," 
                " UserProfile.LoginName = {4},"
                " UserProfile.HashedPassWord = {5},"
                " UserProfile.Preferences = {6}"
            " WHERE UserProfile.UserID = {7}",
             user.getLastName(), user.getFirstName(), user.getMiddleInitial(), user.getEmail(), user.getLoginName(),
             user.getPassword(), preferences, user.getUserID()),
        result
    );


    co_await conn.async_close();

    co_return result;
}

/*
 * Rather than store user preferences separately in fields in the table, all
 * user preferences will be stored in a single TEXT field in the table. This
 * will allow expansion of preferences as needed while not needing to modify
 * the database.
 */
std::string UserDbInterface::buildPreferenceText(const UserModel &user)
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

void UserDbInterface::parsePrefenceText(std::string preferences, UserModel_shp newUser)
{
    std::vector<std::string> subfields = explodeTextField(preferences);

    newUser->setStartTime(subfields[PrefDayStartIdx]);
    newUser->setEndTime(subfields[PrefDayEndIdx]);
    newUser->setPriorityInSchedule(std::stoi(subfields[PrefMajorPriorityIdx]));
    newUser->setMinorPriorityInSchedule(std::stoi(subfields[PrefMinorPriorityIdx]));
    newUser->setUsingLettersForMaorPriority(std::stoi(subfields[PrefUsingLetterIdx]));
    newUser->setSeparatingPriorityWithDot(std::stoi(subfields[PrefUsingDotIdx]));
}
