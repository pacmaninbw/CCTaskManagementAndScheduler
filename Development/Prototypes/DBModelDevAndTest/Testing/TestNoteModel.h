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
    virtual bool databaseObjectInitAfterConstructor() override;
    TestStatus testInsertNote(TestNoteInput testNote) const;
    TestStatus testPositivePathNoteInsertions() const;
    TestStatus testPositivePathNoteUpdate() const;
    TestStatus testPositivePathGetAllNotesForUser() const;
    TestStatus testPositivePathGetNotesForUserWithSimilarContent() const;
    TestStatus testPositivePathGetNotesForUserCreatedDateRange() const;
    TestStatus testPositivePathGetNotesForUserEditedDateRange() const;
    TestStatus testPositivePathGetDashboardNoteTable() const;
    TestStatus testPositivePathDeleteNote() const;
    TestStatus testNegativePathAlreadyInDataBase() const;
    TestStatus testnegativePathNotModified() const;
    TestStatus negativePathMissingRequiredFields() const;
    TestStatus testMissingRequiredFieldsAddUserID(
        NoteModel& testNote, std::vector<std::string>& expectedErrors) const;
    TestStatus testMissingRequiredFieldsAddContent(
        NoteModel& testNote, std::vector<std::string>& expectedErrors) const;

    std::vector<TestNoteInput> m_testInput;
    std::shared_ptr<UserModel> m_userOne;
};

#endif // TESTNOTEMODEL_H_

