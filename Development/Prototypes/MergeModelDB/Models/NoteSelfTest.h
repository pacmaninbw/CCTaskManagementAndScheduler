#ifndef NOTESELFTEST_H_
#define NOTESELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelSelfTest.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class NoteSelfTest : public ModelSelfTest<NoteModel>
{
public:
    NoteSelfTest();
    ~NoteSelfTest() = default;
    virtual bool runSelfTest() override;


private:
    
// Unit test functions
    virtual void selfTestResetAllValues() override;
    virtual bool testAccessorFunctionsPassed() override;

    virtual bool testExceptionHandling() noexcept override;
    virtual bool testExceptionInsert() noexcept override;
    virtual bool testExceptionUpdate() noexcept override;
    virtual bool testExceptionRetrieve() noexcept override;

//    virtual ModelTestStatus testAllInsertFailures() override;

    bool testNoteIdAccesss() noexcept;
    bool testUserIdAccesss() noexcept;
    bool testContentAccess() noexcept;
    bool testDateAddedAccess() noexcept;
    bool testLastUpdateAccess() noexcept;

    bool diffTest() noexcept;
};


#endif // NOTESELFTEST_H_
