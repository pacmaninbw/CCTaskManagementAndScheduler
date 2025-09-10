#ifndef TESTDBINTERFACECORE_H_
#define TESTDBINTERFACECORE_H_

#include "ModelDBInterface.h"
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class TestDBInterfaceCore
{
public:
    enum class TestStatus {TestPassed, TestFailed};
    TestDBInterfaceCore(bool isVerboseOutput, std::string_view modelName);
    virtual ~TestDBInterfaceCore() = default;
    virtual TestDBInterfaceCore::TestStatus runAllTests();
    virtual TestDBInterfaceCore::TestStatus runNegativePathTests();
    virtual TestDBInterfaceCore::TestStatus runPositivePathTests();

protected:
    TestDBInterfaceCore::TestStatus wrongErrorMessage(std::string expectedString, ModelDBInterface* modelUnderTest);
    bool hasErrorMessage(ModelDBInterface* modelUnderTest);
    TestDBInterfaceCore::TestStatus testInsertionFailureMessages(
        ModelDBInterface* modelUnderTest, std::vector<std::string> expectedErrors);
    TestDBInterfaceCore::TestStatus testInsertionFailureMessages(
        std::shared_ptr<ModelDBInterface>modelUnderTest, std::vector<std::string> expectedErrors) {
            ModelDBInterface* ptr = modelUnderTest.get();
            return testInsertionFailureMessages(ptr, expectedErrors);
        };
    void reportTestStatus(TestDBInterfaceCore::TestStatus status, std::string_view path);

    const TestDBInterfaceCore::TestStatus TESTFAILED = TestDBInterfaceCore::TestStatus::TestFailed;
    const TestDBInterfaceCore::TestStatus TESTPASSED = TestDBInterfaceCore::TestStatus::TestPassed;

//    BoostDBInterfaceCore* dbInterfaceUnderTest; 
    bool verboseOutput;
    std::string_view modelUnderTest;
    std::vector<std::function<TestDBInterfaceCore::TestStatus(void)>> negativePathTestFuncsNoArgs;
    std::vector<std::function<TestDBInterfaceCore::TestStatus(void)>> positiviePathTestFuncsNoArgs;
};

#endif  // TESTDBINTERFACECORE_H_