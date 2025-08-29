#ifndef TESTDBINTERFACECORE_H_
#define TESTDBINTERFACECORE_H_

#include "BoostDBInterfaceCore.h"
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class TestDBInterfaceCore
{
public:
    enum class TestStatus {TestPassed, TestFailed};
    TestDBInterfaceCore(BoostDBInterfaceCore& dbInterfaceToTest, bool isVerboseOutput, std::string_view modelName);
    virtual ~TestDBInterfaceCore() = default;
    virtual TestDBInterfaceCore::TestStatus runAllTests();
    virtual TestDBInterfaceCore::TestStatus runNegativePathTests();
    virtual TestDBInterfaceCore::TestStatus runPositivePathTests();

protected:
    TestDBInterfaceCore::TestStatus wrongErrorMessage(std::string expectedString);
    bool hasErrorMessage();
    bool insertionWasSuccessfull(std::size_t primaryKey);
    TestDBInterfaceCore::TestStatus testInsertionFailureMessages(
        std::size_t primaryKey, std::vector<std::string> expectedErrors);
    void reportTestStatus(TestDBInterfaceCore::TestStatus status, std::string_view path);

    const TestDBInterfaceCore::TestStatus TESTFAILED = TestDBInterfaceCore::TestStatus::TestFailed;
    const TestDBInterfaceCore::TestStatus TESTPASSED = TestDBInterfaceCore::TestStatus::TestPassed;

    BoostDBInterfaceCore* dbInterfaceUnderTest; 
    bool verboseOutput;
    std::string_view modelUnderTest;
    std::vector<std::function<TestDBInterfaceCore::TestStatus(void)>> negativePathTestFuncsNoArgs;
    std::vector<std::function<TestDBInterfaceCore::TestStatus(void)>> positiviePathTestFuncsNoArgs;
};

#endif  // TESTDBINTERFACECORE_H_