// Project Header Files
#include "commonUtilities.h"
#include "UserSelfTest.h"

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

UserSelfTest::UserSelfTest()
{
}

void UserSelfTest::selfTestResetAllValues()
{
    ModelSelfTest::selfTestResetAllValues();

    lastName.clear();
    firstName.clear();
    middleInitial.clear();
    email.clear();
    loginName.clear();
    password.clear();
    created.reset();
    lastLogin.reset();
}

std::vector<std::function<TestStatus(void)>> UserSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();
    std::vector<std::function<TestStatus(void)>> attributeAccessTests;

    attributeAccessTests.push_back({std::bind(&UserSelfTest::testUserIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testLastNameAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testFirstNameAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testMiddleInitialAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testEmailAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testLoginNameAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testPassWordAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testCreatedDateAcfcess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testLastLoginAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testStartTimeAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testEndTimeAccesss, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testIncludePriorityInScheduleAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testIncludeMinorPriorityInScheduleAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testUseLetterForMajorPriorityAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserSelfTest::testSeparateMajorAndMinorWithDotAccess, this)});

    return attributeAccessTests;
}

TestStatus UserSelfTest::testUserIdAccesss() noexcept
{
    std::size_t testPrimaryKey = 31;

    return testAccessorFunctions<std::size_t>(testPrimaryKey, &primaryKey, "Primary Key",
        std::bind(&UserModel::setUserID, this, std::placeholders::_1),
        std::bind(&UserModel::getUserID, this));
}

TestStatus UserSelfTest::testLastNameAccess() noexcept
{
    std::string testValue("AnyLastName");
    return testAccessorFunctions<std::string>(testValue, &lastName, "Last Name",
        std::bind(&UserModel::setLastName, this, std::placeholders::_1),
        std::bind(&UserModel::getLastName, this));
}

TestStatus UserSelfTest::testFirstNameAccess() noexcept
{
    std::string testValue("AnyFirstName");
    return testAccessorFunctions<std::string>(testValue, &firstName, "First Name",
        std::bind(&UserModel::setFirstName, this, std::placeholders::_1),
        std::bind(&UserModel::getFirstName, this));
}

TestStatus UserSelfTest::testMiddleInitialAccess() noexcept
{
    std::string testValue("A");
    return testAccessorFunctions<std::string>(testValue, &middleInitial, "Middle Initial",
        std::bind(&UserModel::setMiddleInitial, this, std::placeholders::_1),
        std::bind(&UserModel::getMiddleInitial, this));
}

TestStatus UserSelfTest::testLoginNameAccess() noexcept
{
    std::string testValue("AnyLoginName");
    return testAccessorFunctions<std::string>(testValue, &loginName, "LoginName",
        std::bind(&UserModel::setLoginName, this, std::placeholders::_1),
        std::bind(&UserModel::getLoginName, this));
}

TestStatus UserSelfTest::testPassWordAccess() noexcept
{
    std::string testValue("AnyPassword");
    return testAccessorFunctions<std::string>(testValue, &password, "Password",
        std::bind(&UserModel::setPassword, this, std::placeholders::_1),
        std::bind(&UserModel::getPassword, this));
}

TestStatus UserSelfTest::testCreatedDateAcfcess() noexcept
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();
    return testTimeStampAccessorFunctions(testValue, &created, "Creation TimeStamp",
        std::bind(&UserModel::setCreationDate, this, std::placeholders::_1),
        std::bind(&UserModel::getCreationDate, this));
}

TestStatus UserSelfTest::testLastLoginAccess() noexcept
{
    std::optional<std::chrono::system_clock::time_point> testValue = std::chrono::system_clock::now();
    return testOptionalAccessorFunctions<std::chrono::system_clock::time_point>(testValue, &lastLogin, "Last Login TimeStamp",
        std::bind(&UserModel::setLastLogin, this, std::placeholders::_1),
        std::bind(&UserModel::getLastLogin, this));
}

TestStatus UserSelfTest::testEmailAccess() noexcept
{
    std::string testValue("AnyEmail");
    return testAccessorFunctions<std::string>(testValue, &email, "Email",
        std::bind(&UserModel::setEmail, this, std::placeholders::_1),
        std::bind(&UserModel::getEmail, this));
}

TestStatus UserSelfTest::testStartTimeAccess() noexcept
{
    std::string testValue("4:30 AM");
    return testAccessorFunctions<std::string>(testValue, &preferences.startTime, "Start Time Preference",
        std::bind(&UserModel::setStartTime, this, std::placeholders::_1),
        std::bind(&UserModel::getStartTime, this));
}

TestStatus UserSelfTest::testEndTimeAccesss() noexcept
{
    std::string testValue("4:15 PM");
    return testAccessorFunctions<std::string>(testValue, &preferences.endTime, "End Time Preference",
        std::bind(&UserModel::setEndTime, this, std::placeholders::_1),
        std::bind(&UserModel::getEndTime, this));
}

TestStatus UserSelfTest::testIncludePriorityInScheduleAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &preferences.includePriorityInSchedule, "Include Priority In Schedule",
        std::bind(&UserModel::setPriorityInSchedule, this, std::placeholders::_1),
        std::bind(&UserModel::isPriorityInSchedule, this));
}

TestStatus UserSelfTest::testIncludeMinorPriorityInScheduleAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &preferences.includeMinorPriorityInSchedule, "Include Minor Priority In Schedule",
        std::bind(&UserModel::setMinorPriorityInSchedule, this, std::placeholders::_1),
        std::bind(&UserModel::isMinorPriorityInSchedule, this));
}

TestStatus UserSelfTest::testUseLetterForMajorPriorityAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &preferences.userLetterForMajorPriority, "Use Letter For Major Priority Access",
        std::bind(&UserModel::setUsingLettersForMaorPriority, this, std::placeholders::_1),
        std::bind(&UserModel::isUsingLettersForMaorPriority, this));
}

TestStatus UserSelfTest::testSeparateMajorAndMinorWithDotAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &preferences.separateMajorAndMinorWithDot, "Separate Major And Minor With Dot Access",
        std::bind(&UserModel::setSeparatingPriorityWithDot, this, std::placeholders::_1),
        std::bind(&UserModel::isSeparatingPriorityWithDot, this));
}

std::vector<ExceptionTestElement> UserSelfTest::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionSelectByUserID, this), "selectByUserID"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionSelectByFullName, this), "selectByFullName"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionSelectByLoginName, this), "selectByLoginName"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionSelectByEmail, this), "selectByEmail"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionSelectByLoginAndPassword, this), "selectByLoginAndPassword"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionFormatGetAllUsersQuery, this), "formatGetAllUsersQuery"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionRetrieve, this), "testExceptionRetrieve"});

    return exceptionTests;
}

bool UserSelfTest::testExceptionSelectByLoginName() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("UserSelfTest::selectByLoginName", std::bind(&UserModel::selectByLoginName, this, std::placeholders::_1), "testValue");
}
bool UserSelfTest::testExceptionSelectByEmail() noexcept
{
    selfTestResetAllValues();
    std::string testEmail("testValue@testValue.com");

    return testExceptionAndSuccessNArgs("UserSelfTest::selectByEmail", std::bind(&UserModel::selectByEmail, this, std::placeholders::_1), testEmail);
}

bool UserSelfTest::testExceptionSelectByLoginAndPassword() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("UserModel::selectByLoginAndPassword", 
        std::bind(&UserModel::selectByLoginAndPassword, this, std::placeholders::_1, std::placeholders::_2),
        "TestValue", "TestValue");
}

bool UserSelfTest::testExceptionSelectByFullName() noexcept
{
    selfTestResetAllValues();

        return testExceptionAndSuccessNArgs("UserModel::selectByFullName", 
        std::bind(&UserModel::selectByFullName, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            "TestLastName", "TestFirstName", "MI");
}

bool UserSelfTest::testExceptionSelectByUserID() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("UserModel::selectByUserID", std::bind(&UserModel::selectByUserID, this, std::placeholders::_1), 1);
}

bool UserSelfTest::testExceptionFormatGetAllUsersQuery() noexcept
{
    selfTestResetAllValues();
    
    return testFormatExceptionAndSuccessNArgs("UserModel::formatGetAllUsersQuery", std::bind(&UserModel::formatGetAllUsersQuery, this));
}

bool UserSelfTest::testExceptionInsert() noexcept
{
    selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setLastName("LastName");
    setFirstName("FirstName");
    setMiddleInitial("M");
    setEmail("FirstName.LastName@LastName.com");
    autoGenerateLoginAndPassword();
    setCreationDate(timeStamp);
    setLastLogin(timeStamp);

    return testExceptionAndSuccessNArgs("UserModel::insert", std::bind(&UserModel::insert, this));
}

bool UserSelfTest::testExceptionUpdate() noexcept
{
    selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setUserID(1);
    setLastName("LastName");
    setFirstName("FirstName");
    setMiddleInitial("M");
    setEmail("FirstName.LastName@LastName.com");
    autoGenerateLoginAndPassword();
    setCreationDate(timeStamp);
    setLastLogin(timeStamp);

    return testExceptionAndSuccessNArgs("UserModel::update", std::bind(&UserModel::update, this));
}

bool UserSelfTest::testExceptionRetrieve() noexcept
{
    selfTestResetAllValues();

    setUserID(1);

    return testExceptionAndSuccessNArgs("UserModel::retrieve", std::bind(&UserModel::retrieve, this));
}

TestStatus UserSelfTest::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "Last Name", "First Name", "Email", "Login Name", "Password", "User is missing required values"
    };

    setUserID(0);   // Force a modification so that missing fields can be tested.

    std::vector<std::function<void(std::string)>> fieldSettings;
    fieldSettings.push_back(std::bind(&UserModel::setLastName, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&UserModel::setFirstName, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&UserModel::setEmail, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&UserModel::setLoginName, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&UserModel::setPassword, this, std::placeholders::_1));

    for (auto setField: fieldSettings)
    {
        if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
        {
            return TESTFAILED;
        }
        expectedErrors.erase(expectedErrors.begin());
        setField("teststringvalue");
    }

    expectedErrors.clear();

    if (verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", modelName, __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In  UserSelfTest::testAllInsertFailures() Expected successful insert failed\n" << errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus UserSelfTest::testEqualityOperator() noexcept
{
    UserModel other;

    other.setUserID(primaryKey);
    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;
    
    return (*this == other)? TESTPASSED: TESTFAILED;
}

void UserSelfTest::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

