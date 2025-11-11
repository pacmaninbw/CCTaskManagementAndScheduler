#ifndef MODELDSELFTEST_H_
#define MODELDSELFTEST_H_

// Project Header Files
#include "ModelDBInterface.h"
#include "TestStatus.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <concepts>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

struct ExceptionTestElement
{
    std::function<bool(void)> testExceptionFunction;
    std::string_view functionUnderTest;
};

template<class T>
concept IsDerivedFrom = std::is_base_of_v<ModelDBInterface, T>;

template<class T>
requires IsDerivedFrom<T>
class ModelSelfTest : public T
{
protected:
/*****
 * These methods must be implemented.
 * The tests they implement require the details of the particular model being tested.
 */
    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept = 0;
    virtual bool testExceptionInsert() = 0;     // Should be added too the vector returned by initExceptionTests()
    virtual bool testExceptionUpdate() = 0;     // Should be added too the vector returned by initExceptionTests()
    virtual bool testExceptionRetrieve() = 0;   // Should be added too the vector returned by initExceptionTests()
    virtual TestStatus testAllInsertFailures() = 0;
    virtual TestStatus testAttributeAccessFunctions() noexcept = 0;
    virtual TestStatus testEqualityOperator() noexcept = 0;

public:

    ModelSelfTest() : T() { }

    virtual ~ModelSelfTest() = default;
/*
 * To maximize testy coverage of any particular model, override runSelfTest and
 * provide any additional tests. This default version will provide roughly 80%
 * test coverage. Improved logging will also be achieved by override.
 */    
    virtual TestStatus runSelfTest()
    {
        CoreDBInterface::inSelfTest = true;
        CoreDBInterface::verboseOutput = true;
        bool allSelfTestsPassed = true;

        std::cout << "Running " << ModelDBInterface::modelName << " Self Test\n";

        if (testCommonInsertFailurePath() != TESTPASSED)
        {
            allSelfTestsPassed = false;
        }
        else
        {
            std::cout << "Common Insertion Failure Test PASSED!\n";
        }

        if (testCommonUpdateFailurePath() != TESTPASSED)
        {
            allSelfTestsPassed = false;
        }
        else
        {
            std::cout << "Common Update Failure Test PASSED!\n";
        }

        if (!testSave())
        {
            allSelfTestsPassed = false;
        }

        if (testAttributeAccessFunctions() == TESTFAILED)
        {
            std::cerr << ModelDBInterface::modelName << "::runSelfTest: One or more get or set functions FAILED!\n";
            allSelfTestsPassed = false;
        }

        if (testExceptionHandling()!= TESTPASSED)
        {
            std::cerr  << ModelDBInterface::modelName << "::runSelfTest: Exception handling FAILED!\n";
            allSelfTestsPassed = false;
        }
        
        if (testAllInsertFailures() != TESTPASSED)
        {
            std::cerr << "Test of all insertion failures FAILED!\n";
            allSelfTestsPassed = false;
        }

        if (!testTextFieldManipulation())
        {
            allSelfTestsPassed = false;
        }

        CoreDBInterface::inSelfTest = false;
        CoreDBInterface::verboseOutput = false;
        
        return allSelfTestsPassed? TESTPASSED : TESTFAILED;
    }

/*****
 * ModelSelfTest::selfTestResetAllValues() should be called by any override.
 * The override should provide resets for any of the model under tests attributes.
 */
    virtual void selfTestResetAllValues()
    {
        ModelDBInterface::primaryKey = 0;
        ModelDBInterface::modified = false;
/*****
 * format_opts needs to be reset before any exception test
 */
        CoreDBInterface::format_opts.reset();
        CoreDBInterface::errorMessages.clear();
    }

    virtual bool testSave()
    {
        selfTestResetAllValues();

        bool testPassed = true;
        ModelDBInterface::modified = false;
        ModelDBInterface::primaryKey = 0;

        if (ModelDBInterface::save())
        {
            std::cout << ModelDBInterface::modelName << "::save worked without being modified\n";
            testPassed = false;
        }
        else
        {
            if (!hasErrorMessage())
            {
                std::cout << ModelDBInterface::modelName << "::save Missing expected error messages\n";
                testPassed = false;
            }
            else
            {
                if (wrongErrorMessage("not modified, no changes to save") != TESTPASSED)
                {
                    std::cout << ModelDBInterface::modelName << "::save Wrong error message\n";
                    testPassed = false;
                }
            }
        }

        return testPassed;
    }

    virtual bool testTextFieldManipulation()
    {
        bool textFieldManipulationPassed = true;
        int testFieldCount = 31;
        std::vector<std::string> testInputFields;
        std::string expectedImplodeOutput;

        for (std::size_t i : std::views::iota(1, testFieldCount))
        {
            std::string fieldName = std::format("InputField{}", i);
            testInputFields.push_back(fieldName);
            expectedImplodeOutput.append(fieldName);
            expectedImplodeOutput += ModelDBInterface::delimiter;
        }

        std::string implodeOutput = ModelDBInterface::implodeTextField(testInputFields);
        if (implodeOutput != expectedImplodeOutput)
        {
            textFieldManipulationPassed = false;
            std::cerr << std::format("Unit test of implodeTextField() FAILED!\nExpected Output = {}\nActual Output ={}\n",
                expectedImplodeOutput, implodeOutput);
        }

        if (textFieldManipulationPassed)
        {
            std::vector<std::string> explodeOutput = ModelDBInterface::explodeTextField(implodeOutput);
            if (explodeOutput != testInputFields)
            {
                textFieldManipulationPassed = false;
                std::cerr << "Unit test of explodeTextField() FAILED!\n";
            }
        }

        return textFieldManipulationPassed;
    }

    bool hasErrorMessage()
    {
        std::string testErrorMessage = CoreDBInterface::getAllErrorMessages();
        if (testErrorMessage.empty())
        {
            std::cerr << "No error message generated! TEST FAILED!\n";
            return false;
        }

        return true;
    }

    TestStatus wrongErrorMessage(std::string expectedString)
    {
        std::size_t found = CoreDBInterface::errorMessages.find(expectedString);
        if (found == std::string::npos)
        {
            std::cerr << "Wrong message generated! TEST FAILED!\n";
            std::cerr << CoreDBInterface::errorMessages << "\n";
            std::cerr << "Missing expected: " << expectedString << "\n";
            std::cerr << "Error Messages:" << CoreDBInterface::errorMessages << "\n";
            return TESTFAILED;
        }

        return TESTPASSED;
    }

    virtual TestStatus testInsertionFailureMessages(std::vector<std::string> expectedErrors)
    {
        if (ModelDBInterface::insert())
        {
            std::cerr << std::format("Inserted {} missing required fields!  TEST FAILED\n", ModelDBInterface::modelName);
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

    virtual TestStatus testUpdateFailureMessages(std::vector<std::string> expectedErrors)
    {
        if (ModelDBInterface::update())
        {
            std::cerr << std::format("Update successful with expected errors!  TEST FAILED\n", ModelDBInterface::modelName);
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
                if (!exceptionTest.testExceptionFunction())
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

    TestStatus testCommonInsertFailurePath()
    {
        selfTestResetAllValues();

        ModelDBInterface::primaryKey = 1;
        ModelDBInterface::modified = true;

        std::vector<std::string> alreadyInDB = {"already in Database"};
        if (testInsertionFailureMessages(alreadyInDB) != TESTPASSED)
        {
            return TESTFAILED;
        }

        selfTestResetAllValues();

        std::vector<std::string> notModified = {"not modified!"};
        if (testInsertionFailureMessages(notModified) != TESTPASSED)
        {
            return TESTFAILED;
        }

        return TESTPASSED;
    }

    TestStatus testCommonUpdateFailurePath()
    {
        selfTestResetAllValues();

        std::vector<std::string> notInDB = {"not in Database"};
        if (testUpdateFailureMessages(notInDB) != TESTPASSED)
        {
            return TESTFAILED;
        }

        ModelDBInterface::primaryKey = 1;
        std::vector<std::string> notModified = {"not modified!"};
        if (testUpdateFailureMessages(notModified) != TESTPASSED)
        {
            return TESTFAILED;
        }

        return TESTPASSED;
    }
    
    template <typename U>
    bool testAccessorFunctions(U testValue, U* member, std::string_view memberName, std::function<void(U)>setFunct, std::function<U(void)>getFunct) noexcept
    {
        std::cout << "Running self test on set and get functions for " << ModelDBInterface::modelName << "::" << memberName << "\n";

        ModelDBInterface::modified = false;

        setFunct(testValue);
        if (!ModelDBInterface::isModified())
        {
            std::cerr << "In self test for: " << ModelDBInterface::modelName << " Set function for " << memberName << " FAILED to set modified\n";
            return false;
        }

        if (*member != testValue)
        {
            std::cerr  << "In self test for: " << ModelDBInterface::modelName << "Set function for " << memberName << " FAILED to set member value\n";
            return false;
        }

        if (getFunct() != testValue)
        {
            std::cerr  << "In self test for: " << ModelDBInterface::modelName << "Get function for " << memberName << " FAILED\n";
            return false;
        }

        std::cout << "Self test on set and get functions for " << ModelDBInterface::modelName << "::" << memberName << " PASSED\n";

        return true;
    }

    template <typename U>
    bool testOptionalAccessorFunctions(std::optional<U> testValue, std::optional<U>* member, std::string_view memberName,
        std::function<void(U)>setFunct, std::function<std::optional<U>(void)>getFunct) noexcept
    {
        std::cout << "Running self test on set and get functions for " << ModelDBInterface::modelName << "::" << memberName << "\n";

        ModelDBInterface::modified = false;

        setFunct(testValue.value());
        if (!ModelDBInterface::isModified())
        {
            std::cerr << "In self test for: " << ModelDBInterface::modelName << " set function for " << memberName << " FAILED to set modified\n";
            return false;
        }

        if (*member != testValue)
        {
            if (!member->has_value())
            {
                std::cerr  << "In self test for: " << ModelDBInterface::modelName << "Set function for " << memberName << " FAILED to set member value\n";
            }
            if (member->value() != testValue.value())
            {
                std::cerr  << "In self test for: " << ModelDBInterface::modelName << " expected value: " << testValue.value()
                         << "actual value: " << member->value() << " FAILED to set member value\n";
            }
            return false;
        }

        std::optional<U> returnValue = getFunct();
        if (returnValue != testValue)
        {
            std::cerr  << "In self test for: " << ModelDBInterface::modelName << "Get function for " << memberName << " FAILED\n";
            return false;
        }

        std::cout << "Self test on set and get functions for " << ModelDBInterface::modelName << "::" << memberName << " PASSED\n";

        return true;
    }

    bool testTimeStampAccessorFunctions(std::chrono::system_clock::time_point testValue,
        std::optional<std::chrono::system_clock::time_point>* member,
        std::string_view memberName,
        std::function<void(std::chrono::system_clock::time_point)>setFunct,
        std::function<std::chrono::system_clock::time_point(void)>getFunct) noexcept
    {
        std::cout << "Running self test on set and get functions for " << ModelDBInterface::modelName << "::" << memberName << "\n";

        ModelDBInterface::modified = false;

        setFunct(testValue);
        if (!ModelDBInterface::isModified())
        {
            std::cerr << "In self test for: " << ModelDBInterface::modelName << " set function for " << memberName << " FAILED to set modified\n";
            return false;
        }

        if (!member->has_value() || member->value() != testValue)
        {
            std::cerr  << "In self test for: " << ModelDBInterface::modelName << "Set function for " << memberName << " FAILED to set member value\n";
            return false;
        }

        if (getFunct() != testValue)
        {
            std::cerr  << "In self test for: " << ModelDBInterface::modelName << "Get function for " << memberName << " FAILED\n";
            return false;
        }

        std::cout << "Self test on set and get functions for " << ModelDBInterface::modelName << "::" << memberName << " PASSED\n";

        return true;
    }

    bool testExceptionReportFailure(bool expectSuccess, bool isBool, std::string_view testExceptionFuncName) noexcept
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

        return false;
    }

    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    bool testExceptionAndSuccessNArgs(std::string_view funcName, F funcUnderTest, Ts... args) noexcept
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

        return true;
    }

/*
 * Format functions return strings rather than bool.
 */
    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    bool testFormatExceptionAndSuccessNArgs(std::string_view funcName, F funcUnderTest, Ts... args) noexcept
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

        return true;
    }

/*
 * Used to test formatSQL functions that do not contain a try catch block.
 */
    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    bool testFormatExceptionCatchSuccessNArgs(std::string_view funcName, F funcUnderTest, Ts... args) noexcept
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

        return true;
    }

};

#endif // MODELDSELFTEST_H_


