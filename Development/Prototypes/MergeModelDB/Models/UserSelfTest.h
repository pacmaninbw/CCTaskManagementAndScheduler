#ifndef USERSELFTEST_H_
#define USERSELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelSelfTest.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class UserSelfTest : public ModelSelfTest<UserModel>
{
public:
    UserSelfTest();
    ~UserSelfTest() = default;


private:
    
// Unit test functions
    virtual void selfTestResetAllValues() override;
    virtual std::vector<std::function<TestStatus(void)>> initAttributeAccessTests() noexcept override;
    TestStatus testUserIdAccesss() noexcept;
    TestStatus testLastNameAccess() noexcept;
    TestStatus testFirstNameAccess() noexcept;
    TestStatus testMiddleInitialAccess() noexcept;
    TestStatus testLoginNameAccess() noexcept;
    TestStatus testPassWordAccess() noexcept;
    TestStatus testCreatedDateAcfcess() noexcept;
    TestStatus testLastLoginAccess() noexcept;
    TestStatus testEmailAccess() noexcept;
    TestStatus testStartTimeAccess() noexcept;
    TestStatus testEndTimeAccesss() noexcept;
    TestStatus testIncludePriorityInScheduleAccess() noexcept;
    TestStatus testIncludeMinorPriorityInScheduleAccess() noexcept;
    TestStatus testUseLetterForMajorPriorityAccess() noexcept;
    TestStatus testSeparateMajorAndMinorWithDotAccess() noexcept;


    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept override;
    bool testExceptionSelectByLoginName() noexcept;
    bool testExceptionSelectByEmail() noexcept;
    bool testExceptionSelectByLoginAndPassword() noexcept;
    bool testExceptionSelectByFullName() noexcept;
    bool testExceptionSelectByUserID() noexcept;
    bool testExceptionFormatGetAllUsersQuery() noexcept;
    virtual bool testExceptionInsert() noexcept override;
    virtual bool testExceptionUpdate() noexcept override;
    virtual bool testExceptionRetrieve() noexcept override;

    virtual TestStatus testAllInsertFailures() override;
    virtual TestStatus testEqualityOperator() noexcept override;
    virtual void testOutput() noexcept override;

};

using UserSelfTest_shp = std::shared_ptr<UserSelfTest>;

#endif // USERSELFTEST_H_
