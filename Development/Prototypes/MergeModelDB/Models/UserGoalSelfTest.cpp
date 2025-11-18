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
    inSelfTest = true;
    TestStatus selfTestStatus = TESTPASSED;

    std::cout << "\nRunning " << modelName << " Self Test\n";

    if (testExceptionHandling()!= TESTPASSED)
    {
        std::cerr  << modelName << "::runSelfTest: Exception handling FAILED!\n";
        selfTestStatus = TESTFAILED;
    }
    
    if (testSave() == TESTFAILED)
    {
        selfTestStatus = TESTFAILED;
    }

    if (testAttributeAccessFunctions() == TESTFAILED)
    {
        std::cerr << modelName << "::runSelfTest: One or more get or set functions FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testEqualityOperator() == TESTFAILED)
    {
        std::cerr << std::format("Equality Operator Test: Comparing 2 {}s FAILED!\n", modelName);
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

    inSelfTest = false;
    
    if (selfTestStatus == TESTPASSED)
    {
        std::cout <<  std::format("{} Self Test {}\n", modelName, "PASSED");
    }
    else
    {
        std::cerr <<  std::format("{} Self Test {}\n", modelName, "FAILED");
    }

    return selfTestStatus;
}

void UserGoalSelfTest::selfTestResetAllValues()
{
    ModelSelfTest::selfTestResetAllValues();

    userID = 0;
    description.clear();;
    priority.reset();
    parentID.reset();
    creationDate.reset();
    lastUpdate = {};
}

std::vector<std::function<TestStatus(void)>> UserGoalSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<std::function<TestStatus(void)>> attributeAccessTests;
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
    return testAccessorFunctions<std::size_t>(57, &primaryKey, "Primary Key",
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
    return testTimeStampAccessorFunctions(std::chrono::system_clock::now(), &creationDate, "Date Added",
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
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionSelectByGoalID, this), "selectByGoalID"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionSelectByUserIDAndDescription, this),
        "SelectByUserIDAndDescription"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionFormatSelectAllByUserId, this),
        "FormatSelectAllByUserId"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionFormatSelectAllChildGoalsWithParentFromUser, this),
        "selectAllChildGoalsWithParentFromUser"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionFormatSelectAllChildGoalsWithParent, this),
        "formatSelectAllChildGoalsWithParent"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionFormatSelectByExactDescription, this),
        "formatSelectByExactDescription"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionFormatSelectBySimilarDescription, this),
        "formatSelectBySimilarDescription"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&UserGoalSelfTest::testExceptionRetrieve, this), "testExceptionRetrieve"});

    return exceptionTests;
}

TestStatus UserGoalSelfTest::testExceptionInsert() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setDescription("Testing insertion exception");
    setUserId(27);
    setCreationTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("UserGoalModel::insert", std::bind(&UserGoalModel::insert, this));
}

TestStatus UserGoalSelfTest::testExceptionUpdate() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setGoalId(37);
    setDescription("Testing insertion exception");
    setUserId(23);
    setCreationTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("UserGoalModel::update", std::bind(&UserGoalModel::update, this));
}

TestStatus UserGoalSelfTest::testExceptionRetrieve() noexcept
{
    selfTestResetAllValues();

    setGoalId(1);

    return testExceptionAndSuccessNArgs("UserGoalModel::retrieve", std::bind(&UserGoalModel::retrieve, this));
}

TestStatus UserGoalSelfTest::testExceptionSelectByGoalID() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("UserGoalModel::selectByGoalID", std::bind(&UserGoalModel::selectByGoalID, this, std::placeholders::_1), 1);
}

TestStatus UserGoalSelfTest::testExceptionSelectByUserIDAndDescription() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::string testDescription("Test SelectByUserIDAndDescription Exception Handling");

    return testExceptionAndSuccessNArgs("UserGoalModel::selectByUserIDAndDescription",
        std::bind(&UserGoalModel::selectByUserIDAndDescription,
        this, std::placeholders::_1, std::placeholders::_2), testUserId, testDescription);
}

TestStatus UserGoalSelfTest::testExceptionFormatSelectAllByUserId() noexcept
{
    selfTestResetAllValues();

    return testFormatExceptionAndSuccessNArgs("UserGoalModel::formatSelectAllByUserId",
        std::bind(&UserGoalModel::formatSelectAllByUserId, this, std::placeholders::_1), 1);
}

TestStatus UserGoalSelfTest::testExceptionFormatSelectAllChildGoalsWithParentFromUser() noexcept
{
    selfTestResetAllValues();

    std::size_t testParentId = 1;
    std::size_t testUserId = 23;

    return testFormatExceptionAndSuccessNArgs("UserGoalModel::formatSelectAllChildGoalsWithParentFromUser",
        std::bind(&UserGoalModel::formatSelectAllChildGoalsWithParentFromUser,
        this, std::placeholders::_1, std::placeholders::_2), testParentId, testUserId);
}

TestStatus UserGoalSelfTest::testExceptionFormatSelectAllChildGoalsWithParent() noexcept
{
    selfTestResetAllValues();

    UserGoalModel testParent;
    testParent.setGoalId(1);
    testParent.setUserId(27);

    std::string funcName("UserGoalSelfTest::formatSelectAllChildGoalsWithParent()");
    forceException = true;
    std::string formattedQuery = formatSelectAllChildGoalsWithParent(testParent);
    if (!formattedQuery.empty())
    {
        return testExceptionReportFailure(false, false, funcName);
    }

    forceException = false;
    formattedQuery.clear();
    formattedQuery = formatSelectAllChildGoalsWithParent(testParent);
    if (formattedQuery.empty())
    {
        return testExceptionReportFailure(true, false, funcName);
    }

    return TESTPASSED;

}

TestStatus UserGoalSelfTest::testExceptionFormatSelectByExactDescription() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::string testDescription("Test SelectByUserIDAndDescription Exception Handling");

    return testFormatExceptionAndSuccessNArgs("UserGoalModel::formatSelectByExactDescription",
        std::bind(&UserGoalModel::formatSelectByExactDescription,
        this, std::placeholders::_1, std::placeholders::_2), testDescription, testUserId);
}

TestStatus UserGoalSelfTest::testExceptionFormatSelectBySimilarDescription() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::string testDescription("Test SelectByUserIDAndDescription Exception Handling");

    return testFormatExceptionAndSuccessNArgs("UserGoalModel::formatSelectBySimilarDescription",
        std::bind(&UserGoalModel::formatSelectBySimilarDescription,
        this, std::placeholders::_1, std::placeholders::_2), testDescription, testUserId);
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
    errorMessages.clear();

    if (verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", modelName, __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In  UserGoalSelfTest::testAllInsertFailures() Expected successful insert failed\n" << errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus UserGoalSelfTest::testEqualityOperator() noexcept
{
    UserGoalModel other;

    other.setGoalId(primaryKey);
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

