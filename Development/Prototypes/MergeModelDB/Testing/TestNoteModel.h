#ifndef TESTNOTEMODEL_H_
#define TESTNOTEMODEL_H_

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
    TestStatus testInsertAndGetParent(TestNoteInput testNote);
    TestStatus testPositivePathNoteInsertions();
    TestStatus testNegativePathAlreadyInDataBase();
    TestStatus testnegativePathNotModified();
    TestStatus negativePathMissingRequiredFields();
    TestStatus testMissingRequiredFieldsAddUserID(
        NoteModel& testNote, std::vector<std::string>& expectedErrors);
    TestStatus testMissingRequiredFieldsAddContent(
        NoteModel& testNote, std::vector<std::string>& expectedErrors);

    bool verboseOutput;
    std::vector<std::function<bool(NoteModel_shp)>>positiveTestFuncs;
    std::vector<std::function<bool(void)>> negativePathTestFuncs;
    std::vector<TestNoteInput> testInput;
};

#endif // TESTNOTEMODEL_H_

