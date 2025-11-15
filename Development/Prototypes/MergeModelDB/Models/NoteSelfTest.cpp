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

void NoteSelfTest::selfTestResetAllValues()
{
    ModelSelfTest::selfTestResetAllValues();
    userID = 0;
    content.clear();
    creationDate = {};
    lastUpdate = {};
}

std::vector<std::function<TestStatus(void)>> NoteSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<std::function<TestStatus(void)>> attributeAccessTests;
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
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionSelectByNoteID, this), "selectByUserID"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionRetrieve, this), "testExceptionRetrieve"});

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

