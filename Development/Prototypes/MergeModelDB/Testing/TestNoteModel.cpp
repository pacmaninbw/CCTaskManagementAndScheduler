// Project Header Files
#include "TestNoteModel.h"
#include "TestDBInterfaceCore.h"
#include "NoteQueryProcessor.h"
#include "NoteModel.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <string>
#include <vector>

TestNoteModel::TestNoteModel()
: TestDBInterfaceCore("Note")
{
    std::vector<TestNoteInput> testNoteInput =
    {
        {"Get a Job in Software Engineering"},
        {"Maintain and improve my software engineering skills"},
        {"Maintain and improve my C++ programming skills"},
        {"Maintain and improve my  C programming skills"},
        {"Maintain or archive the BHHS 74 Reunion website"},
        {"Maintain and improve my SQL skills"},
        {"Learn QT graphic system"},
        {"Improve my Linux Programming skills"},
        {"Get an embedded programming certificate"},
        {"Learn open source Linux Core Programming"},
        {"Maintain my health"},
        {"Eat as healthy as possible"},
        {"Walk 30 minutes a day"},
        {"Track my vital signs"},
        {"Get my weight down to 190 pounds"},
        {"Be more organized"},
        {"Effectively moderate the code review site on stack exchange"}
    };

    for (auto testNote: testNoteInput)
    {
        testInput.push_back(testNote);
    }

    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathNoteInsertions, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathNoteUpdate, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathGetAllNotesForUser, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathGetNotesForUserWithSimilarContent, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathGetNotesForUserCreatedDateRange, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathGetNotesForUserEditedDateRange, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathGetDashboardNoteTable, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathDeleteNote, this));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::negativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testNegativePathAlreadyInDataBase, this));

    userOne = std::make_shared<UserModel>();
    userOne->selectByFullName("One", "User", "P");;

}

TestStatus TestNoteModel::testInsertNote(TestNoteInput testNote)
{
    NoteModel newNote;
    newNote.setUserId(userOne->getUserID());
    newNote.setContent(testNote.content);
    newNote.setDateAdded(commonTestTimeStampValue);
    newNote.setLastModified(commonTestTimeStampValue);

    if (!newNote.insert())
    {
        std::cout << "Insertion failed for Note: " << newNote << " :\n";
        std::cout << newNote.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus TestNoteModel::testPositivePathNoteInsertions()
{
    TestStatus testStatus = TESTPASSED;

    for (auto testNote: testInput)
    {
        TestStatus currentResult = testInsertNote(testNote);
        if (testStatus == TESTPASSED)
        {
            testStatus = currentResult;
        }
    }

    if (testStatus == TESTFAILED)
    {
        std::cerr << "Some or all Note Insertions FAILED\n";
    }

    return testStatus;
}

TestStatus TestNoteModel::testPositivePathNoteUpdate()
{
    std::string searchString("Be more organized");
    NoteQueryProcessor NoteQueryProcessorTestInterface;
    NoteQueryProcessorValues allSimilarUserNotes = NoteQueryProcessorTestInterface.getNotesForUserSimlarToContent(userOne->getUserID(), searchString);

    if (allSimilarUserNotes.empty())
    {
        std::cerr << "testPositivePathNoteUpdate: NoteQueryProcessorTestInterface.getNotesForUserSimlarToContent() FAILED\n" <<
            NoteQueryProcessorTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    NoteModel_shp noteToUpdate = allSimilarUserNotes[0];
    noteToUpdate->setContent("Use a planner / organizer");
    if (!noteToUpdate->update())
    {
        std::cerr << "testPositivePathNoteUpdate: noteToUpdate->update() FAILED\n" <<
            noteToUpdate->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << "Update UserNote Passed integration test\n";
        std::cout << *noteToUpdate << "\n";
    }

    return TESTPASSED;
}

TestStatus TestNoteModel::testPositivePathGetAllNotesForUser()
{
    NoteQueryProcessor NoteQueryProcessorTestInterface;
    NoteQueryProcessorValues allUserNotes = NoteQueryProcessorTestInterface.getAllNotesForUser(userOne->getUserID());

    if (allUserNotes.empty())
    {
        std::cerr << "test of NoteQueryProcessorTestInterface.getAllNotesForUser() FAILED\n" <<
            NoteQueryProcessorTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all notes for user ({}) PASSED!\n", userOne->getUserID());
        std::cout << std::format("User {} has {} notes\n", userOne->getUserID(), allUserNotes.size());
        for (auto note: allUserNotes)
        {
            std::cout << *note << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestNoteModel::testPositivePathGetNotesForUserWithSimilarContent()
{
    std::string searchString("Maintain");
    NoteQueryProcessor NoteQueryProcessorTestInterface;
    NoteQueryProcessorValues allSimilarUserNotes = NoteQueryProcessorTestInterface.getNotesForUserSimlarToContent(userOne->getUserID(), searchString);

    if (allSimilarUserNotes.empty())
    {
        std::cerr << "test of NoteQueryProcessorTestInterface.getNotesForUserSimlarToContent() FAILED\n" <<
            NoteQueryProcessorTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all notes for user ({}) similar to {} PASSED!\n", userOne->getUserID(), searchString);
        std::cout << std::format("User {} has {} notes\n", userOne->getUserID(), allSimilarUserNotes.size());
        for (auto note: allSimilarUserNotes)
        {
            std::cout << *note << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestNoteModel::testPositivePathGetNotesForUserCreatedDateRange()
{
    std::chrono::year_month_day startDate = getTodaysDateMinus(OneWeek);
    std::chrono::year_month_day endDate = getTodaysDatePlus(OneWeek);

    NoteQueryProcessor NoteQueryProcessorTestInterface;
    NoteQueryProcessorValues allNotesInRange = NoteQueryProcessorTestInterface.getAllNotesForUserCreatedInDatgeRange(userOne->getUserID(), startDate, endDate);

    if (allNotesInRange.empty())
    {
        std::cerr << "test of NoteQueryProcessorTestInterface.getAllNotesForUserCreatedInDatgeRange() FAILED\n" <<
            NoteQueryProcessorTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all notes for user ({}) created in date range PASSED!\n", userOne->getUserID());
        std::cout << std::format("User {} has {} notes\n", userOne->getUserID(), allNotesInRange.size());
        for (auto note: allNotesInRange)
        {
            std::cout << *note << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestNoteModel::testPositivePathGetNotesForUserEditedDateRange()
{
    std::chrono::year_month_day startDate = getTodaysDateMinus(OneWeek);
    std::chrono::year_month_day endDate = getTodaysDatePlus(OneWeek);

    NoteQueryProcessor NoteQueryProcessorTestInterface;
    NoteQueryProcessorValues allNotesInRange = NoteQueryProcessorTestInterface.getAllNotesForUserEditedInDatgeRange(userOne->getUserID(), startDate, endDate);

    if (allNotesInRange.empty())
    {
        std::cerr << "test of NoteQueryProcessorTestInterface.getAllNotesForUserEditedInDatgeRange() FAILED\n" <<
            NoteQueryProcessorTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all notes for user ({}) edited in date range PASSED!\n", userOne->getUserID());
        std::cout << std::format("User {} has {} notes\n", userOne->getUserID(), allNotesInRange.size());
        for (auto note: allNotesInRange)
        {
            std::cout << *note << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestNoteModel::testPositivePathGetDashboardNoteTable()
{
    std::chrono::year_month_day searchDate = getTodaysDate();

    NoteQueryProcessor NoteQueryProcessorTestInterface;
    NoteQueryProcessorValues allNotesInRange = NoteQueryProcessorTestInterface.getDashboardNoteTable(userOne->getUserID(), searchDate);

    if (allNotesInRange.empty())
    {
        std::cerr << "test of NoteQueryProcessorTestInterface.getDashboardNoteTable() FAILED\n" <<
            NoteQueryProcessorTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all notes for user ({}) for a date PASSED!\n", userOne->getUserID());
        std::cout << std::format("User {} has {} notes\n", userOne->getUserID(), allNotesInRange.size());
        for (auto note: allNotesInRange)
        {
            std::cout << *note << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestNoteModel::testPositivePathDeleteNote()
{
    std::string funcUnderTest("Delete Note");

    std::chrono::year_month_day testDate(constantStringToChronoDate("2026-03-08"));
    NoteQueryProcessor NoteQueryProcessorTestInterface;
    NoteQueryProcessorValues testNoteQueryProcessor = NoteQueryProcessorTestInterface.getAllNotesForUser(userOne->getUserID());
    if (testNoteQueryProcessor.empty())
    {
        std::cerr << std::format("{}: {} {} FAILED\n", funcUnderTest, "userOne schedule is empty", NoteQueryProcessorTestInterface.getAllErrorMessages());
        return TESTFAILED;
    }

    std::size_t itemToHideIndex = testNoteQueryProcessor.size() > 3? testNoteQueryProcessor.size() - 2 : testNoteQueryProcessor.size() - 1;
    NoteModel_shp noteToHide = testNoteQueryProcessor[itemToHideIndex];
    if (!noteToHide->hide(userOne->getUserID()))
    {
        std::cerr << std::format("itemToHide->hide({}) FAILED!", userOne->getUserID()) << noteToHide->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (!noteToHide->isDeleted())
    {
        std::cerr << std::format("{}: note ({}) was not marked as deleted {} FAILED\n", funcUnderTest,
            noteToHide->getNoteId(), noteToHide->getAllErrorMessages());
        return TESTFAILED;
    }

    NoteQueryProcessorValues alteredList = NoteQueryProcessorTestInterface.getAllNotesForUser(userOne->getUserID());
    if (!(alteredList.size() < testNoteQueryProcessor.size()))
    {
        std::cerr << std::format("Deleted note ({}) did not decrease the size of the user note list. TEST FAILED\n",
            noteToHide->getNoteId());
        return TESTFAILED;
    }

    for (auto itemInList: alteredList)
    {
        if (itemInList->getNoteId() == noteToHide->getNoteId())
        {
            std::cerr << "The wrong note was deleted. TEST FAILED\n";
            return TESTFAILED;
        }
    }

    if (programOptions.verboseOutput)
    {
        std::cout << "Original note list size: " << testNoteQueryProcessor.size() << " Altered schedule size: " << alteredList.size() << "\n";
        std::cout << std::format("note ({}) for user ({}) marked Deleted. TEST PASSED\n",
            noteToHide->getNoteId(), userOne->getUserID());
    }

    return TESTPASSED;
}

TestStatus TestNoteModel::testNegativePathAlreadyInDataBase()
{
    NoteModel_shp noteAlreadyInDB = std::make_shared<NoteModel>();
    noteAlreadyInDB->setNoteId(1);
    if (!noteAlreadyInDB->retrieve())
    {
        std::cout << "Note 1 not found in database!!\n";
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(noteAlreadyInDB, expectedErrors);
}

TestStatus TestNoteModel::testnegativePathNotModified()
{
    NoteModel_shp noteNotModified = std::make_shared<NoteModel>();
    noteNotModified->setNoteId(1);
    if (!noteNotModified->retrieve())
    {
        std::cout << "Note 1 not found in database!!\n";
        return TESTFAILED;
    }

    noteNotModified->setNoteId(0); // Force it to check modified rather than Already in DB.
    noteNotModified->clearModified();
    std::vector<std::string> expectedErrors = {"not modified!"};
    return testInsertionFailureMessages(noteNotModified, expectedErrors);
}

TestStatus TestNoteModel::negativePathMissingRequiredFields()
{
    TestStatus testStatus = TESTPASSED;

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Content", "missing required values"
    };

    NoteModel testNote;
    testNote.setNoteId(0);  // prevent the not modified error.

    std::vector<std::function<TestStatus(NoteModel&, std::vector<std::string>&)>> fieldTestUpdate =
    {
        {std::bind(&TestNoteModel::testMissingRequiredFieldsAddUserID, this, std::placeholders::_1, std::placeholders::_2)},
        {std::bind(&TestNoteModel::testMissingRequiredFieldsAddContent, this, std::placeholders::_1, std::placeholders::_2)},
    };

    for (auto fieldTest: fieldTestUpdate)
    {
        TestStatus thisTestStatus = fieldTest(testNote, expectedErrors);
        if (testStatus == TESTPASSED)
        {
            testStatus = thisTestStatus;
        }
    }

    testNote.setDateAdded(commonTestTimeStampValue);
    testNote.setLastModified(commonTestTimeStampValue);
    testNote.save();
    if (!testNote.isInDataBase())
    {
        std::cout << testNote.getAllErrorMessages() << testNote << "\n";
        std::cout << "Primary key for user: " << testNote.getNoteId() << " not set!\n";
        if (verboseOutput)
        {
            std::cout << testNote << "\n\n";
        }
        testStatus = TESTFAILED;
    }

    return testStatus;
}

TestStatus TestNoteModel::testMissingRequiredFieldsAddUserID(
    NoteModel &testNote, std::vector<std::string>& expectedErrors)
{
    TestStatus testStatus =
        testInsertionFailureMessages(&testNote, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testNote.setUserId(userOne->getUserID());

    return testStatus;
}

TestStatus TestNoteModel::testMissingRequiredFieldsAddContent(
    NoteModel &testNote, std::vector<std::string>& expectedErrors)
{
    TestStatus testStatus =
        testInsertionFailureMessages(&testNote, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testNote.setContent("Test missing field Content");

    return testStatus;
}

