#ifndef LISTSELFTEST_H_
#define LISTSELFTEST_H_

// Project Header Files
#include "ListDBInterface.h"
#include "TestStatus.h"

// Standard C++ Header Files
#include <concepts>
#include <exception>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

/*
 */
struct ListExceptionTestElement
{
    std::function<TestStatus(void)> testExceptionFunction;
    std::string_view functionUnderTest;
};

template<typename ListClass>
//requires std::is_base_of<ListDBInterface<ModelDBInterface>, ListClass>::value
class ListSelfTest : public ListClass
{
protected:
/*****
 * These methods must be implemented.
 * The tests they implement require the details of the particular model being tested.
 * 
 * Init functions should call the local version of selfTestResetAllValues().
 */

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept = 0;

public:
    ListSelfTest() : ListClass() { itemUnderTest = ListClass::getListTypeName(); }
    virtual ~ListSelfTest() = default;

    virtual TestStatus runSelfTest() noexcept = 0;

protected:

    std::string_view itemUnderTest;

    virtual void selfTestResetAllValues() noexcept
    {
        CoreDBInterface::inSelfTest = true;
        CoreDBInterface::format_opts.reset();
        CoreDBInterface::errorMessages.clear();

        ListClass::primaryKeyResults.clear();
        ListClass::returnType.clear();
        ListClass::queryGenerator.setSelfTest(true);
        ListClass::queryGenerator.clearModified();
        ListClass::queryGenerator.clearErrorMessages();
        ListClass::queryGenerator.testResetFormatOpts();
    }

    virtual TestStatus testListExceptionHandling() noexcept
    {
        std::cout << "\n\nRunning Exception Handling Test Cases " << itemUnderTest << "\n";

        selfTestResetAllValues();

        bool globalForceException = CoreDBInterface::forceException;

        std::vector<ListExceptionTestElement> exceptionTests = initListExceptionTests();

        TestStatus exceptionHandlingPassed = forceListExceptionsLoop(exceptionTests);

        CoreDBInterface::forceException = globalForceException;

        std::cout << "\n\n";

        return exceptionHandlingPassed;
    }

    virtual TestStatus forceListExceptionsLoop(std::vector<ListExceptionTestElement> exceptionTests) noexcept
    {
        TestStatus testAllExceptionHandling = TESTPASSED;
        std::string_view failFunction;
        
        std::cout << std::format("In force exception loop for {}", itemUnderTest) << std::endl;
        try
        {
            for (auto exceptionTest: exceptionTests)
            {
                failFunction = exceptionTest.functionUnderTest;
                if (exceptionTest.testExceptionFunction() == TESTFAILED)
                {
                    std::cerr << std::format("{}::{}: Exception Test FAILED\n",
                        itemUnderTest, failFunction);
                    testAllExceptionHandling = TESTFAILED;
                }
            }
        }

        catch (std::exception &uncaughtException)
        {
            std::cerr << std::format("{}::{}:: Caught Unhandled Exception!! Test FAILED!\n{}",
                itemUnderTest, failFunction, uncaughtException.what());
            testAllExceptionHandling = TESTFAILED;
        }

        return testAllExceptionHandling;
    }

    TestStatus testListExceptionReportFailure(bool expectSuccess, bool isBool, std::string_view testExceptionFuncName) noexcept
    {
        std::string reportFailure = std::format("In {}::{}: ", itemUnderTest, testExceptionFuncName);

        if (expectSuccess)
        {
            reportFailure.append(std::format("expected success returned {} \n", isBool? "false" : "empty vector"));
            reportFailure.append(CoreDBInterface::errorMessages);
            reportFailure.append("\n");
        }
        else
        {
            reportFailure.append(std::format("expected failure returned {} ", isBool? "true" : "vector with values"));
        }
        reportFailure.append("\nException Test FAILED");

        std::cout << reportFailure << std::endl;

        return TESTFAILED;
    }

    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    TestStatus testListExceptionAndSuccessNArgs(std::string_view funcName, F funcUnderTest, Ts... args) noexcept
    {
        std::cout << std::format("\nException Test for {}", funcName) << std::endl;
        std::string debugName(funcName);

        CoreDBInterface::forceException = true;
        ListClass::queryGenerator.setForceExceptions(true);
        if (!funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(false, false, funcName);
        }

        selfTestResetAllValues();
        ListClass::queryGenerator.setForceExceptions(false);
        if (!funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(false, false, funcName);
        }

        selfTestResetAllValues();
        CoreDBInterface::forceException = false;
        ListClass::queryGenerator.setForceExceptions(true);
        if (!funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(false, false, funcName);
        }

        selfTestResetAllValues();
        ListClass::queryGenerator.setForceExceptions(false);
        if (funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(true, false, funcName);
        }

        return TESTPASSED;
    }
};

#endif // LISTSELFTEST_H_


