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

bool UserGoalSelfTest::runSelfTest()
{
    inSelfTest = true;
    bool allSelfTestsPassed = true;

    if (verboseOutput)
    {
        std::clog <<  std::format("Running {} Self Test\n", modelName);
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
        std::clog << std::format("Equalify Operator test for {} FAILED!\n", modelName);
        allSelfTestsPassed = false;
    }
    
    if (verboseOutput)
    {
        std::clog << "Test Output: " << *this << "\n";
    }

    if (testAllInsertFailures() != TESTPASSED)
    {
        allSelfTestsPassed = false;
        std::clog << "Test of all insertion failures FAILED!\n";
    }

    if (testCommonUpdateFailurePath() != TESTPASSED)
    {
        allSelfTestsPassed = false;
    }

    inSelfTest = false;

    std::clog <<  std::format("{} Self Test {}\n", modelName, allSelfTestsPassed? "PASSED" : "FAILED");

    return allSelfTestsPassed;
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

bool UserGoalSelfTest::testAccessorFunctionsPassed()
{
    selfTestResetAllValues();

    bool allAccessorFunctionsPassed = true;
    std::vector<std::function<bool(void)>> accessTests;
    accessTests.push_back({std::bind(&UserGoalSelfTest::testGoalIdAccesss, this)});
    accessTests.push_back({std::bind(&UserGoalSelfTest::testUserIdAccesss, this)});
    accessTests.push_back({std::bind(&UserGoalSelfTest::testDescriptionAccess, this)});
    accessTests.push_back({std::bind(&UserGoalSelfTest::testCreationDateAccess, this)});
    accessTests.push_back({std::bind(&UserGoalSelfTest::testParentIdAccess, this)});
    accessTests.push_back({std::bind(&UserGoalSelfTest::testPriorityAccess, this)});

    for (auto accessTest: accessTests)
    {
        if (!accessTest())
        {
            allAccessorFunctionsPassed = false;
        }
    }

    return allAccessorFunctionsPassed;
}

bool UserGoalSelfTest::testGoalIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(57, &primaryKey, "Primary Key",
        std::bind(&UserGoalModel::setGoalId, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getGoalId, this));
}

bool UserGoalSelfTest::testUserIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(23, &userID, "User ID",
        std::bind(&UserGoalModel::setUserId, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getUserId, this));
}

bool UserGoalSelfTest::testDescriptionAccess() noexcept
{
    return testAccessorFunctions<std::string>("Test note content access", &description, "Content",
        std::bind(&UserGoalModel::setDescription, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getDescription, this));
}

bool UserGoalSelfTest::testCreationDateAccess() noexcept
{
    return testTimeStampAccessorFunctions(std::chrono::system_clock::now(), &creationDate, "Date Added",
        std::bind(&UserGoalModel::setCreationTimeStamp, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getCreationTimeStamp, this));
}

bool UserGoalSelfTest::testParentIdAccess() noexcept
{
    return testOptionalAccessorFunctions<std::size_t>(1, &parentID, "Parent ID",
        std::bind(&UserGoalModel::setParentID, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getParentId, this));
}

bool UserGoalSelfTest::testPriorityAccess() noexcept
{
    return testOptionalAccessorFunctions<unsigned int>(1, &priority, "Priority",
        std::bind(&UserGoalModel::setPriority, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getPriority, this));
}

bool UserGoalSelfTest::testExceptionHandling() noexcept
{
    selfTestResetAllValues();

    bool exceptionHandlingPassed = true;
    bool globalForceException = forceException;
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

    if (!forceExceptionsLoop(exceptionTests))
    {
        exceptionHandlingPassed = false;
    }

    forceException = globalForceException;

    return exceptionHandlingPassed;
}

bool UserGoalSelfTest::testExceptionInsert() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setDescription("Testing insertion exception");
    setUserId(27);
    setCreationTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("UserGoalModel::insert", std::bind(&UserGoalModel::insert, this));
}

bool UserGoalSelfTest::testExceptionUpdate() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setGoalId(37);
    setDescription("Testing insertion exception");
    setUserId(23);
    setCreationTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("UserGoalModel::update", std::bind(&UserGoalModel::update, this));
}

bool UserGoalSelfTest::testExceptionRetrieve() noexcept
{
    selfTestResetAllValues();

    setGoalId(1);

    return testExceptionAndSuccessNArgs("UserGoalModel::retrieve", std::bind(&UserGoalModel::retrieve, this));
}

bool UserGoalSelfTest::testExceptionSelectByGoalID() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("UserGoalModel::selectByGoalID", std::bind(&UserGoalModel::selectByGoalID, this, std::placeholders::_1), 1);
}

bool UserGoalSelfTest::testExceptionSelectByUserIDAndDescription() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::string testDescription("Test SelectByUserIDAndDescription Exception Handling");

    return testExceptionAndSuccessNArgs("UserGoalModel::selectByUserIDAndDescription",
        std::bind(&UserGoalModel::selectByUserIDAndDescription,
        this, std::placeholders::_1, std::placeholders::_2), testUserId, testDescription);
}

bool UserGoalSelfTest::testExceptionFormatSelectAllByUserId() noexcept
{
    selfTestResetAllValues();

    return testFormatExceptionAndSuccessNArgs("UserGoalModel::formatSelectAllByUserId",
        std::bind(&UserGoalModel::formatSelectAllByUserId, this, std::placeholders::_1), 1);
}

bool UserGoalSelfTest::testExceptionFormatSelectAllChildGoalsWithParentFromUser() noexcept
{
    selfTestResetAllValues();

    std::size_t testParentId = 1;
    std::size_t testUserId = 23;

    return testFormatExceptionAndSuccessNArgs("UserGoalModel::formatSelectAllChildGoalsWithParentFromUser",
        std::bind(&UserGoalModel::formatSelectAllChildGoalsWithParentFromUser,
        this, std::placeholders::_1, std::placeholders::_2), testParentId, testUserId);
}

bool UserGoalSelfTest::testExceptionFormatSelectAllChildGoalsWithParent() noexcept
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

    return true;

}

bool UserGoalSelfTest::testExceptionFormatSelectByExactDescription() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::string testDescription("Test SelectByUserIDAndDescription Exception Handling");

    return testFormatExceptionAndSuccessNArgs("UserGoalModel::formatSelectByExactDescription",
        std::bind(&UserGoalModel::formatSelectByExactDescription,
        this, std::placeholders::_1, std::placeholders::_2), testDescription, testUserId);
}

bool UserGoalSelfTest::testExceptionFormatSelectBySimilarDescription() noexcept
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
        std::clog << "UserGoalSelfTest::testAllInsertFailures() before successful insert *this = " << *this << "\n";
    }

    if (!insert())
    {
        std::clog << "In  UserGoalSelfTest::testAllInsertFailures() Expected successful insert failed\n" << errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

bool UserGoalSelfTest::diffTest() noexcept
{
    UserGoalModel other;

    other.setGoalId(primaryKey);
    if (*this == other)
    {
        return false;
    }

    other = *this;

    return *this == other;
}

