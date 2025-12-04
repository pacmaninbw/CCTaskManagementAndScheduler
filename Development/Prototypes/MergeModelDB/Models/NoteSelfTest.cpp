// Project Header Files
#include "NoteSelfTest.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

NoteSelfTest::NoteSelfTest()
{
}

TestStatus NoteSelfTest::runSelfTest() noexcept
{
    CoreDBInterface::inSelfTest = true;
    TestStatus selfTestStatus = TESTPASSED;
    std::string_view modelName(ModelDBInterface::modelName);

    std::cout << "\nRunning " << modelName << " Self Test\n";

    if (ExceptionSelfTest::testExceptionHandling()!= TESTPASSED)
    {
        std::cerr  << modelName << "::runSelfTest: Exception handling FAILED!\n";
        selfTestStatus = TESTFAILED;
    }
    
    if (testSave() == TESTFAILED)
    {
        selfTestStatus = TESTFAILED;
    }

    if (AttributeSelfTest::testAttributeAccessFunctions() == TESTFAILED)
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

    CoreDBInterface::inSelfTest = false;
    
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

void NoteSelfTest::selfTestResetAllValues() noexcept
{
    ModelSelfTest::selfTestResetAllValues();
    userID = 0;
    content.clear();
    creationDate = {};
    lastUpdate = {};
}

std::vector<AttributeTestFunction> NoteSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<AttributeTestFunction> attributeAccessTests;
    attributeAccessTests.push_back({std::bind(&NoteSelfTest::testNoteIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&NoteSelfTest::testUserIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&NoteSelfTest::testContentAccess, this)});
    attributeAccessTests.push_back({std::bind(&NoteSelfTest::testDateAddedAccess, this)});
    attributeAccessTests.push_back({std::bind(&NoteSelfTest::testLastUpdateAccess, this)});

    return attributeAccessTests;
}

std::vector<ExceptionTestElement> NoteSelfTest::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionSelectByNoteID, this), "selectByNoteID"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionRetrieve, this), "testExceptionRetrieve"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionFormatSelectByUserId, this), "selectByUserId"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionFormatSelectByUserIdAndSimilarContent, this), "selectByUserIdAndSimilarContent"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionFormatSelectByUserIdAndCreationDateRange, this), "selectByUserIdAndCreationDateRange"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionFormatSelectByUserIdAndUpdateDateRange, this), "selectByUserIdAndUpdateDateRange"});

    return exceptionTests;
}

TestStatus NoteSelfTest::testExceptionInsert() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setContent("Testing insertion exception");
    setUserId(27);
    setDateAdded(timeStamp);
    setLastModified(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::insert", std::bind(&NoteModel::insert, this));
}

TestStatus NoteSelfTest::testExceptionUpdate() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setNoteId(1);
    setContent("Testing insertion exception");
    setUserId(27);
    setDateAdded(timeStamp);
    setLastModified(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::update", std::bind(&NoteModel::update, this));
}

TestStatus NoteSelfTest::testExceptionRetrieve() noexcept
{
    selfTestResetAllValues();

    setNoteId(1);

    return testExceptionAndSuccessNArgs("NoteModel::retrieve", std::bind(&NoteModel::retrieve, this));
}

TestStatus NoteSelfTest::testExceptionSelectByNoteID() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("NoteModel::selectByNoteID", std::bind(&NoteModel::selectByNoteID, this, std::placeholders::_1), 1);
}

TestStatus NoteSelfTest::testExceptionFormatSelectByUserId() noexcept
{
    selfTestResetAllValues();

    return testFormatExceptionAndSuccessNArgs("NoteModel::formatSelectByUserId",
        std::bind(&NoteModel::formatSelectByUserId, this, std::placeholders::_1), 1);
}

TestStatus NoteSelfTest::testExceptionFormatSelectByUserIdAndSimilarContent() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::string testContent("Test Content");

    return testFormatExceptionAndSuccessNArgs("NoteModel::formatSelectByUserIdAndSimilarContent",
        std::bind(&NoteModel::formatSelectByUserIdAndSimilarContent, this, std::placeholders::_1, std::placeholders::_2),
        testUserId, testContent);
}

TestStatus NoteSelfTest::testExceptionFormatSelectByUserIdAndCreationDateRange() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::chrono::year_month_day startDate = getTodaysDateMinus(OneWeek);
    std::chrono::year_month_day endDate = getTodaysDate();

    return testFormatExceptionAndSuccessNArgs("NoteModel::formatSelectByUserIdAndCreationDateRange",
        std::bind(&NoteModel::formatSelectByUserIdAndCreationDateRange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        testUserId, startDate, endDate);
}

TestStatus NoteSelfTest::testExceptionFormatSelectByUserIdAndUpdateDateRange() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::chrono::year_month_day startDate = getTodaysDateMinus(OneWeek);
    std::chrono::year_month_day endDate = getTodaysDate();

    return testFormatExceptionAndSuccessNArgs("NoteModel::formatSelectByUserIdAndUpdateDateRange",
        std::bind(&NoteModel::formatSelectByUserIdAndUpdateDateRange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        testUserId, startDate, endDate);
}

TestStatus NoteSelfTest::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Content", "Date Created", "Last Update", " missing required values"
    };

    setNoteId(0);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setUserId(27);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setContent("Testing negative note insertion path");

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setDateAdded(timeStamp);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setLastModified(timeStamp);

    expectedErrors.clear();
    errorMessages.clear();

    if (verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", modelName, __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In  NoteSelfTest::testAllInsertFailures() Expected successful insert failed\n" << errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus NoteSelfTest::testNoteIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(27, &primaryKey, "Primary Key",
        std::bind(&NoteModel::setNoteId, this, std::placeholders::_1),
        std::bind(&NoteModel::getNoteId, this));
}

TestStatus NoteSelfTest::testUserIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(31, &userID, "User ID",
        std::bind(&NoteModel::setUserId, this, std::placeholders::_1),
        std::bind(&NoteModel::getUserId, this));
}

TestStatus NoteSelfTest::testContentAccess() noexcept
{
    return testAccessorFunctions<std::string>("Test note content access", &content, "Content",
        std::bind(&NoteModel::setContent, this, std::placeholders::_1),
        std::bind(&NoteModel::getContent, this));
}

TestStatus NoteSelfTest::testDateAddedAccess() noexcept
{
    return testTimeStampAccessorFunctions(std::chrono::system_clock::now(), &creationDate, "Date Added",
        std::bind(&NoteModel::setDateAdded, this, std::placeholders::_1),
        std::bind(&NoteModel::getDateAdded, this));
}

TestStatus NoteSelfTest::testLastUpdateAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();
    return testTimeStampAccessorFunctions(testValue, &lastUpdate, "Date Added",
        std::bind(&NoteModel::setLastModified, this, std::placeholders::_1),
        std::bind(&NoteModel::getLastModified, this));
}

TestStatus NoteSelfTest::testEqualityOperator() noexcept
{
    NoteModel other;

    other.setNoteId(primaryKey);
    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;

    return (*this == other)? TESTPASSED : TESTFAILED;
}

void NoteSelfTest::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

