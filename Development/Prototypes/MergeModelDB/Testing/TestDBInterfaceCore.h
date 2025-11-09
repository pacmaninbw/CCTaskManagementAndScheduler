#ifndef TESTDBINTERFACECORE_H_
#define TESTDBINTERFACECORE_H_

// Project Header Files
#include "CommandLineParser.h"
#include "ModelDBInterface.h"
#include "TestStatus.h"

// StandardS C++ Header Files
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class TestDBInterfaceCore
{
public:
    TestDBInterfaceCore(std::string_view modelName);
    virtual ~TestDBInterfaceCore() = default;
    virtual TestStatus runAllTests();
    virtual TestStatus runNegativePathTests();
    virtual TestStatus runPositivePathTests();

protected:
    TestStatus wrongErrorMessage(std::string expectedString, ModelDBInterface* modelUnderTest);
    bool hasErrorMessage(ModelDBInterface* modelUnderTest);
    TestStatus testInsertionFailureMessages(
        ModelDBInterface* modelUnderTest, std::vector<std::string> expectedErrors);
    TestStatus testInsertionFailureMessages(
        std::shared_ptr<ModelDBInterface>modelUnderTest, std::vector<std::string> expectedErrors) {
            ModelDBInterface* ptr = modelUnderTest.get();
            return testInsertionFailureMessages(ptr, expectedErrors);
        };
    void reportTestStatus(TestStatus status, std::string_view path);

    bool verboseOutput;
    bool forceErrors;
    bool forceExceptions;
    bool runSelfTest;
    std::string_view modelUnderTest;
    std::vector<std::function<TestStatus(void)>> negativePathTestFuncsNoArgs;
    std::vector<std::function<TestStatus(void)>> positiviePathTestFuncsNoArgs;
};

#endif  // TESTDBINTERFACECORE_H_
