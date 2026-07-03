// Project Header Files
#include "UserGoalSelfTest.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>


UserGoalSelfTest::UserGoalSelfTest()
{
}

TestStatus UserGoalSelfTest::runSelfTest() noexcept
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

void UserGoalSelfTest::selfTestResetAllValues() noexcept
{
    ModelSelfTest::selfTestResetAllValues();

    userID = 0;
    description.clear();;
    priority.reset();
    parentID.reset();
    creationDate.reset();
    lastUpdate = {};
}

std::vector<AttributeTestFunction> UserGoalSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<AttributeTestFunction> attributeAccessTests;
    attributeAccessTests.push_back({std::bind(&UserGoalSelfTest::testGoalIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalSelfTest::testUserIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalSelfTest::testDescriptionAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalSelfTest::testCreationDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalSelfTest::testParentIdAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalSelfTest::testPriorityAccess, this)});

    return attributeAccessTests;
}

TestStatus UserGoalSelfTest::testGoalIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(57, &m_PrimaryKey, "Primary Key",
        std::bind(&UserGoalModel::setGoalId, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getGoalId, this));
}

TestStatus UserGoalSelfTest::testUserIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(23, &userID, "User ID",
        std::bind(&UserGoalModel::setUserId, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getUserId, this));
}

TestStatus UserGoalSelfTest::testDescriptionAccess() noexcept
{
    return testAccessorFunctions<std::string>("Test note content access", &description, "Content",
        std::bind(&UserGoalModel::setDescription, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getDescription, this));
}

TestStatus UserGoalSelfTest::testCreationDateAccess() noexcept
{
    return testTimeStampAccessorFunctions(commonTestTimeStampValue, &creationDate, "Date Added",
        std::bind(&UserGoalModel::setCreationTimeStamp, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getCreationTimeStamp, this));
}

TestStatus UserGoalSelfTest::testParentIdAccess() noexcept
{
    return testOptionalAccessorFunctions<std::size_t>(1, &parentID, "Parent ID",
        std::bind(&UserGoalModel::setParentID, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getParentId, this));
}

TestStatus UserGoalSelfTest::testPriorityAccess() noexcept
{
    return testOptionalAccessorFunctions<unsigned int>(1, &priority, "Priority",
        std::bind(&UserGoalModel::setPriority, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getPriority, this));
}


std::vector<ExceptionTestElement> UserGoalSelfTest::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}

TestStatus UserGoalSelfTest::testExceptionInsert() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = commonTestTimeStampValue;
    setDescription("Testing insertion exception");
    setUserId(27);
    setCreationTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("UserGoalModel::insert", std::bind(&UserGoalModel::insert, this));
}

TestStatus UserGoalSelfTest::testExceptionUpdate() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = commonTestTimeStampValue;
    setGoalId(37);
    setDescription("Testing insertion exception");
    setUserId(23);
    setCreationTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("UserGoalModel::update", std::bind(&UserGoalModel::update, this));
}

TestStatus UserGoalSelfTest::testExceptionHide() noexcept
{
    selfTestResetAllValues();
    std::size_t testUserId = 1;

    setGoalId(1);
    setUserId(testUserId);

    return testExceptionAndSuccessNArgs("UserGoalModel::hide", std::bind(&UserGoalModel::hide, this, std::placeholders::_1), testUserId);
}



TestStatus UserGoalSelfTest::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Description", " missing required values"
    };

    setGoalId(0);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setUserId(51);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setDescription("Testing negative note insertion path");

    expectedErrors.clear();
    clearErrorMessages();

    setCreationTimeStamp(commonTestTimeStampValue);

    if (m_VerboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", m_ModelName, __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In  UserGoalSelfTest::testAllInsertFailures() Expected successful insert failed\n" << m_ErrorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus UserGoalSelfTest::testEqualityOperator() noexcept
{
    UserGoalModel other;

    other.setGoalId(m_PrimaryKey);
    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;

    return (*this == other)? TESTPASSED : TESTFAILED;
}

void UserGoalSelfTest::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

