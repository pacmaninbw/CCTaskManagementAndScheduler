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
    scheduleItemType = static_cast<ScheduleItemModel::ScheduleItemType>(0);
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
//    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionSelectByTaskID, this), "selectByTaskID"});

    return exceptionTests;
}

TestStatus ScheduleItemSelfTest::testExceptionInsert() noexcept
{
    return TestStatus();
}

TestStatus ScheduleItemSelfTest::testExceptionUpdate() noexcept
{
    return TestStatus();
}

TestStatus ScheduleItemSelfTest::testExceptionRetrieve() noexcept
{
    return TestStatus();
}

TestStatus ScheduleItemSelfTest::testAllInsertFailures()
{
    return TestStatus();
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
    attributeAccessTests.push_back({std::bind(&ScheduleItemSelfTest::testScheduleItemTypeAccess, this)});
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

TestStatus ScheduleItemSelfTest::testScheduleItemTypeAccess() noexcept
{
    ScheduleItemModel::ScheduleItemType testValue = ScheduleItemModel::ScheduleItemType::Phone_Call;
    setType(testValue);

    return TESTFAILED;
}

TestStatus ScheduleItemSelfTest::testStartTimeAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();

    return testTimeStampAccessorFunctions(testValue, &startTime, "Schedule Item Start Time",
        std::bind(&ScheduleItemSelfTest::setStartDateAndTime, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getStartTime, this));
}

TestStatus ScheduleItemSelfTest::testEndTimeAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();

    return testTimeStampAccessorFunctions(testValue, &endTime, "Schedule Item End Time",
        std::bind(&ScheduleItemSelfTest::setEndDateAndTime, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getEndTime, this));
}

TestStatus ScheduleItemSelfTest::testCreationTimeStampAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();

    return testTimeStampAccessorFunctions(testValue, &creationTimeStamp, "Schedule Item Creation Timestamp",
        std::bind(&ScheduleItemSelfTest::setCreationDate, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getCreationDate, this));
}

TestStatus ScheduleItemSelfTest::testLastUpDateTimeStampAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();

    return testTimeStampAccessorFunctions(testValue, &lastUpdate, "Schedule Item Last Update Timestamp",
        std::bind(&ScheduleItemSelfTest::setLastUpdate, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getLastUpdate, this));
}

TestStatus ScheduleItemSelfTest::testPersonalAccess() noexcept
{
    return TESTFAILED;
}

TestStatus ScheduleItemSelfTest::testLocationAccess() noexcept
{
    std::string testValue("Home Office");

    return testOptionalAccessorFunctions<std::string>(testValue, &location, "Due Date",
        std::bind(&ScheduleItemSelfTest::setLocation, this, std::placeholders::_1),
        std::bind(&ScheduleItemSelfTest::getLocation, this));
}

