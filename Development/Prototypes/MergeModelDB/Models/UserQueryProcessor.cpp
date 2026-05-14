// Project Header Files
#include "QueryProcessor.h"
#include "UserQueryProcessor.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>

UserQueryProcessor::UserQueryProcessor()
: QueryProcessor<UserModel>("User")
{
    requiredColumns =  {"UserID", "Organization_ID", "LastName", "FirstName", "MiddleInitial",  "EmailAddress", "LoginName", "HashedPassWord", "UserAdded", "LastLogin", "Preferences", "Hidden"};
    for (auto columnName: requiredColumns)
    {
        columnToIndexMap.push_back(columnName);
    }
}

UserModelList UserQueryProcessor::getAllUsers() noexcept
{
    errorMessages.clear();

    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUser : ");
    
    UserModelList allUsers;

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::results localResult = runQueryAsync(boost::mysql::format_sql(
            format_opts.value(), "SELECT * FROM UserProfile "));
        allUsers = processResults(localResult);
    }
        
    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::getAllUsers : {}", e.what()));
    }
    
    return allUsers;
}

UserModel_shp UserQueryProcessor::processResultRow(boost::mysql::row_view &queryRow)
{
    std::size_t userId = queryRow.at(UserIdIdx).as_uint64();
    std::string lastName = queryRow.at(LastNameIdx).as_string();
    std::string firstName = queryRow.at(FirstNameIdx).as_string();
    std::string middleInitial = queryRow.at(MiddleInitialIdx).as_string();
    std::string email = queryRow.at(EmailAddressIdx).as_string();
    std::string loginName = queryRow.at(LoginNameIdx).as_string();
    std::string password = queryRow.at(PasswordIdx).as_string();
    std::chrono::system_clock::time_point created = (boostMysqlDateTimeToChronoTimePoint(queryRow.at(UserAddedIdx).as_datetime()));
    std::chrono::system_clock::time_point lastLogin;
    std::size_t organizationId = 0;
    if (!queryRow.at(LastLoginIdx).is_null())
    {
        lastLogin = boostMysqlDateTimeToChronoTimePoint(queryRow.at(LastLoginIdx).as_datetime());
    }
    std::string preferences = queryRow.at(PreferencesIdx).as_string();
    if (!queryRow.at(OrganizationIdx).is_null())
    {
        organizationId = queryRow.at(OrganizationIdx).as_uint64();
    }

    return std::make_shared<UserModel>(userId, lastName, firstName, middleInitial, email, loginName, password, preferences, created, lastLogin, organizationId);
}

void UserQueryProcessor::fillRequiredIndexes()
{
    assignValueToIndex("UserID", UserIdIdx);
    assignValueToIndex("Organization_ID", OrganizationIdx);
    assignValueToIndex("LastName", LastNameIdx);
    assignValueToIndex("FirstName", FirstNameIdx);
    assignValueToIndex("MiddleInitial", MiddleInitialIdx);
    assignValueToIndex("EmailAddress", EmailAddressIdx);
    assignValueToIndex("LoginName", LoginNameIdx);
    assignValueToIndex("HashedPassWord", PasswordIdx);
    assignValueToIndex("UserAdded", UserAddedIdx);
    assignValueToIndex("LastLogin", LastLoginIdx);
    assignValueToIndex("Preferences", PreferencesIdx);
    assignValueToIndex("Hidden", HiddenIdx);
}

std::vector<ListExceptionTestElement> UserQueryProcessor::initListExceptionTests() noexcept 
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionsGetAllUsers, this), "selectByUserID"});

    return exceptionTests;
}

TestStatus UserQueryProcessor::testExceptionsGetAllUsers() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("UserQueryProcessorSelfTest::testExceptionsGetAllUsers()", std::bind(&UserQueryProcessor::getAllUsers, this));
}

