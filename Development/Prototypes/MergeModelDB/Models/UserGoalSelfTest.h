#ifndef USERGOALSELFTEST_H_
#define USERGOALSELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelSelfTest.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

class UserGoalSelfTest : public ModelSelfTest<UserGoalModel>
{
public:
    UserGoalSelfTest();
    ~UserGoalSelfTest() = default;
    virtual TestStatus runSelfTest() override;

private:
    
// Unit test functions
    virtual void selfTestResetAllValues() override;
    virtual bool testAccessorFunctionsPassed() override;
    bool testGoalIdAccesss() noexcept;
    bool testUserIdAccesss() noexcept;
    bool testDescriptionAccess() noexcept;
    bool testCreationDateAccess() noexcept;
    bool testParentIdAccess() noexcept;
    bool testPriorityAccess() noexcept;

    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept override;
    virtual bool testExceptionInsert() noexcept override;
    virtual bool testExceptionUpdate() noexcept override;
    virtual bool testExceptionRetrieve() noexcept override;
    bool testExceptionSelectByGoalID() noexcept;
    bool testExceptionSelectByUserIDAndDescription() noexcept;
    bool testExceptionFormatSelectAllByUserId() noexcept;
    bool testExceptionFormatSelectAllChildGoalsWithParentFromUser() noexcept;
    bool testExceptionFormatSelectAllChildGoalsWithParent() noexcept;
    bool testExceptionFormatSelectByExactDescription() noexcept;
    bool testExceptionFormatSelectBySimilarDescription() noexcept;

    virtual TestStatus testAllInsertFailures() override;
    virtual TestStatus testEqualityOperator() noexcept override;
};

#endif // USERGOALSELFTEST_H_

