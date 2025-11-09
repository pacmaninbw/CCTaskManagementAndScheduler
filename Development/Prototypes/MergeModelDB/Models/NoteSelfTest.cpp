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

bool NoteSelfTest::runSelfTest()
{
    inSelfTest = true;
    bool allSelfTestsPassed = true;

    if (verboseOutput)
    {
        std::cout <<  std::format("Running {} Self Test\n", modelName);
    }

    if (!testExceptionHandling())
    {
        std::cerr << "Exception handling FAILED!\n";
        allSelfTestsPassed = false;
    }
    
    if (!testSave())
    {
        allSelfTestsPassed = false;
    }

    if (!testAccessorFunctionsPassed())
    {
        std::cerr << "One or more get or set functions FAILED!\n";
        allSelfTestsPassed = false;
    }

    if (!diffTest())
    {
        std::cerr << std::format("Equality Operator test for {} FAILED!\n", modelName);
        allSelfTestsPassed = false;
    }
    
    if (verboseOutput)
    {
        std::cout << "Test Output: " << *this << "\n";
    }

    if (testAllInsertFailures() != TESTPASSED)
    {
        allSelfTestsPassed = false;
        std::cerr << "Test of all insertion failures FAILED!\n";
    }

    if (testCommonUpdateFailurePath() != TESTPASSED)
    {
        allSelfTestsPassed = false;
    }

    inSelfTest = false;

    if (allSelfTestsPassed)
    {
        std::cout <<  std::format("{} Self Test {}\n", modelName, "PASSED");
    }
    else
    {
        std::cerr <<  std::format("{} Self Test {}\n", modelName, "FAILED");
    }

    return allSelfTestsPassed;
}

void NoteSelfTest::selfTestResetAllValues()
{
    ModelSelfTest::selfTestResetAllValues();
    userID = 0;
    content.clear();
    creationDate = {};
    lastUpdate = {};
}

bool NoteSelfTest::testAccessorFunctionsPassed()
{
    selfTestResetAllValues();

    bool allAccessorFunctionsPassed = true;
    std::vector<std::function<bool(void)>> accessTests;
    accessTests.push_back({std::bind(&NoteSelfTest::testNoteIdAccesss, this)});
    accessTests.push_back({std::bind(&NoteSelfTest::testUserIdAccesss, this)});
    accessTests.push_back({std::bind(&NoteSelfTest::testContentAccess, this)});
    accessTests.push_back({std::bind(&NoteSelfTest::testDateAddedAccess, this)});
    accessTests.push_back({std::bind(&NoteSelfTest::testLastUpdateAccess, this)});

    for (auto accessTest: accessTests)
    {
        if (!accessTest())
        {
            allAccessorFunctionsPassed = false;
        }
    }

    return allAccessorFunctionsPassed;
}

bool NoteSelfTest::testExceptionHandling() noexcept
{
    selfTestResetAllValues();

    bool exceptionHandlingPassed = true;
    bool globalForceException = forceException;
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionSelectByNoteID, this), "selectByUserID"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionRetrieve, this), "testExceptionRetrieve"});

    if (!forceExceptionsLoop(exceptionTests))
    {
        exceptionHandlingPassed = false;
    }

    forceException = globalForceException;

    return exceptionHandlingPassed;
}

bool NoteSelfTest::testExceptionInsert() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setContent("Testing insertion exception");
    setUserId(27);
    setDateAdded(timeStamp);
    setLastModified(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::insert", std::bind(&NoteModel::insert, this));
}

bool NoteSelfTest::testExceptionUpdate() noexcept
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

bool NoteSelfTest::testExceptionRetrieve() noexcept
{
    selfTestResetAllValues();

    setNoteId(1);

    return testExceptionAndSuccessNArgs("NoteModel::retrieve", std::bind(&NoteModel::retrieve, this));
}

bool NoteSelfTest::testExceptionSelectByNoteID() noexcept
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
        std::cout << "NoteSelfTest::testAllInsertFailures() before successful insert *this = " << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In  NoteSelfTest::testAllInsertFailures() Expected successful insert failed\n" << errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

bool NoteSelfTest::testNoteIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(27, &primaryKey, "Primary Key",
        std::bind(&NoteModel::setNoteId, this, std::placeholders::_1),
        std::bind(&NoteModel::getNoteId, this));
}

bool NoteSelfTest::testUserIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(31, &userID, "User ID",
        std::bind(&NoteModel::setUserId, this, std::placeholders::_1),
        std::bind(&NoteModel::getUserId, this));
}

bool NoteSelfTest::testContentAccess() noexcept
{
    return testAccessorFunctions<std::string>("Test note content access", &content, "Content",
        std::bind(&NoteModel::setContent, this, std::placeholders::_1),
        std::bind(&NoteModel::getContent, this));
}

bool NoteSelfTest::testDateAddedAccess() noexcept
{
    return testTimeStampAccessorFunctions(std::chrono::system_clock::now(), &creationDate, "Date Added",
        std::bind(&NoteModel::setDateAdded, this, std::placeholders::_1),
        std::bind(&NoteModel::getDateAdded, this));
}

bool NoteSelfTest::testLastUpdateAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();
    return testTimeStampAccessorFunctions(testValue, &lastUpdate, "Date Added",
        std::bind(&NoteModel::setLastModified, this, std::placeholders::_1),
        std::bind(&NoteModel::getLastModified, this));
}

bool NoteSelfTest::diffTest() noexcept
{
    NoteModel other;

    other.setNoteId(primaryKey);
    if (*this == other)
    {
        return false;
    }

    other = *this;

    return *this == other;
}
