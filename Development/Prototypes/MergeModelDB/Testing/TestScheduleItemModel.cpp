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
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathGetScheduleItemsForUserWithSimilarTitleDateRange, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathGetScheduleItemsForUserByDate, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathUpdateScheduleItem, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathFindEventSToRepeat, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathFindEventsToRepeatCompletion, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathFindLocationsToRepeatCompletion, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testPositivePathDeleteScheduleItem, this));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::negativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestScheduleItemModel::testNegativePathAlreadyInDataBase, this));

    userOne = std::make_shared<UserModel>();
    userOne->selectByFullName("One", "User", "P");
}

TestStatus TestScheduleItemModel::testInsertScheduleItem(TestScheduleItemInput testScheduleItem)
{
    ScheduleItemModel newScheduleItem;
    newScheduleItem.setUserID(userOne->getUserID());
    newScheduleItem.setTitle(testScheduleItem.title);
    newScheduleItem.setStartDateAndTime(constantStringToChronoTimePoint(testScheduleItem.startTimeStr));
    newScheduleItem.setEndDateAndTime(constantStringToChronoTimePoint(testScheduleItem.endTimeStr));
    newScheduleItem.setPersonal(testScheduleItem.personal);
    newScheduleItem.setLocation(testScheduleItem.location);
    newScheduleItem.setCreationDate(commonTestTimeStampValue);
    newScheduleItem.setLastUpdate(commonTestTimeStampValue);

    if (!newScheduleItem.insert())
    {
        std::cout << "Insertion failed for ScheduleItem: " << newScheduleItem << " :\n";
        std::cout << newScheduleItem.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (userOneFirstScheduleItem == 0)
    {
        userOneFirstScheduleItem = newScheduleItem.getScheduleItemID();
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

TestStatus TestScheduleItemModel::testPositivePathUpdateScheduleItem()
{
    TestStatus testStatus = TESTPASSED;

    ScheduleItemModel_shp scheduleitemToUpdate = std::make_shared<ScheduleItemModel>();
    scheduleitemToUpdate->setScheduleItemID(userOneFirstScheduleItem);
    scheduleitemToUpdate->setUserID(userOne->getUserID());
    if (!scheduleitemToUpdate->retrieve())
    {
        std::cout << "ScheduleItem 1 not found in database!!\n";
        return TESTFAILED;
    }

    scheduleitemToUpdate->setTitle("Dr. David Aftergood");
    scheduleitemToUpdate->setLocation("99. N. La Cienega");
    scheduleitemToUpdate->setPersonal(false);

    if (!scheduleitemToUpdate->update())
    {
        std::cout << "Update failed for ScheduleItem: " << *scheduleitemToUpdate << " :\n";
        std::cout << scheduleitemToUpdate->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    return testStatus;
}


TestStatus TestScheduleItemModel::testPositivePathGetScheduleItemsForUserWithSimilarTitleDateRange()
{
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
        std::cout << std::format("Find all schedule items for user ({}) similar to {} PASSED!\n", userOne->getUserID(), searchString);
        std::cout << std::format("User {} has {} schedule items\n", userOne->getUserID(), allSimilarUserScheduleItems.size());
        for (auto scheduleitems: allSimilarUserScheduleItems)
        {
            std::cout << *scheduleitems << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestScheduleItemModel::testPositivePathGetScheduleItemsForUserByDate()
{
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
        std::cout << std::format("Find all schedule items for user ({}) edited in date range PASSED!\n", userOne->getUserID());
        std::cout << std::format("User {} has {} schedule items\n", userOne->getUserID(), allScheduleItemsInRange.size());
        for (auto scheduleitem: allScheduleItemsInRange)
        {
            std::cout << *scheduleitem << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestScheduleItemModel::testPositivePathFindEventSToRepeat()
{
    std::string searchString("birthday");
    ScheduleItemList ScheduleItemListTestInterface(userOne->getUserID());
    std::vector<std::string> matchingEvents = ScheduleItemListTestInterface.findEventSToRepeat(searchString);

    if (matchingEvents.empty())
    {
        std::cerr << "test of ScheduleItemListTestInterface.getScheduleItemsForUserSimlarToContent() FAILED\n" <<
            ScheduleItemListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all schedule items for user ({}) similar to {} PASSED!\n", userOne->getUserID(), searchString);
        std::cout << std::format("User {} has {} schedule items\n", userOne->getUserID(), matchingEvents.size());
        for (auto scheduleitems: matchingEvents)
        {
            std::cout << scheduleitems << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestScheduleItemModel::testPositivePathFindEventsToRepeatCompletion()
{
    ScheduleItemList ScheduleItemListTestInterface(userOne->getUserID());
    std::vector<std::string> matchingEvents = ScheduleItemListTestInterface.findEventsForRepeatCompletion();

    if (matchingEvents.empty())
    {
        std::cerr << "test of ScheduleItemListTestInterface.findEventsForRepeatCompletion() FAILED\n" <<
            ScheduleItemListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all schedule items for user ({}) PASSED!\n", userOne->getUserID());
        std::cout << std::format("User {} has {} schedule items\n", userOne->getUserID(), matchingEvents.size());
        for (auto scheduleitems: matchingEvents)
        {
            std::cout << scheduleitems << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestScheduleItemModel::testPositivePathFindLocationsToRepeatCompletion()
{
    ScheduleItemList ScheduleItemListTestInterface(userOne->getUserID());
    std::vector<std::string> matchingLocations = ScheduleItemListTestInterface.findLocationsForRepeatCompletion();

    if (matchingLocations.empty())
    {
        std::cerr << "test of ScheduleItemListTestInterface.findLocationsForRepeatCompletion() FAILED\n" <<
            ScheduleItemListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all schedule locations for user ({}) PASSED!\n", userOne->getUserID());
        std::cout << std::format("User {} has {} schedule locations\n", userOne->getUserID(), matchingLocations.size());
        for (auto scheduleitems: matchingLocations)
        {
            std::cout << scheduleitems << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestScheduleItemModel::testPositivePathDeleteScheduleItem()
{
    std::string funcUnderTest("Delete Schedule Item");
    UserModel_shp realUserOne = std::make_shared<UserModel>();
    realUserOne->selectByFullName("Black", "Paul", "A");

    if (!realUserOne->isInDataBase())
    {
        std::cerr << std::format("{}: {} {} FAILED\n", funcUnderTest, "realUserOne not found in database", realUserOne->getAllErrorMessages());
        return TESTFAILED;
    }

    std::chrono::year_month_day testDate(constantStringToChronoDate("2026-03-08"));
    ScheduleItemList schediList(realUserOne->getUserID());
    ScheduleItemListValues testUserSchedule = schediList.getUserDaySchedule(testDate);
    if (testUserSchedule.empty())
    {
        std::cerr << std::format("{}: {} {} FAILED\n", funcUnderTest, "realUserOne schedule is empty", schediList.getAllErrorMessages());
        return TESTFAILED;
    }

    std::size_t itemToHideIndex = testUserSchedule.size() > 3? testUserSchedule.size() - 2 : testUserSchedule.size() - 1;
    ScheduleItemModel_shp itemToHide = testUserSchedule[itemToHideIndex];
    if (!itemToHide->hide(realUserOne->getUserID()))
    {
        std::cerr << std::format("itemToHide->hide({}) FAILED!", realUserOne->getUserID()) << itemToHide->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (!itemToHide->isDeleted())
    {
        std::cerr << std::format("{}: schedule item ({}) was not marked as deleted {} FAILED\n", funcUnderTest,
            itemToHide->getScheduleItemID(), itemToHide->getAllErrorMessages());
        return TESTFAILED;
    }

    ScheduleItemListValues alteredList = schediList.getUserDaySchedule(testDate);
    if (!(alteredList.size() < testUserSchedule.size()))
    {
        std::cerr << std::format("Deleted schedule item ({}) did not decrease the size of the user schedule. TEST FAILED\n",
            itemToHide->getScheduleItemID());
        return TESTFAILED;
    }

    for (auto itemInList: alteredList)
    {
        if (itemInList->getScheduleItemID() == itemToHide->getScheduleItemID())
        {
            std::cerr << "The wrong Schedule Item was deleted. TEST FAILED\n";
            return TESTFAILED;
        }
    }

    if (programOptions.verboseOutput)
    {
        std::cout << "Original schedule size: " << testUserSchedule.size() << " Altered schedule size: " << alteredList.size() << "\n";
        std::cout << std::format("Schedule Item ({}) for user ({}) marked Deleted. TEST PASSED\n",
            itemToHide->getScheduleItemID(), realUserOne->getUserID());
    }

    return TESTPASSED;
}


TestStatus TestScheduleItemModel::testNegativePathAlreadyInDataBase()
{
    ScheduleItemModel_shp scheduleitemsAlreadyInDB = std::make_shared<ScheduleItemModel>();
    scheduleitemsAlreadyInDB->setScheduleItemID(userOneFirstScheduleItem);
    scheduleitemsAlreadyInDB->setUserID(userOne->getUserID());
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
    scheduleitemsNotModified->setScheduleItemID(userOneFirstScheduleItem);
    scheduleitemsNotModified->setUserID(userOne->getUserID());
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

    testScheduleItem.setCreationDate(commonTestTimeStampValue);
    testScheduleItem.setLastUpdate(commonTestTimeStampValue);
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
    testScheduleItem.setUserID(userOne->getUserID());

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

