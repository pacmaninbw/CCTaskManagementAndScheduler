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
    clearPreviousErrors();

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
    clearPreviousErrors();

    try
    {
        NSBM::results localResult = runQueryAsync(
            std::bind(&UserDbInterface::coRoSelectUserByID, this, std::placeholders::_1),
            userID);

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
    clearPreviousErrors();

    try
    {
        NSBM::results localResult = runQueryAsync(
            std::bind(&UserDbInterface::coRoSelectUserByFullName, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            lastName, firstName, middleI);

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
    clearPreviousErrors();

    try
    {
        NSBM::results localResult = runQueryAsync(
            std::bind(&UserDbInterface::coRoSelectUserByEmailAddress, this, std::placeholders::_1),
            emailAddress);

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
    clearPreviousErrors();

    try
    {
        NSBM::results localResults = runQueryAsync(
            std::bind(&UserDbInterface::coRoSelectUserByLoginName, this, std::placeholders::_1),
            loginName);

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
    clearPreviousErrors();

    try
    {
        NSBM::results localResult = runQueryAsync(
            std::bind(&UserDbInterface::coRoSelectUserByLoginAndPassword, this, std::placeholders::_1, std::placeholders::_2),
            loginName, password);

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
    clearPreviousErrors();

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
    newUser->setUserID(rv.at(0).as_uint64());
    newUser->setLastName(rv.at(1).as_string());
    newUser->setFirstName(rv.at(2).as_string());
    newUser->setMiddleInitial(rv.at(3).as_string());
    newUser->setEmail(rv.at(4).as_string());
    newUser->setLoginName(rv.at(5).as_string());
    newUser->setPassword(rv.at(6).as_string());
    newUser->setStartTime(rv.at(7).as_string());
    newUser->setEndTime(rv.at(8).as_string());
    if (!rv.at(9).is_null())
    {
        newUser->setPriorityInSchedule(rv.at(9).as_int64());
    }
    if (!rv.at(10).is_null())
    {
        newUser->setMinorPriorityInSchedule(rv.at(10).as_int64());
    }
    if (!rv.at(11).is_null())
    {
        newUser->setUsingLettersForMaorPriority(rv.at(11).as_int64());
    }
    if (!rv.at(12).is_null())
    {
        newUser->setSeparatingPriorityWithDot(rv.at(12).as_int64());
    }

    // All the set functions set modified, since this user is new in memory it is not modified.
    newUser->clearModified();
}

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByID(std::size_t userID)
{
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

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByFullName(
    std::string_view lastName, std::string_view firstName, std::string_view middleI
)
{
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

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByEmailAddress(std::string_view emailAddr)
{
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

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByLoginName(std::string_view loginName)
{
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

NSBA::awaitable<NSBM::results> UserDbInterface::coRoSelectUserByLoginAndPassword(
    std::string_view loginName, std::string_view password)
{
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

