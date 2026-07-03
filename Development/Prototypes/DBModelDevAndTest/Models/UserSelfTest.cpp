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

TestStatus UserSelfTest::runSelfTest() noexcept
{
    m_SelfTest = true;
    TestStatus selfTestStatus = TESTPASSED;

    std::cout << "\nRunning " << m_ModelName << " Self Test\n";

    if (testExceptionHandling()!= TESTPASSED)
    {
        std::cerr  << m_ModelName << "::runSelfTest: Exception handling FAILED!\n";
        selfTestStatus = TESTFAILED;
    }
    
    if (testSave() == TESTFAILED)
    {
        selfTestStatus = TESTFAILED;
    }

    if (testAttributeAccessFunctions() == TESTFAILED)
    {
        std::cerr << m_ModelName << "::runSelfTest: One or more get or set functions FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testEqualityOperator() == TESTFAILED)
    {
        std::cerr << std::format("Equality Operator Test: Comparing 2 {}s FAILED!\n", m_ModelName);
        selfTestStatus = TESTFAILED;
    }

    testOutput();

    if (testAllInsertFailures() != TESTPASSED)
    {
        std::cerr << "Test of all insertion failures FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Insertion Failure Test PASSED!\n";
    }

    if (testCommonUpdateFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Update Failure Test PASSED!\n";
    }

    if (testTextFieldManipulation() == TESTFAILED)
    {
        selfTestStatus = TESTFAILED;
    }

    m_SelfTest = false;
    
    if (selfTestStatus == TESTPASSED)
    {
        std::cout <<  std::format("{} Self Test {}\n", m_ModelName, "PASSED");
    }
    else
    {
        std::cerr <<  std::format("{} Self Test {}\n", m_ModelName, "FAILED");
    }

    return selfTestStatus;
}

void UserSelfTest::selfTestResetAllValues() noexcept
{
    ModelSelfTest::selfTestResetAllValues();

    m_LastName.clear();
    m_FirstName.clear();
    m_MiddleInitial.clear();
    m_Email.clear();
    m_LoginName.clear();
    m_Password.clear();
    m_Created.reset();
    m_LastLogin.reset();
}

std::vector<AttributeTestFunction> UserSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();
    std::vector<AttributeTestFunction> attributeAccessTests;

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

    return testAccessorFunctions<std::size_t>(testPrimaryKey, &m_PrimaryKey, "Primary Key",
        std::bind(&UserModel::setUserID, this, std::placeholders::_1),
        std::bind(&UserModel::getUserID, this));
}

TestStatus UserSelfTest::testLastNameAccess() noexcept
{
    std::string testValue("AnyLastName");
    return testAccessorFunctions<std::string>(testValue, &m_LastName, "Last Name",
        std::bind(&UserModel::setLastName, this, std::placeholders::_1),
        std::bind(&UserModel::getLastName, this));
}

TestStatus UserSelfTest::testFirstNameAccess() noexcept
{
    std::string testValue("AnyFirstName");
    return testAccessorFunctions<std::string>(testValue, &m_FirstName, "First Name",
        std::bind(&UserModel::setFirstName, this, std::placeholders::_1),
        std::bind(&UserModel::getFirstName, this));
}

TestStatus UserSelfTest::testMiddleInitialAccess() noexcept
{
    std::string testValue("A");
    return testAccessorFunctions<std::string>(testValue, &m_MiddleInitial, "Middle Initial",
        std::bind(&UserModel::setMiddleInitial, this, std::placeholders::_1),
        std::bind(&UserModel::getMiddleInitial, this));
}

TestStatus UserSelfTest::testLoginNameAccess() noexcept
{
    std::string testValue("AnyLoginName");
    return testAccessorFunctions<std::string>(testValue, &m_LoginName, "LoginName",
        std::bind(&UserModel::setLoginName, this, std::placeholders::_1),
        std::bind(&UserModel::getLoginName, this));
}

TestStatus UserSelfTest::testPassWordAccess() noexcept
{
    std::string testValue("AnyPassword");
    return testAccessorFunctions<std::string>(testValue, &m_Password, "Password",
        std::bind(&UserModel::setPassword, this, std::placeholders::_1),
        std::bind(&UserModel::getPassword, this));
}

TestStatus UserSelfTest::testCreatedDateAcfcess() noexcept
{
    std::chrono::system_clock::time_point testValue = commonTestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_Created, "Creation TimeStamp",
        std::bind(&UserModel::setCreationDate, this, std::placeholders::_1),
        std::bind(&UserModel::getCreationDate, this));
}

TestStatus UserSelfTest::testLastLoginAccess() noexcept
{
    std::optional<std::chrono::system_clock::time_point> testValue = commonTestTimeStampValue;
    return testOptionalAccessorFunctions<std::chrono::system_clock::time_point>(testValue, &m_LastLogin, "Last Login TimeStamp",
        std::bind(&UserModel::setLastLogin, this, std::placeholders::_1),
        std::bind(&UserModel::getLastLogin, this));
}

TestStatus UserSelfTest::testEmailAccess() noexcept
{
    std::string testValue("AnyEmail");
    return testAccessorFunctions<std::string>(testValue, &m_Email, "Email",
        std::bind(&UserModel::setEmail, this, std::placeholders::_1),
        std::bind(&UserModel::getEmail, this));
}

TestStatus UserSelfTest::testStartTimeAccess() noexcept
{
    std::string testValue("4:30 AM");
    return testAccessorFunctions<std::string>(testValue, &m_Preferences.startTime, "Start Time Preference",
        std::bind(&UserModel::setStartTime, this, std::placeholders::_1),
        std::bind(&UserModel::getStartTime, this));
}

TestStatus UserSelfTest::testEndTimeAccesss() noexcept
{
    std::string testValue("4:15 PM");
    return testAccessorFunctions<std::string>(testValue, &m_Preferences.endTime, "End Time Preference",
        std::bind(&UserModel::setEndTime, this, std::placeholders::_1),
        std::bind(&UserModel::getEndTime, this));
}

TestStatus UserSelfTest::testIncludePriorityInScheduleAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &m_Preferences.includePriorityInSchedule, "Include Priority In Schedule",
        std::bind(&UserModel::setPriorityInSchedule, this, std::placeholders::_1),
        std::bind(&UserModel::isPriorityInSchedule, this));
}

TestStatus UserSelfTest::testIncludeMinorPriorityInScheduleAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &m_Preferences.includeMinorPriorityInSchedule, "Include Minor Priority In Schedule",
        std::bind(&UserModel::setMinorPriorityInSchedule, this, std::placeholders::_1),
        std::bind(&UserModel::isMinorPriorityInSchedule, this));
}

TestStatus UserSelfTest::testUseLetterForMajorPriorityAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &m_Preferences.userLetterForMajorPriority, "Use Letter For Major Priority Access",
        std::bind(&UserModel::setUsingLettersForMaorPriority, this, std::placeholders::_1),
        std::bind(&UserModel::isUsingLettersForMaorPriority, this));
}

TestStatus UserSelfTest::testSeparateMajorAndMinorWithDotAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &m_Preferences.separateMajorAndMinorWithDot, "Separate Major And Minor With Dot Access",
        std::bind(&UserModel::setSeparatingPriorityWithDot, this, std::placeholders::_1),
        std::bind(&UserModel::isSeparatingPriorityWithDot, this));
}

std::vector<ExceptionTestElement> UserSelfTest::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&UserSelfTest::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}


TestStatus UserSelfTest::testExceptionInsert() noexcept
{
    selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = commonTestTimeStampValue;
    setLastName("LastName");
    setFirstName("FirstName");
    setMiddleInitial("M");
    setEmail("FirstName.LastName@LastName.com");
    autoGenerateLoginAndPassword();
    setCreationDate(timeStamp);
    setLastLogin(timeStamp);

    return testExceptionAndSuccessNArgs("UserModel::insert", std::bind(&UserModel::insert, this));
}

TestStatus UserSelfTest::testExceptionUpdate() noexcept
{
    selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = commonTestTimeStampValue;
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

TestStatus UserSelfTest::testExceptionHide() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;

    setUserID(testUserId);

    return testExceptionAndSuccessNArgs("UserModel::hide", std::bind(&UserModel::hide, this, std::placeholders::_1), testUserId);
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

    if (m_VerboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", m_ModelName, __func__) << *this << "\n";
    }

    setCreationDate(commonTestTimeStampValue);
    if (!insert())
    {
        std::cout << "In  UserSelfTest::testAllInsertFailures() Expected successful insert failed\n" << m_ErrorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus UserSelfTest::testEqualityOperator() noexcept
{
    UserModel other;

    other.setUserID(m_PrimaryKey);
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

