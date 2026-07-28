// Project Header Files
#include "QueryProcessor.h"
#include "UserQueryProcessor.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>
#include <memory>

UserQueryProcessor::UserQueryProcessor()
: QueryProcessor<UserModel, UserDbQueryValues>("User")
{
}

UserModelList UserQueryProcessor::getAllUsers() noexcept
{
    clearErrorMessages();

    UserModelList allUsers;

    try
    {
        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(boost::mysql::format_sql(
            getFormatOptions(), "SELECT * FROM user_profile"));
        allUsers = processStaticResults(localResult);
    }
        
    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::getAllUsers : {}", e.what()));
    }
    
    return allUsers;
}

UserModel_shp UserQueryProcessor::getUserByID(std::size_t userId) noexcept
{
    clearErrorMessages();
    UserModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_profile WHERE user_profile.user_id ={}", userId);
        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{}({}) : {}", __func__, userId, e.what()));
    }

    return found;
}

UserModel_shp UserQueryProcessor::getUserByLoginName(const std::string_view &loginName) noexcept
{
    clearErrorMessages();
    UserModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_profile WHERE user_profile.user_login = {}", loginName);
        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{}({}) : {}", __func__, loginName, e.what()));
    }

    return found;
}

UserModel_shp UserQueryProcessor::getUserByEmail(const std::string_view &emailAddress) noexcept
{
    clearErrorMessages();
    UserModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_profile WHERE user_profile.email_address = {}", emailAddress);
        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{}({}) : {}", __func__, emailAddress, e.what()));
    }

    return found;
}

UserModel_shp UserQueryProcessor::getUserByLoginAndPassword(const std::string_view &loginName, const std::string_view &password) noexcept
{
    clearErrorMessages();
    UserModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx,
            "SELECT * FROM user_profile WHERE user_profile.user_login = {}  AND user_profile.hashed_password = {}",
            loginName, password);
        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{}({}, {}) : {}", __func__, loginName, password, e.what()));
    }

    return found;
}

UserModel_shp UserQueryProcessor::getUserByFullName(
    const std::string_view &lastName,
    const std::string_view &firstName,
    const std::string_view &middleI) noexcept
{
    clearErrorMessages();
    UserModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE user_profile.last_name = {} ", lastName);
        boost::mysql::format_sql_to(fctx, "AND user_profile.first_name = {} ", firstName);
        boost::mysql::format_sql_to(fctx, "AND user_profile.middle_initial = {}", middleI);
        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{}({}, {}, {}) : {}", __func__, lastName, firstName, middleI, e.what()));
    }

    return found;
}

/*
 * Unit Test / Self Test
 */
std::vector<ListExceptionTestElement> UserQueryProcessor::initListExceptionTests() noexcept 
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionsGetAllUsers, this), "getAllUsers"});
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionGetUserById, this), "getUserByID"});
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionGetUserByLoginName, this), "getUserByLoginName"});
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionGetUserByEmail, this), "getUserByEmail"});
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionGetUserByLoginAndPassword, this), "getUserByLoginAndPassword"});
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionGetUserByFullName, this), "getUserByFullName"});

    return exceptionTests;
}

TestStatus UserQueryProcessor::testExceptionsGetAllUsers() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("UserQueryProcessorSelfTest::testExceptionsGetAllUsers()",
        std::bind(&UserQueryProcessor::getAllUsers, this));
}

TestStatus UserQueryProcessor::testExceptionGetUserById() noexcept
{
    selfTestResetAllValues();

    std::size_t testUser1 = 1;

    return testExceptionAndSuccessNArgs("UserQueryProcessor::testExceptionGetUserById()",
        std::bind(&UserQueryProcessor::getUserByID, this, std::placeholders::_1), testUser1);
}

TestStatus UserQueryProcessor::testExceptionGetUserByLoginName() noexcept
{
    selfTestResetAllValues();

    std::string testUser1("UserOne");

    return testExceptionAndSuccessNArgs("UserQueryProcessor::testExceptionGetUserByLoginName()",
        std::bind(&UserQueryProcessor::getUserByLoginName, this, std::placeholders::_1), testUser1);
}

TestStatus UserQueryProcessor::testExceptionGetUserByEmail() noexcept
{
    selfTestResetAllValues();

    std::string testUser1("UserOne@readyUserOne.com");

    return testExceptionAndSuccessNArgs("UserQueryProcessor::testExceptionGetUserByEmail()",
        std::bind(&UserQueryProcessor::getUserByEmail, this, std::placeholders::_1), testUser1);
}

TestStatus UserQueryProcessor::testExceptionGetUserByLoginAndPassword() noexcept
{
    selfTestResetAllValues();

    std::string testUser1("UserOne");
    std::string testPassword("testPassword");

    return testExceptionAndSuccessNArgs("UserQueryProcessor::testExceptionGetUserByLoginAndPassword()",
        std::bind(&UserQueryProcessor::getUserByLoginAndPassword, this, std::placeholders::_1, std::placeholders::_2),
        testUser1, testPassword);
}

TestStatus UserQueryProcessor::testExceptionGetUserByFullName() noexcept
{
    selfTestResetAllValues();

    std::string testFirstName("First");
    std::string testLastName("Last");
    std::string testMiddleI("middle");

    return testExceptionAndSuccessNArgs("UserQueryProcessor::testExceptionGetUserByFullName()",
        std::bind(&UserQueryProcessor::getUserByFullName, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3), testFirstName, testLastName, testMiddleI);
}

