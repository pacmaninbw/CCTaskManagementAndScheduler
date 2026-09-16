// Project Header Files
#include "chronoBoostConversions.h"
#include "commonUtilities.h"
#include "OrganizationModel.h"
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

UserModelList UserQueryProcessor::getAllUsersAddedOn(std::chrono::year_month_day dateAdded) noexcept
{
    clearErrorMessages();

    UserModelList allUsersAddedOnDate;
    std::chrono::system_clock::time_point startSearch;
    std::chrono::system_clock::time_point endSearch;
    common::getHourRangeForDate(dateAdded, startSearch, endSearch);

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE user_profile.created_timestamp >= {} ",
            common::toBoostDateTime(startSearch));
        boost::mysql::format_sql_to(fctx, "AND user_profile.created_timestamp <= {} ",
            common::toBoostDateTime(endSearch));

        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(std::move(fctx).get().value());
        allUsersAddedOnDate = processStaticResults(localResult);
    }
        
    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return allUsersAddedOnDate;
}

UserModelList UserQueryProcessor::getAllUsersDeletedOn(std::chrono::year_month_day dateDeleted) noexcept
{
    clearErrorMessages();

    UserModelList allUsersDeletedOnDate;
    std::chrono::system_clock::time_point startSearch;
    std::chrono::system_clock::time_point endSearch;
    common::getHourRangeForDate(dateDeleted, startSearch, endSearch);

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE user_profile.last_modified_time_stamp >= {} ",
            common::toBoostDateTime(startSearch));
        boost::mysql::format_sql_to(fctx, "AND user_profile.last_modified_time_stamp <= {} ",
            common::toBoostDateTime(endSearch));
        boost::mysql::format_sql_to(fctx, "AND user_profile.deleted = 1");

        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(std::move(fctx).get().value());
        allUsersDeletedOnDate = processStaticResults(localResult);
    }
        
    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return allUsersDeletedOnDate;
}

UserModelList UserQueryProcessor::getAllUsersFrom(std::shared_ptr<OrganizationModel> organization) noexcept
{
    clearErrorMessages();

    UserModelList allUsersInOrganization;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE user_profile.id_organization = {} ", organization->getOrganizationId());
        boost::mysql::format_sql_to(fctx, "AND user_profile.deleted <> 1 ");

        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(std::move(fctx).get().value());
        allUsersInOrganization = processStaticResults(localResult);
    }
        
    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return allUsersInOrganization;
}

UserModelList UserQueryProcessor::getAllActiveUsers() noexcept
{
    clearErrorMessages();

    UserModelList allActiveUsers;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE user_profile.deleted <> 1 ");

        StaticQueryUser localResult = staticRunQueryAsync<UserDbQueryValues>(std::move(fctx).get().value());
        allActiveUsers = processStaticResults(localResult);
    }
        
    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return allActiveUsers;
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
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionGetAllUsersAddedOnDate, this), "getAllUsersAddedOnDate"});
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionGetAllUsersDeletedOnDate, this), "getAllUsersDeletedOnDate"});
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionGetAllUsersFrom, this), "getAllUsersFrom"});
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionGetAllActiveUsers, this), "getAllActiveUsers"});

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

TestStatus UserQueryProcessor::testExceptionGetAllUsersAddedOnDate() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day testDate = common::ProductionTestDataAddedDate;

    return testListExceptionAndSuccessNArgs("UserQueryProcessorSelfTest::testExceptionGetAllUsersAddedOnDate()",
        std::bind(&UserQueryProcessor::getAllUsersAddedOn, this, std::placeholders::_1), testDate);
}

TestStatus UserQueryProcessor::testExceptionGetAllUsersDeletedOnDate() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day testDate = common::ProductionTestDataAddedDate;

    return testListExceptionAndSuccessNArgs("UserQueryProcessorSelfTest::testExceptionGetAllUsersDeletedOnDate()",
        std::bind(&UserQueryProcessor::getAllUsersDeletedOn, this, std::placeholders::_1), testDate);
}

TestStatus UserQueryProcessor::testExceptionGetAllUsersFrom() noexcept
{
    selfTestResetAllValues();

    OrganizationModel_shp testOrganization = std::make_shared<OrganizationModel>();
    testOrganization->setOrganizationId(1);

    return testListExceptionAndSuccessNArgs("UserQueryProcessorSelfTest::testExceptionGetAllUsersFrom()",
        std::bind(&UserQueryProcessor::getAllUsersFrom, this, std::placeholders::_1), testOrganization);
}

TestStatus UserQueryProcessor::testExceptionGetAllActiveUsers() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("UserQueryProcessorSelfTest::testExceptionGetAllActiveUsers()",
        std::bind(&UserQueryProcessor::getAllActiveUsers, this));
}
