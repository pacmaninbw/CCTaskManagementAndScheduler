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
#include <optional>
#include <string>
#include <vector>

class NoteSelfTest : public ModelSelfTest<NoteModel>
{
public:
    NoteSelfTest();
    ~NoteSelfTest() = default;
    virtual TestStatus runSelfTest() override;

private:
    
// Unit test functions
    virtual void selfTestResetAllValues() override;
    virtual bool testAccessorFunctionsPassed() override;

    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept override;
    virtual bool testExceptionInsert() noexcept override;
    virtual bool testExceptionUpdate() noexcept override;
    virtual bool testExceptionRetrieve() noexcept override;
    bool testExceptionSelectByNoteID() noexcept;

    virtual TestStatus testAllInsertFailures() override;

    bool testNoteIdAccesss() noexcept;
    bool testUserIdAccesss() noexcept;
    bool testContentAccess() noexcept;
    bool testDateAddedAccess() noexcept;
    bool testLastUpdateAccess() noexcept;

    virtual TestStatus testEqualityOperator() noexcept override;
};


#endif // NOTESELFTEST_H_
