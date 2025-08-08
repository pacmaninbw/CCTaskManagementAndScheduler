#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include "BoostDBInterfaceCore.h"
#include <exception>
#include <format>
#include <functional>
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
}

std::size_t UserDbInterface::insert(const UserModel &user)
{
    prepareForRunQueryAsync();

    try
    {
        NSBA::io_context ctx;
        NSBM::results localResult;

        NSBA::co_spawn(
            ctx, coRoInsertUser(user),
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
        selectStatementWhatArgs.push_back(std::any(userID));

        NSBM::results localResult = runQueryAsync(std::bind(&UserDbInterface::coRoSelectUserByID, this));

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
        selectStatementWhatArgs.push_back(std::any(lastName));
        selectStatementWhatArgs.push_back(std::any(firstName));
        selectStatementWhatArgs.push_back(std::any(middleI));

        NSBM::results localResult = runQueryAsync(std::bind(&UserDbInterface::coRoSelectUserByFullName, this));

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
        selectStatementWhatArgs.push_back(std::any(emailAddress));

        NSBM::results localResult = runQueryAsync(
            std::bind(&UserDbInterface::coRoSelectUserByEmailAddress, this));

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
        selectStatementWhatArgs.push_back(std::any(loginName));

        NSBM::results localResults = runQueryAsync(std::bind(&UserDbInterface::coRoSelectUserByLoginName, this));

        newUser = processResult(localResults);
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
        selectStatementWhatArgs.push_back(std::any(loginName));
        selectStatementWhatArgs.push_back(std::any(password));

        NSBM::results localResult = runQueryAsync(std::bind(&UserDbInterface::coRoSelectUserByLoginAndPassword, this));

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
        NSBM::results localResult = runQueryAsync(std::bind(&UserDbInterface::coRoSelectAllUsers, this));

        userList = processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserDbInterface::getUserByLoginName : {}", e.what()));
    }

    return userList;
}
/**/
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
    newUser->setStartTime(rv.at(StartDayIdx).as_string());
    newUser->setEndTime(rv.at(EndDayIdx).as_string());
    if (!rv.at(PriorityGroupIdx).is_null())
    {
        newUser->setPriorityInSchedule(rv.at(PriorityGroupIdx).as_int64());
    }
    if (!rv.at(PriorityIdx).is_null())
    {
        newUser->setMinorPriorityInSchedule(rv.at(PriorityIdx).as_int64());
    }
    if (!rv.at(UseLettersIdx).is_null())
    {
        newUser->setUsingLettersForMaorPriority(rv.at(UseLettersIdx).as_int64());
    }
    if (!rv.at(DotSeparationIdx).is_null())
    {
        newUser->setSeparatingPriorityWithDot(rv.at(DotSeparationIdx).as_int64());
    }

    // All the set functions set modified, since this user is new in memory it is not modified.
    newUser->clearModified();
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByID()
{
    std::size_t userID = std::any_cast<std::size_t>(selectStatementWhatArgs[0]);
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results result;

    co_await conn.async_execute(
        NSBM::with_params("SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
            "UseLettersForMajorPriority, SeparatePriorityWithDot FROM UserProfile WHERE UserID = {}", userID),
        result
    );

    co_await conn.async_close();

    co_return result;
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByFullName()
{
    std::string_view lastName = std::any_cast<std::string_view>(selectStatementWhatArgs[0]);
    std::string_view firstName = std::any_cast<std::string_view>(selectStatementWhatArgs[1]);
    std::string_view middleI = std::any_cast<std::string_view>(selectStatementWhatArgs[2]);

    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results result;

    co_await conn.async_execute(
        NSBM::with_params("SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
            "UseLettersForMajorPriority, SeparatePriorityWithDot FROM UserProfile WHERE LastName = {} AND FirstName = {} AND MiddleInitial = {}",
            lastName, firstName, middleI),
        result
    );

    co_await conn.async_close();

    co_return result;
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByEmailAddress()
{
    std::string_view emailAddr = std::any_cast<std::string_view>(selectStatementWhatArgs[0]);

    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results result;

    co_await conn.async_execute(
        NSBM::with_params("SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
            "UseLettersForMajorPriority, SeparatePriorityWithDot FROM UserProfile WHERE EmailAddress = {}", emailAddr),
        result
    );

    co_await conn.async_close();

    co_return result;
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByLoginName()
{
    std::string_view loginName = std::any_cast<std::string_view>(selectStatementWhatArgs[0]);

    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results result;

    co_await conn.async_execute(
        NSBM::with_params("SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
            "UseLettersForMajorPriority, SeparatePriorityWithDot FROM UserProfile WHERE LoginName = {}", loginName),
        result
    );

    co_await conn.async_close();

    co_return result;
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoInsertUser(const UserModel& user)
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results result;

    // Boolean values are stored as TINYINT and need to be converted.
    co_await conn.async_execute(
        NSBM::with_params("INSERT INTO UserProfile (LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
            "UseLettersForMajorPriority, SeparatePriorityWithDot) VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11})",
             user.getLastName(), user.getFirstName(), user.getMiddleInitial(), user.getEmail(), user.getLoginName(),
             user.getPassword(), user.getStartTime(), user.getEndTime(), static_cast<int>(user.isPriorityInSchedule()),
             static_cast<int>(user.isMinorPriorityInSchedule()), static_cast<int>(user.isUsingLettersForMaorPriority()),
             static_cast<int>(user.isSeparatingPriorityWithDot())),
        result
    );


    co_await conn.async_close();

    co_return result;
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectAllUsers()
{
    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results result;

    co_await conn.async_execute(
        "SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
            "UseLettersForMajorPriority, SeparatePriorityWithDot FROM UserProfile ORDER BY UserID",
        result
    );

    co_await conn.async_close();

    co_return result;
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByLoginAndPassword()
{
    std::string_view loginName = std::any_cast<std::string_view>(selectStatementWhatArgs[0]);
    std::string_view password = std::any_cast<std::string_view>(selectStatementWhatArgs[1]);

    NSBM::any_connection conn(co_await NSBA::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    NSBM::results result;

    co_await conn.async_execute(
        NSBM::with_params("SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
            "UseLettersForMajorPriority, SeparatePriorityWithDot FROM UserProfile WHERE LoginName = {} AND HashedPassWord = {}",
            loginName, password),
        result
    );

    co_await conn.async_close();

    co_return result;
}

