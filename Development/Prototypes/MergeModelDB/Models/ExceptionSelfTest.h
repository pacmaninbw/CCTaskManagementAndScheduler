#ifndef EXCEPTIONSELFTEST_H_
#define EXCEPTIONSELFTEST_H_

// Project Header Files
#include "ModelDBInterface.h"
#include "TestStatus.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <concepts>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

/*
 * Each of the model classes will be used in application that provides a graphic
 * user interface. The application needs to be robust and handle exceptions as
 * they occur in a manor where the application will provide necessary information
 * to the user if an error occurs. Error messages returned by the libraries in
 * use may not be meaningful to a user. This class tests the functions in the
 * model that may receive an exception from the underlying libraries.
 * 
 * Exceptions are forced at what be the library level to test the exception
 * handling capability of the model.
 * 
 * An exception test should first test what the function will do if an exception
 * is thrown and then test to see that the function properly works otherwise.
 * 
 * A list of exception tests needs tyo be provided by the class that is testing
 * the model.
 */

struct ExceptionTestElement
{
    std::function<TestStatus(void)> testExceptionFunction;
    std::string_view functionUnderTest;
};

template<class Model>
requires std::is_base_of_v<ModelDBInterface, Model>
class ExceptionSelfTest : public virtual Model
{
protected:
/*****
 * These methods must be implemented.
 * The tests they implement require the details of the particular model being tested.
 * 
 * Init functions should call the local version of selfTestResetAllValues().
 */
    virtual void selfTestResetAllValues() noexcept = 0;
    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept = 0;
    virtual TestStatus testExceptionInsert() = 0;     // Should be added too the vector returned by initExceptionTests()
    virtual TestStatus testExceptionUpdate() = 0;     // Should be added too the vector returned by initExceptionTests()
    virtual TestStatus testExceptionRetrieve() = 0;   // Should be added too the vector returned by initExceptionTests()

public:

    ExceptionSelfTest() : Model() { }

    virtual ~ExceptionSelfTest() = default;

protected:

    virtual TestStatus testExceptionHandling() noexcept
    {
        std::cout << "\n\nRunning Exception Handling Test Cases\n";

        selfTestResetAllValues();

        bool globalForceException = CoreDBInterface::forceException;

        std::vector<ExceptionTestElement> exceptionTests = initExceptionTests();

        TestStatus exceptionHandlingPassed = forceExceptionsLoop(exceptionTests);

        CoreDBInterface::forceException = globalForceException;

        std::cout << "\n\n";

        return exceptionHandlingPassed;
    }

    virtual TestStatus forceExceptionsLoop(std::vector<ExceptionTestElement> exceptionTests) noexcept
    {
        TestStatus testAllExceptionHandling = TESTPASSED;
        
        try
        {
            for (auto exceptionTest: exceptionTests)
            {
                if (exceptionTest.testExceptionFunction() == TESTFAILED)
                {
                    std::cerr << std::format("{}::{} returned true with exception: Exception Test Failed\n",
                        ModelDBInterface::modelName, exceptionTest.functionUnderTest);
                    testAllExceptionHandling = TESTFAILED;
                }
            }
        }

        catch (std::exception &uncaughtException)
        {
            std::cerr << ModelDBInterface::modelName << "::testExceptionHandling():: Caught Unhandled Exception!! Test FAILED!\n";
            std::cerr << uncaughtException.what() << "\n";
            testAllExceptionHandling = TESTFAILED;
        }

        return testAllExceptionHandling;
    }

    TestStatus testExceptionReportFailure(bool expectSuccess, bool isBool, std::string_view testExceptionFuncName) noexcept
    {
        std::string reportFailure = std::format("In {}::{}: ", ModelDBInterface::modelName, testExceptionFuncName);

        if (expectSuccess)
        {
            reportFailure.append(std::format("expected success returned {} \n", isBool? "false" : "empty string"));
            reportFailure.append(CoreDBInterface::errorMessages);
            reportFailure.append("\n");
        }
        else
        {
            reportFailure.append(std::format("expected failure returned {} ", isBool? "true" : "formatted string"));
        }

        std::cout << reportFailure << "\n";

        return TESTFAILED;
    }

    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    TestStatus testExceptionAndSuccessNArgs(std::string_view funcName, F funcUnderTest, Ts... args) noexcept
    {
        CoreDBInterface::forceException = true;
        if (funcUnderTest(args...))
        {
            return testExceptionReportFailure(false, true, funcName);
        }

        CoreDBInterface::forceException = false;
        if (!funcUnderTest(args...))
        {
            return testExceptionReportFailure(true, true, funcName);
        }

        return TESTPASSED;
    }

/*
 * Format functions return strings rather than bool.
 */
    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    TestStatus testFormatExceptionAndSuccessNArgs(std::string_view funcName, F funcUnderTest, Ts... args) noexcept
    {
        CoreDBInterface::forceException = true;
        std::string formattedQuery = funcUnderTest(args...);
        if (!formattedQuery.empty())
        {
            return testExceptionReportFailure(false, false, funcName);
        }

        CoreDBInterface::forceException = false;
        formattedQuery.clear();
        formattedQuery = funcUnderTest(args...);
        if (formattedQuery.empty())
        {
            return testExceptionReportFailure(true, false, funcName);
        }

        return TESTPASSED;
    }

/*
 * Used to test formatSQL functions that do not contain a try catch block.
 */
    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    TestStatus testFormatExceptionCatchSuccessNArgs(std::string_view funcName, F funcUnderTest, Ts... args) noexcept
    {
        std::string formattedQuery;

        try
        {
            CoreDBInterface::forceException = true;
            formattedQuery = funcUnderTest(args...);
            return testExceptionReportFailure(false, false, funcName);    
        }
        catch(const std::exception& e)
        {
            std::cout << "Caught expected exception from " << funcName << "(): " << e.what() << "\n";
            CoreDBInterface::forceException = false; 
            formattedQuery = funcUnderTest(args...);
            if (formattedQuery.empty())
            {
                return testExceptionReportFailure(true, false, funcName);
            }
        }        

        return TESTPASSED;
    }

};

#endif // EXCEPTIONSELFTEST_H_


