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
    virtual TestStatus testAttributeAccessFunctions() noexcept override;
    bool testUserIdAccesss() noexcept;
    bool testLastNameAccess() noexcept;
    bool testFirstNameAccess() noexcept;
    bool testMiddleInitialAccess() noexcept;
    bool testLoginNameAccess() noexcept;
    bool testPassWordAccess() noexcept;
    bool testCreatedDateAcfcess() noexcept;
    bool testLastLoginAccess() noexcept;
    bool testEmailAccess() noexcept;
    bool testStartTimeAccess() noexcept;
    bool testEndTimeAccesss() noexcept;
    bool testIncludePriorityInScheduleAccess() noexcept;
    bool testIncludeMinorPriorityInScheduleAccess() noexcept;
    bool testUseLetterForMajorPriorityAccess() noexcept;
    bool testSeparateMajorAndMinorWithDotAccess() noexcept;


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
