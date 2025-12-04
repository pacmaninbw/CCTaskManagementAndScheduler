#ifndef NOTESELFTEST_H_
#define NOTESELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#if 0
#include "AttributeSelfTest.h"
#include "ExceptionSelfTest.h"
#include "FunctionalSelfTest.h"
#else
#include "ModelSelfTest.h"
#endif
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
    TestStatus runSelfTest() noexcept;

private:
    
// Unit test functions
    virtual void selfTestResetAllValues() noexcept override;

    virtual std::vector<AttributeTestFunction> initAttributeAccessTests() noexcept override;

    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept override;
    virtual TestStatus testExceptionInsert() noexcept override;
    virtual TestStatus testExceptionUpdate() noexcept override;
    virtual TestStatus testExceptionRetrieve() noexcept override;
    TestStatus testExceptionSelectByNoteID() noexcept;
    TestStatus testExceptionFormatSelectByUserId() noexcept;
    TestStatus testExceptionFormatSelectByUserIdAndSimilarContent() noexcept;
    TestStatus testExceptionFormatSelectByUserIdAndCreationDateRange() noexcept;
    TestStatus testExceptionFormatSelectByUserIdAndUpdateDateRange() noexcept;

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
