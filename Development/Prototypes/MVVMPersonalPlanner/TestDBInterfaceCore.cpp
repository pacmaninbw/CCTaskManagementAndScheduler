#include "BoostDBInterfaceCore.h"
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include "TestDBInterfaceCore.h"
#include <vector>

TestDBInterfaceCore::TestDBInterfaceCore(
    BoostDBInterfaceCore& dbInterfaceToTest, bool isVerboseOutput, std::string_view modelName)
: verboseOutput{isVerboseOutput}, modelUnderTest{modelName}
{
    dbInterfaceUnderTest = &dbInterfaceToTest;
}

TestDBInterfaceCore::TestStatus TestDBInterfaceCore::runAllTests()
{
    dbInterfaceUnderTest->initFormatOptions();
    TestDBInterfaceCore::TestStatus positivePathPassed = runPositivePathTests();
    TestDBInterfaceCore::TestStatus negativePathPassed = runNegativePathTests();
    
    if (positivePathPassed == TESTPASSED && negativePathPassed == TESTPASSED)
    {
        std::clog << std::format(
            "All tests for database insertions and retrievals of {} PASSED!\n",
            modelUnderTest);
        return TESTPASSED;
    }

    std::clog << std::format(
        "Some or all tests for database insertions and retrievals of {} FAILED!\n",
        modelUnderTest);
    return TESTFAILED;
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

    if (allTestPassed == TESTPASSED)
    {
        std::clog << std::format(
            "All negative path tests for database insertions and retrievals of {} PASSED!\n",
            modelUnderTest);
    }
    else
    {
        std::clog << std::format(
            "Some or all negative path tests for database insertions and retrievals of {} FAILED!\n",
            modelUnderTest);
    }

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

    if (allTestPassed == TESTPASSED)
    {
        std::clog << std::format(
            "All positive path tests for database insertions and retrievals of {} PASSED!\n",
            modelUnderTest);
    }
    else
    {
        std::clog << std::format(
            "Some or all positive path tests for database insertions and retrievals of {} FAILED!\n",
            modelUnderTest);
    }

    return allTestPassed;
}

/*
 * Protected methods.
 */
TestDBInterfaceCore::TestStatus TestDBInterfaceCore::wrongErrorMessage(std::string expectedString)
{
    std::string errorMessage = dbInterfaceUnderTest->getAllErrorMessages();
    std::size_t found = errorMessage.find(expectedString);
    if (found == std::string::npos)
    {
        std::clog << "Wrong message generated! TEST FAILED!\n";
        std::clog << errorMessage << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

bool TestDBInterfaceCore::hasErrorMessage()
{
    std::string errorMessage = dbInterfaceUnderTest->getAllErrorMessages();

    if (errorMessage.empty())
    {
        std::clog << "No error message generated! TEST FAILED!\n";
        return false;
    }

    return true;
}

bool TestDBInterfaceCore::insertionWasSuccessfull(std::size_t primaryKey)
{
    if (primaryKey > 0)
    {
        std::clog << std::format("Inserted {} missing required fields!  TEST FAILED\n", modelUnderTest);
        return true;
    }

    return false;
}

TestDBInterfaceCore::TestStatus TestDBInterfaceCore::testInsertionFailureMessages(std::size_t primaryKey, std::vector<std::string> expectedErrors)
{
    if (insertionWasSuccessfull(primaryKey))
    {
        return TESTFAILED;
    }

    if (!hasErrorMessage())
    {
        return TESTFAILED;
    }

    for (auto expectedError: expectedErrors)
    {
        if (wrongErrorMessage(expectedError) == TESTFAILED)
        {
            return TESTFAILED;
        }
    }

    return TESTPASSED;
}
