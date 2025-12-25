// Project Header Files
#include "TestNoteModel.h"
#include "TestDBInterfaceCore.h"
#include "NoteList.h"
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
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathGetAllNotesForUser, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathGetNotesForUserWithSimilarContent, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathGetNotesForUserCreatedDateRange, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testPositivePathGetNotesForUserEditedDateRange, this));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::negativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestNoteModel::testNegativePathAlreadyInDataBase, this));
}

TestStatus TestNoteModel::testInsertNote(TestNoteInput testNote)
{
    UserModel_shp userOne = std::make_shared<UserModel>();
    userOne->selectByUserID(1);
    NoteModel newNote;
    newNote.setUserId(userOne->getUserID());
    newNote.setContent(testNote.content);
    newNote.setDateAdded(std::chrono::system_clock::now());

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

TestStatus TestNoteModel::testPositivePathGetAllNotesForUser()
{
    NoteList NoteListTestInterface;
    NoteListValues allUserNotes = NoteListTestInterface.getAllNotesForUser(1);

    if (allUserNotes.empty())
    {
        std::cerr << "test of NoteListTestInterface.getAllNotesForUser() FAILED\n" <<
            NoteListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all notes for user ({}) PASSED!\n", 1);
        std::cout << std::format("User {} has {} notes\n", 1, allUserNotes.size());
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
    NoteList NoteListTestInterface;
    NoteListValues allSimilarUserNotes = NoteListTestInterface.getNotesForUserSimlarToContent(1, searchString);

    if (allSimilarUserNotes.empty())
    {
        std::cerr << "test of NoteListTestInterface.getNotesForUserSimlarToContent() FAILED\n" <<
            NoteListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all notes for user ({}) similar to {} PASSED!\n", 1, searchString);
        std::cout << std::format("User {} has {} notes\n", 1, allSimilarUserNotes.size());
        for (auto note: allSimilarUserNotes)
        {
            std::cout << *note << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestNoteModel::testPositivePathGetNotesForUserCreatedDateRange()
{
    std::chrono::year_month_day startDate = commonTestDateRangeStartValue;
    std::chrono::year_month_day endDate = commonTestDateValue;

    NoteList NoteListTestInterface;
    NoteListValues allNotesInRange = NoteListTestInterface.getAllNotesForUserCreatedInDatgeRange(1, startDate, endDate);

    if (allNotesInRange.empty())
    {
        std::cerr << "test of NoteListTestInterface.getAllNotesForUserCreatedInDatgeRange() FAILED\n" <<
            NoteListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all notes for user ({}) created in date range PASSED!\n", 1);
        std::cout << std::format("User {} has {} notes\n", 1, allNotesInRange.size());
        for (auto note: allNotesInRange)
        {
            std::cout << *note << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestNoteModel::testPositivePathGetNotesForUserEditedDateRange()
{
    std::chrono::year_month_day startDate = commonTestDateRangeStartValue;
    std::chrono::year_month_day endDate = commonTestDateValue;

    NoteList NoteListTestInterface;
    NoteListValues allNotesInRange = NoteListTestInterface.getAllNotesForUserEditedInDatgeRange(1, startDate, endDate);

    if (allNotesInRange.empty())
    {
        std::cerr << "test of NoteListTestInterface.getAllNotesForUserEditedInDatgeRange() FAILED\n" <<
            NoteListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all notes for user ({}) edited in date range PASSED!\n", 1);
        std::cout << std::format("User {} has {} notes\n", 1, allNotesInRange.size());
        for (auto note: allNotesInRange)
        {
            std::cout << *note << "\n";
        }
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
    testNote.setUserId(1);

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

