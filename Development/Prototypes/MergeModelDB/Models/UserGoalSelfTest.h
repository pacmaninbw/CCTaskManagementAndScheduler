#ifndef USERGOALSELFTEST_H_
#define USERGOALSELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelSelfTest.h"
#include "TestStatus.h"
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

private:
    
// Unit test functions
    virtual void selfTestResetAllValues() override;
    virtual std::vector<std::function<TestStatus(void)>> initAttributeAccessTests() noexcept override;
    TestStatus testGoalIdAccesss() noexcept;
    TestStatus testUserIdAccesss() noexcept;
    TestStatus testDescriptionAccess() noexcept;
    TestStatus testCreationDateAccess() noexcept;
    TestStatus testParentIdAccess() noexcept;
    TestStatus testPriorityAccess() noexcept;

    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept override;
    virtual TestStatus testExceptionInsert() noexcept override;
    virtual TestStatus testExceptionUpdate() noexcept override;
    virtual TestStatus testExceptionRetrieve() noexcept override;
    TestStatus testExceptionSelectByGoalID() noexcept;
    TestStatus testExceptionSelectByUserIDAndDescription() noexcept;
    TestStatus testExceptionFormatSelectAllByUserId() noexcept;
    TestStatus testExceptionFormatSelectAllChildGoalsWithParentFromUser() noexcept;
    TestStatus testExceptionFormatSelectAllChildGoalsWithParent() noexcept;
    TestStatus testExceptionFormatSelectByExactDescription() noexcept;
    TestStatus testExceptionFormatSelectBySimilarDescription() noexcept;

    virtual TestStatus testAllInsertFailures() override;
    virtual TestStatus testEqualityOperator() noexcept override;
    virtual void testOutput() noexcept override;
};

#endif // USERGOALSELFTEST_H_

