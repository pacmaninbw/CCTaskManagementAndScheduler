// Project Header Files
#include "QueryProcessor.h"
#include "UserQueryProcessor.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>
#include <memory>

UserQueryProcessor::UserQueryProcessor()
: QueryProcessor<UserModel>(
    "User",
    {
        "UserID", "Organization_ID", "LastName", "FirstName", "MiddleInitial", 
        "EmailAddress", "LoginName", "HashedPassWord", "UserAdded", "LastLogin",
        "Preferences", "Hidden"
    }
)
{

}

UserModelList UserQueryProcessor::getAllUsers() noexcept
{
    clearErrorMessages();

    UserModelList allUsers;

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::results localResult = runQueryAsync(boost::mysql::format_sql(
            format_opts.value(), "CALL GetAllUsers()"));
        allUsers = processResults(localResult);
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetUserByID({})", userId);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        found = getOneResult(localResult);
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetUserByLoginName({})", loginName);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        found = getOneResult(localResult);
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetUserByEmail({})", emailAddress);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        found = getOneResult(localResult);
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetUserByLoginAndPassword({}, {})", loginName, password);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        found = getOneResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{}({}, {}) : {}", __func__, loginName, password, e.what()));
    }

    return found;
}

UserModel_shp UserQueryProcessor::getUserByFullName(const std::string_view &lastName, const std::string_view &firstName, const std::string_view &middleI) noexcept
{
    clearErrorMessages();
    UserModel_shp found = nullptr;

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetUserByFullName({}, {}, {})", lastName, firstName, middleI);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        found = getOneResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{}({}, {}, {}) : {}", __func__, lastName, firstName, middleI, e.what()));
    }

    return found;
}

UserModel_shp UserQueryProcessor::processResultRow(boost::mysql::row_view &queryRow)
{
    std::size_t userId = queryRow.at(m_UserIdIdx).as_uint64();
    std::string lastName = queryRow.at(m_LastNameIdx).as_string();
    std::string firstName = queryRow.at(m_FirstNameIdx).as_string();
    std::string middleInitial = queryRow.at(m_MiddleInitialIdx).as_string();
    std::string email = queryRow.at(m_EmailAddressIdx).as_string();
    std::string loginName = queryRow.at(m_LoginNameIdx).as_string();
    std::string password = queryRow.at(m_PasswordIdx).as_string();
    std::chrono::system_clock::time_point created = (boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_UserAddedIdx).as_datetime()));
    std::chrono::system_clock::time_point lastLogin;
    std::size_t organizationId = 0;
    if (!queryRow.at(m_LastLoginIdx).is_null())
    {
        lastLogin = boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_LastLoginIdx).as_datetime());
    }
    std::string preferences = queryRow.at(m_PreferencesIdx).as_string();
    if (!queryRow.at(m_OrganizationIdx).is_null())
    {
        organizationId = queryRow.at(m_OrganizationIdx).as_uint64();
    }

    return std::make_shared<UserModel>(userId, lastName, firstName, middleInitial, email, loginName, password, preferences, created, lastLogin, organizationId);
}

void UserQueryProcessor::fillRequiredIndexes()
{
    assignValueToIndex("UserID", m_UserIdIdx);
    assignValueToIndex("Organization_ID", m_OrganizationIdx);
    assignValueToIndex("LastName", m_LastNameIdx);
    assignValueToIndex("FirstName", m_FirstNameIdx);
    assignValueToIndex("MiddleInitial", m_MiddleInitialIdx);
    assignValueToIndex("EmailAddress", m_EmailAddressIdx);
    assignValueToIndex("LoginName", m_LoginNameIdx);
    assignValueToIndex("HashedPassWord", m_PasswordIdx);
    assignValueToIndex("UserAdded", m_UserAddedIdx);
    assignValueToIndex("LastLogin", m_LastLoginIdx);
    assignValueToIndex("Preferences", m_PreferencesIdx);
    assignValueToIndex("Hidden", m_HiddenIdx);
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

    return testListExceptionAndSuccessNArgs("UserQueryProcessorSelfTest::testExceptionsGetAllUsers()", std::bind(&UserQueryProcessor::getAllUsers, this));
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
        std::bind(&UserQueryProcessor::getUserByLoginAndPassword, this, std::placeholders::_1, std::placeholders::_2), testUser1, testPassword);
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
