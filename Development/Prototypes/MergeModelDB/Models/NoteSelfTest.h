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

private:
    
// Unit test functions
    virtual void selfTestResetAllValues() override;

    virtual std::vector<std::function<TestStatus(void)>> initAttributeAccessTests() noexcept override;

    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept override;
    virtual bool testExceptionInsert() noexcept override;
    virtual bool testExceptionUpdate() noexcept override;
    virtual bool testExceptionRetrieve() noexcept override;
    bool testExceptionSelectByNoteID() noexcept;

    virtual TestStatus testAllInsertFailures() override;

    TestStatus testNoteIdAccesss() noexcept;
    TestStatus testUserIdAccesss() noexcept;
    TestStatus testContentAccess() noexcept;
    TestStatus testDateAddedAccess() noexcept;
    TestStatus testLastUpdateAccess() noexcept;

    virtual TestStatus testEqualityOperator() noexcept override;
    virtual void testOutput() noexcept override;
};


#endif // NOTESELFTEST_H_
