#ifndef TESTNOTEMODEL_H_
#define TESTNOTEMODEL_H_

class UserModel;

// Project Header Files
#include "TestDBInterfaceCore.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <functional>
#include <string>
#include <vector>

class TestNoteModel : public TestDBInterfaceCore
{
public:
    struct TestNoteInput
    {
        std::string content;
    };

    TestNoteModel();
    ~TestNoteModel() = default;

private:
    TestStatus testInsertNote(TestNoteInput testNote);
    TestStatus testPositivePathNoteInsertions();
    TestStatus testPositivePathNoteUpdate();
    TestStatus testPositivePathGetAllNotesForUser();
    TestStatus testPositivePathGetNotesForUserWithSimilarContent();
    TestStatus testPositivePathGetNotesForUserCreatedDateRange();
    TestStatus testPositivePathGetNotesForUserEditedDateRange();
    TestStatus testPositivePathGetDashboardNoteTable();
    TestStatus testPositivePathDeleteNote();
    TestStatus testNegativePathAlreadyInDataBase();
    TestStatus testnegativePathNotModified();
    TestStatus negativePathMissingRequiredFields();
    TestStatus testMissingRequiredFieldsAddUserID(
        NoteModel& testNote, std::vector<std::string>& expectedErrors);
    TestStatus testMissingRequiredFieldsAddContent(
        NoteModel& testNote, std::vector<std::string>& expectedErrors);

    std::vector<TestNoteInput> m_testInput;
    std::shared_ptr<UserModel> m_userOne;
};

#endif // TESTNOTEMODEL_H_

