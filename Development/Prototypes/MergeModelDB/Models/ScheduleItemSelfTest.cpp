// Project Header Files
#include "commonUtilities.h"
#include "GenericDictionary.h"
#include "ScheduleItemSelfTest.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

ScheduleItemSelfTest::ScheduleItemSelfTest()
{

}

TestStatus ScheduleItemSelfTest::runSelfTest() noexcept
{
    inSelfTest = true;
    TestStatus selfTestStatus = TESTPASSED;

    std::cout << "\nRunning " << modelName << " Self Test" << std::endl;

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

void ScheduleItemSelfTest::selfTestResetAllValues() noexcept
{
    ModelSelfTest::selfTestResetAllValues();

    userID = 0;
    title.clear();
    startTime.reset();
    endTime.reset();
    personal = false;
    location.reset();
    creationTimeStamp.reset();
    lastUpdate.reset();
}

std::vector<ExceptionTestElement> ScheduleItemSelfTest::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&ScheduleItemSelfTest::testExceptionFormatSelectScheduleItemsByDateAndUser, this),
        "Select by Datee and User"});
    exceptionTests.push_back({std::bind(&ScheduleItemSelfTest::testExceptionFormatSelectSiByContentDateRangeUser, this),
        "Select by Content, Date Range and User"});
    exceptionTests.push_back({std::bind(&ScheduleItemSelfTest::testExceptionInsert, this), "testExceeptionInsert"});
    exceptionTests.push_back({std::bind(&ScheduleItemSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&ScheduleItemSelfTest::testExceptionRetrieve, this), "testExceptionRetrieve"});

    return exceptionTests;
}

TestStatus ScheduleItemSelfTest::testExceptionInsert() noexcept
{
    selfTestResetAllValues();
    forceException = true;

    setUserID(1);
    setTitle("Testing Exception handling for Schedule Item Insert");
    setStartDateAndTime(commonTestTimeStampValue);
    setEndDateAndTime(commonTestTimeStampValue);
    setCreationDate(commonTestTimeStampValue);

    if (testFormatExceptionCatchSuccessNArgs(
        "ScheduleItemSelfTest::formatInsertStatement", std::bind(&ScheduleItemSelfTest::formatInsertStatement, this)) == TESTFAILED)
    {
        std::cerr << "ScheduleItemSelfTest::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("ScheduleItemModel::insert", std::bind(&ScheduleItemModel::insert, this));
}

TestStatus ScheduleItemSelfTest::testExceptionUpdate() noexcept
{
    selfTestResetAllValues();

    setScheduleItemID(57);
    setUserID(1);
    setTitle("Testing Exception handling for Schedule Item Update");
    setStartDateAndTime(commonTestTimeStampValue);
    setEndDateAndTime(commonTestTimeStampValue);
    setCreationDate(commonTestTimeStampValue);
    setLastUpdate(commonTestTimeStampValue);

    if (testFormatExceptionCatchSuccessNArgs(
        "ScheduleItemSelfTest::formatUpdateStatement", std::bind(&ScheduleItemSelfTest::formatUpdateStatement, this)) == TESTFAILED)
    {
        std::cerr << "ScheduleItemSelfTest::formatUpdateStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("ScheduleItemModel::update", std::bind(&ScheduleItemModel::update, this));
}

TestStatus ScheduleItemSelfTest::testExceptionRetrieve() noexcept
{
    selfTestResetAllValues();

    setScheduleItemID(57);
    setUserID(1);

    if (testFormatExceptionCatchSuccessNArgs(
        "ScheduleItemSelfTest::formatSelectStatement", std::bind(&ScheduleItemSelfTest::formatSelectStatement, this)) == TESTFAILED)
    {
        std::cerr << "ScheduleItemSelfTest::formatSelectStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("ScheduleItemModel::retrieve", std::bind(&ScheduleItemModel::retrieve, this));
}

TestStatus ScheduleItemSelfTest::testExceptionFormatSelectScheduleItemsByDateAndUser() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day testDate(commonTestDateValue);

    return testFormatExceptionAndSuccessNArgs("ScheduleItemSelfTest::testExceptionFormatSelectScheduleItemsByDateAndUser",
        std::bind(&ScheduleItemModel::formatSelectScheduleItemsByDateAndUser, this, std::placeholders::_1, std::placeholders::_2), testDate, 1);
}

TestStatus ScheduleItemSelfTest::testExceptionFormatSelectSiByContentDateRangeUser() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day startSearch(commonTestDateRangeStartValue);
    std::chrono::year_month_day endSearch(commonTestDateRangeEndValue);
    std::string testTitle("This is only a test");
    std::size_t testUser = 1;

    return testFormatExceptionAndSuccessNArgs("ScheduleItemSelfTest::testExceptionFormatSelectSiByContentDateRangeUser",
        std::bind(&ScheduleItemModel::formatSelectSiByContentDateRangeUser, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
        testTitle, startSearch, endSearch, testUser);
}

TestStatus ScheduleItemSelfTest::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Title", "Start Time", "End Time", " missing required values"
    };

    setScheduleItemID(0);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());

    setUserID(1);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());

    setTitle("Test missing required fields: Set title");
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());

    setStartDateAndTime(commonTestTimeStampValue);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());

    setEndDateAndTime(commonTestTimeStampValue);

    expectedErrors.clear();
    errorMessages.clear();

    setCreationDate(commonTestTimeStampValue);

    if (verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", modelName, __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In ScheduleItemSelfTest::testAllInsertFailuresExpected successful insert failed\n" << errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus ScheduleItemSelfTest::testEqualityOperator() noexcept
{
    try
    {
        ScheduleItemModel other;

        other.setScheduleItemID(primaryKey);

        if (*this == other)
        {
            return TESTFAILED;
        }

        other = *this;

        return (*this == other)? TESTPASSED : TESTFAILED;
    }
    catch(std::exception &e)
    {
        std::cerr << std::format("ScheduleItemSelfTest::testEqualityOperator:: Caught Exception:: {}\n", e.what());
        return TESTFAILED;
    }
}

void ScheduleItemSelfTest::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

std::vector<AttributeTestFunction> ScheduleItemSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<AttributeTestFunction> attributeAccessTests; 
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testScheduleItemIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testUserIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testTitleAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testStartTimeAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testEndTimeAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testCreationTimeStampAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testLastUpDateTimeStampAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testLocationAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testPersonalAccess, this)});

    return attributeAccessTests;
}

TestStatus ScheduleItemSelfTest::testScheduleItemIDAccess() noexcept
{
    std::size_t testPrimaryKey = 57;

    return testAccessorFunctions<std::size_t>(testPrimaryKey, &primaryKey, "Primary Key",
        std::bind(&ScheduleItemModel::setScheduleItemID, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getScheduleItemID, this));
}

TestStatus ScheduleItemSelfTest::testUserIDAccess() noexcept
{
    std::size_t testValue = 1;

    return testAccessorFunctions<std::size_t>(testValue, &userID, "User ID",
        std::bind(&ScheduleItemModel::setUserID, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getUserID, this));
}

TestStatus ScheduleItemSelfTest::testTitleAccess() noexcept
{
    std::string testValue("Testing schedule item Title string access");

    return testAccessorFunctions<std::string>(testValue, &title, "Title",
        std::bind(&ScheduleItemModel::setTitle, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getTitle, this));
}

TestStatus ScheduleItemSelfTest::testStartTimeAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = commonTestTimeStampValue;

    return testTimeStampAccessorFunctions(testValue, &startTime, "Schedule Item Start Time",
        std::bind(&ScheduleItemSelfTest::setStartDateAndTime, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getStartTime, this));
}

TestStatus ScheduleItemSelfTest::testEndTimeAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = commonTestTimeStampValue;

    return testTimeStampAccessorFunctions(testValue, &endTime, "Schedule Item End Time",
        std::bind(&ScheduleItemSelfTest::setEndDateAndTime, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getEndTime, this));
}

TestStatus ScheduleItemSelfTest::testCreationTimeStampAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = commonTestTimeStampValue;

    return testTimeStampAccessorFunctions(testValue, &creationTimeStamp, "Schedule Item Creation Timestamp",
        std::bind(&ScheduleItemSelfTest::setCreationDate, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getCreationDate, this));
}

TestStatus ScheduleItemSelfTest::testLastUpDateTimeStampAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = commonTestTimeStampValue;

    return testTimeStampAccessorFunctions(testValue, &lastUpdate, "Schedule Item Last Update Timestamp",
        std::bind(&ScheduleItemSelfTest::setLastUpdate, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getLastUpdate, this));
}

TestStatus ScheduleItemSelfTest::testPersonalAccess() noexcept
{
    bool testValue = true;

    return testAccessorFunctions<bool>(testValue, &personal, "Personal",
        std::bind(&ScheduleItemModel::setPersonal, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::isPersonal, this));
}

TestStatus ScheduleItemSelfTest::testLocationAccess() noexcept
{
    std::string testValue("Home Office");

    return testOptionalAccessorFunctions<std::string>(testValue, &location, "Location",
        std::bind(&ScheduleItemSelfTest::setLocation, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getLocation, this));
}

