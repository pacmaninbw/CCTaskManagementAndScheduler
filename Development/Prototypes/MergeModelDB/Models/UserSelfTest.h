#ifndef USERSELFTEST_H_
#define USERSELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelSelfTest.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <functional>
#include <memory>
#include <vector>

class UserSelfTest : public ModelSelfTest<UserModel>
{
public:
    UserSelfTest();
    ~UserSelfTest() = default;
    virtual TestStatus runSelfTest() noexcept override;

private:
    
// Unit test functions
    virtual void selfTestResetAllValues() noexcept override;
    virtual std::vector<AttributeTestFunction> initAttributeAccessTests() noexcept override;
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
    TestStatus testExceptionSelectByLoginName() noexcept;
    TestStatus testExceptionSelectByEmail() noexcept;
    TestStatus testExceptionSelectByLoginAndPassword() noexcept;
    TestStatus testExceptionSelectByFullName() noexcept;
    TestStatus testExceptionSelectByUserID() noexcept;
    TestStatus testExceptionFormatGetAllUsersQuery() noexcept;
    virtual TestStatus testExceptionInsert() noexcept override;
    virtual TestStatus testExceptionUpdate() noexcept override;
    virtual TestStatus testExceptionRetrieve() noexcept override;

    virtual TestStatus testAllInsertFailures() override;
    virtual TestStatus testEqualityOperator() noexcept override;
    virtual void testOutput() noexcept override;

};

using UserSelfTest_shp = std::shared_ptr<UserSelfTest>;

#endif // USERSELFTEST_H_
