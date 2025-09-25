#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include "TestDBInterfaceCore.h"
#include <vector>

TestDBInterfaceCore::TestDBInterfaceCore(
    bool isVerboseOutput, std::string_view modelName)
: verboseOutput{isVerboseOutput}, modelUnderTest{modelName}
{
}

TestDBInterfaceCore::TestStatus TestDBInterfaceCore::runAllTests()
{
    TestDBInterfaceCore::TestStatus positivePathPassed = runPositivePathTests();
    TestDBInterfaceCore::TestStatus negativePathPassed = runNegativePathTests();
    
    TestDBInterfaceCore::TestStatus allTestsStatus =
        (positivePathPassed == TESTPASSED && negativePathPassed == TESTPASSED) ? TESTPASSED : TESTFAILED;

    reportTestStatus(allTestsStatus, "");

    return allTestsStatus;
}

TestDBInterfaceCore::TestStatus TestDBInterfaceCore::runNegativePathTests()
{
    TestDBInterfaceCore::TestStatus allTestPassed = TESTPASSED;

    for (auto test: negativePathTestFuncsNoArgs)
    {
        TestDBInterfaceCore::TestStatus testResult = test();
        if (allTestPassed == TESTPASSED)
        {
            allTestPassed = testResult;
        }
    }

    reportTestStatus(allTestPassed, "negative");

    return allTestPassed;
}

TestDBInterfaceCore::TestStatus TestDBInterfaceCore::runPositivePathTests()
{
    TestDBInterfaceCore::TestStatus allTestPassed = TESTPASSED;

    for (auto test: positiviePathTestFuncsNoArgs)
    {
        TestDBInterfaceCore::TestStatus testResult = test();
        if (allTestPassed == TESTPASSED)
        {
            allTestPassed = testResult;
        }
    }

    reportTestStatus(allTestPassed, "positive");

    return allTestPassed;
}

/*
 * Protected methods.
 */
TestDBInterfaceCore::TestStatus TestDBInterfaceCore::wrongErrorMessage(std::string expectedString, ModelDBInterface* modelUnderTest)
{
    std::string errorMessage = modelUnderTest->getAllErrorMessages();
    std::size_t found = errorMessage.find(expectedString);
    if (found == std::string::npos)
    {
        std::clog << "Wrong message generated! TEST FAILED!\n";
        std::clog << errorMessage << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

bool TestDBInterfaceCore::hasErrorMessage(ModelDBInterface* modelUnderTest)
{
    std::string errorMessage = modelUnderTest->getAllErrorMessages();

    if (errorMessage.empty())
    {
        std::clog << "No error message generated! TEST FAILED!\n";
        return false;
    }

    if (verboseOutput)
    {
        std::clog << "Expected error was; " << errorMessage << "\n";
    }

    return true;
}

TestDBInterfaceCore::TestStatus TestDBInterfaceCore::testInsertionFailureMessages(ModelDBInterface* modelUnderTest, std::vector<std::string> expectedErrors)
{
    if (modelUnderTest->insert())
    {
        std::clog << std::format("Inserted {} missing required fields!  TEST FAILED\n", modelUnderTest->getModelName());
        return TESTFAILED;
    }

    if (!hasErrorMessage(modelUnderTest))
    {
        return TESTFAILED;
    }

    for (auto expectedError: expectedErrors)
    {
        if (wrongErrorMessage(expectedError, modelUnderTest) == TESTFAILED)
        {
            return TESTFAILED;
        }
    }

    return TESTPASSED;
}

void TestDBInterfaceCore::reportTestStatus(TestDBInterfaceCore::TestStatus status, std::string_view path)
{
    std::string_view statusStr = status == TESTPASSED? "PASSED" : "FAILED";

    if (path.length() > 0)
    {
        std::clog << std::format(
            "All {} path tests for database insertions and retrievals of {} {}!\n",
            path, modelUnderTest, statusStr);
    }
    else
    {
        std::clog << std::format(
            "All tests for database insertions and retrievals of {} {}!\n",
            modelUnderTest, statusStr);

    }
}
