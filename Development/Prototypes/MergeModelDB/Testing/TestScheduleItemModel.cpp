// Project Header Files
#include "ScheduleItemList.h"
#include "ScheduleItemModel.h"
#include "TestDBInterfaceCore.h"
#include "TestScheduleItemModel.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <string>
#include <vector>

TestScheduleItemModel::TestScheduleItemModel()
: TestDBInterfaceCore("ScheduleItem")
{
    std::vector<TestScheduleItemInput> testScheduleItemInput =
    {
        {"2024-12-02 9:30:00", "2024-12-02 10:30:00", "Dr. Cinman", true, "8635 W. Third Street"},
        {"2024-12-11 9:00:00", "2024-12-11 10:30:00", "Bill McCandless birthday call", true, ""},
        {"2024-12-13 9:00:00", "2024-12-13 10:30:00", "Jean Hom birthday call", true, ""},
        {"2024-12-13 10:00:00", "2024-12-13 11:00:00", "Dr. David Aftergood", true, "99 N. La Cienega Blvd."},
        {"2024-12-18 9:00:00", "2024-12-18 10:30:00", "Dina birthday call", true, ""},
        {"2025-12-11 9:00:00", "2025-12-11 10:30:00", "Bill McCandless birthday call", true, ""},
        {"2025-12-13 9:00:00", "2025-12-11 10:30:00", "Jean Hom birthday call", true, ""},
        {"2025-12-18 11:00:00", "2025-12-18 11:15:00", "Dina birthday call", true, ""}
    };

    for (auto testScheduleItem: testScheduleItemInput)
    {
        testInput.push_back(testScheduleItem);
    }

    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathScheduleItemInsertions, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathGetScheduleItemsForUserWithSimilarContentDateRange, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathGetScheduleItemsForUserByDate, this));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::negativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testNegativePathAlreadyInDataBase, this));
}

TestStatus TestScheduleItemModel::testInsertScheduleItem(TestScheduleItemInput testScheduleItem)
{
    UserModel_shp userOne = std::make_shared<UserModel>();
    userOne->selectByUserID(1);
    ScheduleItemModel newScheduleItem;
    newScheduleItem.setUserID(userOne->getUserID());
    newScheduleItem.setTitle(testScheduleItem.title);
    newScheduleItem.setStartDateAndTime(constantStringToChronoTimePoint(testScheduleItem.startTimeStr));
    newScheduleItem.setEndDateAndTime(constantStringToChronoTimePoint(testScheduleItem.endTimeStr));
    newScheduleItem.setPersonal(testScheduleItem.personal);
    newScheduleItem.setLocation(testScheduleItem.location);

    if (!newScheduleItem.insert())
    {
        std::cout << "Insertion failed for ScheduleItem: " << newScheduleItem << " :\n";
        std::cout << newScheduleItem.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus TestScheduleItemModel::testPositivePathScheduleItemInsertions()
{
    TestStatus testStatus = TESTPASSED;

    for (auto testScheduleItem: testInput)
    {
        TestStatus currentResult = testInsertScheduleItem(testScheduleItem);
        if (testStatus == TESTPASSED)
        {
            testStatus = currentResult;
        }
    }

    if (testStatus == TESTFAILED)
    {
        std::cerr << "Some or all ScheduleItem Insertions FAILED\n";
    }

    return testStatus;
}

TestStatus TestScheduleItemModel::testPositivePathGetScheduleItemsForUserWithSimilarContentDateRange()
{
    UserModel_shp userOne = std::make_shared<UserModel>();
    userOne->selectByUserID(1);
    std::chrono::year_month_day searchStart(constantStringToChronoDate("2024-12-1"));
    std::chrono::year_month_day searchEnd(constantStringToChronoDate("2024-12-31"));

    std::string searchString("birthday");
    ScheduleItemList ScheduleItemListTestInterface(userOne->getUserID());
    ScheduleItemListValues allSimilarUserScheduleItems = ScheduleItemListTestInterface.findUserScheduleItemsByContentAndDateRange(
        searchString, searchStart, searchEnd);

    if (allSimilarUserScheduleItems.empty())
    {
        std::cerr << "test of ScheduleItemListTestInterface.getScheduleItemsForUserSimlarToContent() FAILED\n" <<
            ScheduleItemListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all schedule itemss for user ({}) similar to {} PASSED!\n", 1, searchString);
        std::cout << std::format("User {} has {} schedule itemss\n", 1, allSimilarUserScheduleItems.size());
        for (auto scheduleitems: allSimilarUserScheduleItems)
        {
            std::cout << *scheduleitems << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestScheduleItemModel::testPositivePathGetScheduleItemsForUserByDate()
{
    UserModel_shp userOne = std::make_shared<UserModel>();
    userOne->selectByUserID(1);

    std::chrono::year_month_day startDate(constantStringToChronoDate("2024-12-13"));

    ScheduleItemList schediList(userOne->getUserID());
    ScheduleItemListValues allScheduleItemsInRange = schediList.getUserDaySchedule(startDate);

    if (allScheduleItemsInRange.empty())
    {
        std::cerr << "test of ScheduleItemListTestInterface.getAllScheduleItemsForUserEditedInDatgeRange() FAILED\n" <<
            schediList.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all schedule itemss for user ({}) edited in date range PASSED!\n", 1);
        std::cout << std::format("User {} has {} schedule itemss\n", 1, allScheduleItemsInRange.size());
        for (auto scheduleitem: allScheduleItemsInRange)
        {
            std::cout << *scheduleitem << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestScheduleItemModel::testNegativePathAlreadyInDataBase()
{
    ScheduleItemModel_shp scheduleitemsAlreadyInDB = std::make_shared<ScheduleItemModel>();
    scheduleitemsAlreadyInDB->setScheduleItemID(1);
    scheduleitemsAlreadyInDB->setUserID(1);
    if (!scheduleitemsAlreadyInDB->retrieve())
    {
        std::cout << "ScheduleItem 1 not found in database!!\n";
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(scheduleitemsAlreadyInDB, expectedErrors);
}

TestStatus TestScheduleItemModel::testnegativePathNotModified()
{
    ScheduleItemModel_shp scheduleitemsNotModified = std::make_shared<ScheduleItemModel>();
    scheduleitemsNotModified->setScheduleItemID(1);
    scheduleitemsNotModified->setUserID(1);
    if (!scheduleitemsNotModified->retrieve())
    {
        std::cout << "ScheduleItem 1 not found in database!!\n";
        return TESTFAILED;
    }

    scheduleitemsNotModified->setScheduleItemID(0); // Force it to check modified rather than Already in DB.
    scheduleitemsNotModified->clearModified();
    std::vector<std::string> expectedErrors = {"not modified!"};
    return testInsertionFailureMessages(scheduleitemsNotModified, expectedErrors);
}

TestStatus TestScheduleItemModel::negativePathMissingRequiredFields()
{
    TestStatus testStatus = TESTPASSED;

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Title", "Start Time", "End Time", " missing required values"
    };

    ScheduleItemModel testScheduleItem;
    testScheduleItem.setScheduleItemID(0);  // prevent the not modified error.

    std::vector<std::function<TestStatus(ScheduleItemModel&, std::vector<std::string>&)>> fieldTestUpdate =
    {
        {std::bind(&TestScheduleItemModel::testMissingRequiredFieldsAddUserID, this, std::placeholders::_1, std::placeholders::_2)},
        {std::bind(&TestScheduleItemModel::testMissingRequiredFieldsAddTitle, this, std::placeholders::_1, std::placeholders::_2)},
        {std::bind(&TestScheduleItemModel::testMissingRequiredFieldsAddStartTime, this, std::placeholders::_1, std::placeholders::_2)},
        {std::bind(&TestScheduleItemModel::testMissingRequiredFieldsAddEndTime, this, std::placeholders::_1, std::placeholders::_2)},
    };

    for (auto fieldTest: fieldTestUpdate)
    {
        TestStatus thisTestStatus = fieldTest(testScheduleItem, expectedErrors);
        if (testStatus == TESTPASSED)
        {
            testStatus = thisTestStatus;
        }
    }

    testScheduleItem.save();
    if (!testScheduleItem.isInDataBase())
    {
        std::cerr << testScheduleItem.getAllErrorMessages() << testScheduleItem << "\n";
        std::cerr << "Primary key for user: testScheduleItem.getScheduleItemID() not set!\n";
        if (verboseOutput)
        {
            std::cerr << testScheduleItem << "\n\n";
        }
        testStatus = TESTFAILED;
    }

    return testStatus;
}

TestStatus TestScheduleItemModel::testMissingRequiredFieldsAddUserID(
    ScheduleItemModel &testScheduleItem, std::vector<std::string>& expectedErrors)
{
    TestStatus testStatus = testInsertionFailureMessages(&testScheduleItem, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testScheduleItem.setUserID(1);

    return testStatus;
}

TestStatus TestScheduleItemModel::testMissingRequiredFieldsAddTitle(
    ScheduleItemModel &testScheduleItem, std::vector<std::string>& expectedErrors)
{
    TestStatus testStatus = testInsertionFailureMessages(&testScheduleItem, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testScheduleItem.setTitle("Test missing field Title");

    return testStatus;
}

TestStatus TestScheduleItemModel::testMissingRequiredFieldsAddStartTime(ScheduleItemModel &testScheduleItem, std::vector<std::string> &expectedErrors)
{
    TestStatus testStatus = testInsertionFailureMessages(&testScheduleItem, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testScheduleItem.setStartDateAndTime(constantStringToChronoTimePoint("2024-12-31 8:30:00"));

    return testStatus;
}

TestStatus TestScheduleItemModel::testMissingRequiredFieldsAddEndTime(ScheduleItemModel &testScheduleItem, std::vector<std::string> &expectedErrors)
{
    TestStatus testStatus = testInsertionFailureMessages(&testScheduleItem, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testScheduleItem.setEndDateAndTime(constantStringToChronoTimePoint("2024-12-31 10:00:00"));

    return testStatus;
}
