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
#include <memory>
#include <optional>
#include <string>
#include <vector>

class UserGoalSelfTest : public ModelSelfTest<UserGoalModel>
{
public:
    UserGoalSelfTest();
    ~UserGoalSelfTest() = default;
//    virtual bool runSelfTest() override;


private:
    
// Unit test functions
//    virtual void selfTestResetAllValues() override;
//    virtual bool testAccessorFunctionsPassed() override;

//    virtual bool testExceptionHandling() noexcept override;
//    virtual bool testExceptionInsert() noexcept override;
//    virtual bool testExceptionUpdate() noexcept override;
//    virtual bool testExceptionRetrieve() noexcept override;

//    virtual ModelTestStatus testAllInsertFailures() override;

};

#endif // USERGOALSELFTEST_H_
