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

bool UserSelfTest::runSelfTest()
{
    inSelfTest = true;
    verboseOutput = true;

    bool allSelfTestsPassed = true;

    if (verboseOutput)
    {
        std::clog << "Running UserModel Self Test\n";
    }

    if (!testExceptionHandling())
    {
        std::clog << "Exception handling FAILED!\n";
        allSelfTestsPassed = false;
    }
    
    if (!testSave())
    {
        allSelfTestsPassed = false;
    }

    if (!testAccessorFunctionsPassed())
    {
        std::clog << "One or more get or set functions FAILED!\n";
        allSelfTestsPassed = false;
    }

    if (!diffTest())
    {
        allSelfTestsPassed = false;
    }
    
    if (verboseOutput)
    {
        std::clog << "Test Ouput: " << *this << "\n";
    }

    if (testAllInsertFailures() != TESTPASSED)
    {
        allSelfTestsPassed = false;
        std::clog << "Test of all insertion failures FAILED!\n";
    }

    if (!testTextFieldManipulation())
    {
        allSelfTestsPassed = false;
    }

    inSelfTest = false;
    verboseOutput = false;

    return allSelfTestsPassed;
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

bool UserSelfTest::testAccessorFunctionsPassed()
{
    selfTestResetAllValues();

    bool allAccessorFunctionsPassed = true;
    std::vector<std::function<bool(void)>> accessTests;
    accessTests.push_back({std::bind(&UserSelfTest::testUserIdAccesss, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testLastNameAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testFirstNameAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testMiddleInitialAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testEmailAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testLoginNameAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testPassWordAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testCreatedDateAcfcess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testLastLoginAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testStartTimeAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testEndTimeAccesss, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testIncludePriorityInScheduleAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testIncludeMinorPriorityInScheduleAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testUseLetterForMajorPriorityAccess, this)});
    accessTests.push_back({std::bind(&UserSelfTest::testSeparateMajorAndMinorWithDotAccess, this)});

    for (auto accessTest: accessTests)
    {
        if (!accessTest())
        {
            allAccessorFunctionsPassed = false;
        }
    }

    return allAccessorFunctionsPassed;
}

bool UserSelfTest::testUserIdAccesss() noexcept
{
    std::size_t testPrimaryKey = 31;

    return testAccessorFunctions<std::size_t>(testPrimaryKey, &primaryKey, "Primary Key",
        std::bind(&UserModel::setUserID, this, std::placeholders::_1),
        std::bind(&UserModel::getUserID, this));
}

bool UserSelfTest::testLastNameAccess() noexcept
{
    std::string testValue("AnyLastName");
    return testAccessorFunctions<std::string>(testValue, &lastName, "Last Name",
        std::bind(&UserModel::setLastName, this, std::placeholders::_1),
        std::bind(&UserModel::getLastName, this));
}

bool UserSelfTest::testFirstNameAccess() noexcept
{
    std::string testValue("AnyFirstName");
    return testAccessorFunctions<std::string>(testValue, &firstName, "First Name",
        std::bind(&UserModel::setFirstName, this, std::placeholders::_1),
        std::bind(&UserModel::getFirstName, this));
}

bool UserSelfTest::testMiddleInitialAccess() noexcept
{
    std::string testValue("A");
    return testAccessorFunctions<std::string>(testValue, &middleInitial, "Middle Initial",
        std::bind(&UserModel::setMiddleInitial, this, std::placeholders::_1),
        std::bind(&UserModel::getMiddleInitial, this));
}

bool UserSelfTest::testLoginNameAccess() noexcept
{
    std::string testValue("AnyLoginName");
    return testAccessorFunctions<std::string>(testValue, &loginName, "LoginName",
        std::bind(&UserModel::setLoginName, this, std::placeholders::_1),
        std::bind(&UserModel::getLoginName, this));
}

bool UserSelfTest::testPassWordAccess() noexcept
{
    std::string testValue("AnyPassword");
    return testAccessorFunctions<std::string>(testValue, &password, "Password",
        std::bind(&UserModel::setPassword, this, std::placeholders::_1),
        std::bind(&UserModel::getPassword, this));
}

bool UserSelfTest::testCreatedDateAcfcess() noexcept
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();
    return testTimeStampAccessorFunctions(testValue, &created, "Creation TimeStamp",
        std::bind(&UserModel::setCreationDate, this, std::placeholders::_1),
        std::bind(&UserModel::getCreationDate, this));
}

bool UserSelfTest::testLastLoginAccess() noexcept
{
    std::optional<std::chrono::system_clock::time_point> testValue = std::chrono::system_clock::now();
    return testOptionalAccessorFunctions<std::chrono::system_clock::time_point>(testValue, &lastLogin, "Last Login TimeStamp",
        std::bind(&UserModel::setLastLogin, this, std::placeholders::_1),
        std::bind(&UserModel::getLastLogin, this));
}

bool UserSelfTest::testEmailAccess() noexcept
{
    std::string testValue("AnyEmail");
    return testAccessorFunctions<std::string>(testValue, &email, "Email",
        std::bind(&UserModel::setEmail, this, std::placeholders::_1),
        std::bind(&UserModel::getEmail, this));
}

bool UserSelfTest::testStartTimeAccess() noexcept
{
    std::string testValue("4:30 AM");
    return testAccessorFunctions<std::string>(testValue, &preferences.startTime, "Start Time Preference",
        std::bind(&UserModel::setStartTime, this, std::placeholders::_1),
        std::bind(&UserModel::getStartTime, this));
}

bool UserSelfTest::testEndTimeAccesss() noexcept
{
    std::string testValue("4:15 PM");
    return testAccessorFunctions<std::string>(testValue, &preferences.endTime, "End Time Preference",
        std::bind(&UserModel::setEndTime, this, std::placeholders::_1),
        std::bind(&UserModel::getEndTime, this));
}

bool UserSelfTest::testIncludePriorityInScheduleAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &preferences.includePriorityInSchedule, "Include Priority In Schedule",
        std::bind(&UserModel::setPriorityInSchedule, this, std::placeholders::_1),
        std::bind(&UserModel::isPriorityInSchedule, this));
}

bool UserSelfTest::testIncludeMinorPriorityInScheduleAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &preferences.includeMinorPriorityInSchedule, "Include Minor Priority In Schedule",
        std::bind(&UserModel::setMinorPriorityInSchedule, this, std::placeholders::_1),
        std::bind(&UserModel::isMinorPriorityInSchedule, this));
}

bool UserSelfTest::testUseLetterForMajorPriorityAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &preferences.userLetterForMajorPriority, "Use Letter For Major Priority Access",
        std::bind(&UserModel::setUsingLettersForMaorPriority, this, std::placeholders::_1),
        std::bind(&UserModel::isUsingLettersForMaorPriority, this));
}

bool UserSelfTest::testSeparateMajorAndMinorWithDotAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &preferences.separateMajorAndMinorWithDot, "Separate Major And Minor With Dot Access",
        std::bind(&UserModel::setSeparatingPriorityWithDot, this, std::placeholders::_1),
        std::bind(&UserModel::isSeparatingPriorityWithDot, this));
}

bool UserSelfTest::testExceptionHandling() noexcept
{
    selfTestResetAllValues();

    bool exceptionHandlingPassed = true;
    bool globalForceException = forceException;
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

    if (!forceExceptionsLoop(exceptionTests))
    {
        exceptionHandlingPassed = false;
    }

    forceException = globalForceException;

    return exceptionHandlingPassed;
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

ModelTestStatus UserSelfTest::testAllInsertFailures()
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

    std::clog << "UserSelfTest::testAllInsertFailures() before successful insert *this = " << *this << "\n";

    if (!insert())
    {
        std::clog << "In  UserSelfTest::testAllInsertFailures() Expected successful insert failed\n" << errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

bool UserSelfTest::diffTest()
{
    UserModel other;

    if (*this == other)
    {
        std::clog << "UserModel Diff FAILED!\n";
        return false;
    }

    other.setUserID(primaryKey);
    other.setLastName(lastName);
    other.setFirstName(firstName);
    other.setMiddleInitial(middleInitial);
    other.setLoginName(loginName);
    other.setPassword(password);

    return *this == other;
}
