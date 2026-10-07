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
    CoreDBInterface::m_selfTest = true;
    TestStatus selfTestStatus = TESTPASSED;
    std::string_view modelName(getModelName());

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

    CoreDBInterface::m_selfTest = false;
    
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
    m_userID = 0;
    m_content.clear();
    m_createdTimeStamp.reset();
    m_lastUpdateTimeStamp.reset();
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
    attributeAccessTests.push_back({std::bind(&NoteSelfTest::testLastModifiedByUserAccess, this)});

    return attributeAccessTests;
}

std::vector<ExceptionTestElement> NoteSelfTest::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
//    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionRetrieve, this), "testExceptionRetrieve"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}

TestStatus NoteSelfTest::testExceptionInsert() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setContent("Testing insertion exception");
    setUserId(27);
    setCreatedTimeStamp(timeStamp);
    setLastModifiedTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::insert", std::bind(&NoteModel::insert, this));
}

TestStatus NoteSelfTest::testExceptionUpdate() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setNoteId(1);
    setContent("Testing insertion exception");
    setUserId(27);
    setCreatedTimeStamp(timeStamp);
    setLastModifiedTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::update", std::bind(&NoteModel::update, this));
}

TestStatus NoteSelfTest::testExceptionHide() noexcept
{
   selfTestResetAllValues();
   std::size_t testUserId = 27;

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setNoteId(1);
    setContent("Testing insertion exception");
    setUserId(testUserId);
    setCreatedTimeStamp(timeStamp);
    setLastModifiedTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::hide", std::bind(&NoteModel::hide, this, std::placeholders::_1), testUserId);
}

TestStatus NoteSelfTest::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Content", " missing required values"
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

    expectedErrors.clear();
    clearErrorMessages();

    if (m_verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", getModelName(), __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In  NoteSelfTest::testAllInsertFailures() Expected successful insert failed\n" << m_errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus NoteSelfTest::testNoteIdAccesss() noexcept
{
    return testPrimaryKeyAccessFunctions(27,
        std::bind(&NoteModel::setNoteId, this, std::placeholders::_1),
        std::bind(&NoteModel::getNoteId, this));
}

TestStatus NoteSelfTest::testUserIdAccesss() noexcept
{
    return testForeignKeyFields(31, &m_userID, "User ID",
        std::bind(&NoteModel::setUserId, this, std::placeholders::_1),
        std::bind(&NoteModel::getUserId, this));
}

TestStatus NoteSelfTest::testContentAccess() noexcept
{
    return testAccessorFunctions<std::string>("Test note content access", &m_content, "Content",
        std::bind(&NoteModel::setContent, this, std::placeholders::_1),
        std::bind(&NoteModel::getContent, this));
}

TestStatus NoteSelfTest::testDateAddedAccess() noexcept
{
    return testTimeStampAccessorFunctions(common::TestTimeStampValue, &m_createdTimeStamp, "Date Added",
        std::bind(&NoteModel::setCreatedTimeStamp, this, std::placeholders::_1),
        std::bind(&NoteModel::getCreatedTSValue, this));
}

TestStatus NoteSelfTest::testLastUpdateAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_lastUpdateTimeStamp, "Last Modified Time Stamp",
        std::bind(&NoteModel::setLastModifiedTimeStamp, this, std::placeholders::_1),
        std::bind(&NoteModel::getLastModifiedValue, this));
}

TestStatus NoteSelfTest::testLastModifiedByUserAccess() noexcept
{
    std::size_t testUserId = 1;

    return testLastModifiedByAccess(testUserId);
}

TestStatus NoteSelfTest::testEqualityOperator() noexcept
{
    NoteModel other;

    other.setNoteId(m_primaryKey);
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

