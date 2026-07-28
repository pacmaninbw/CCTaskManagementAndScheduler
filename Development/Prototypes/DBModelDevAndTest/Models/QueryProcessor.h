#ifndef QUERYPROCESSOR_H_
#define QUERYPROCESSOR_H_

// Project Header Files
#include "ModelDBInterface.h"
#include "commonTestValues.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <boost/mysql/pfr.hpp>

// Standard C++ Header Files
#include <concepts>
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

/*
 * Templated Class to handle model select queries with multiple results.
 */
struct ListExceptionTestElement
{
    /*
     * this struct is used during self testing to exercise all paths through a function
     * including the catch clause of a try / catch block.
     */
    std::function<TestStatus(void)> testExceptionFunction;
    const char* functionUnderTest;
};

template<typename ListType, typename DbTransLator>
requires std::is_base_of<ModelDBInterface, ListType>::value
class QueryProcessor : public CoreDBInterface
{
protected:

public:
    QueryProcessor(std::string modelname)
    : CoreDBInterface()
    {
        /*
         * The listTypeName variable is used during testing and debugging.
         */
        modelName = modelname;
        std::string tempListType(modelname);
        tempListType.append("QueryProcessor");
        listTypeName = tempListType;
    }
    virtual ~QueryProcessor() = default;

    std::string getListTypeName() const noexcept { return listTypeName; };
    
    bool runStringOnlyQuery(std::string queryString)
    {
        clearErrorMessages();

        try
        {
            stringOnlyResults.clear();
            boost::mysql::results localResult = runQueryAsync(queryString);
            return processStringOnlyResults(localResult);
        }

        catch(const std::exception& e)
        {
            appendErrorMessage(std::format("In {}List.runStringOnlyQuery() : {}", listTypeName, e.what()));
            return false;
        }
    }

    virtual TestStatus runSelfTest() noexcept
    {
        selfTestResetAllValues();

        std::cout << "\nRunning " << listTypeName << " Self Test\n";

        return testListExceptionHandling();
    };

protected:
    /*
     * The results of a database query are passed in and converted into a list of
     * model objects.
     */
    virtual std::vector<std::shared_ptr<ListType>>
        processStaticResults(boost::mysql::static_results<boost::mysql::pfr_by_name<DbTransLator>> &queryResults)
    {
        std::vector<std::shared_ptr<ListType>> queryResultValues;
        queryResultValues.clear();
        if (m_selfTest)
        {
            // In self test if we are not forcing an exception, the function must return
            // something. The processResultRow function will fail in self test in the 
            // boost::mysql code, where it will fail an assert().
            if (m_forceException)
            {
                throw std::out_of_range("Forced exception");
            }
            if (m_forceError)
            {
                return queryResultValues;
            }
            queryResultValues.push_back(std::make_shared<ListType>());
        }
        else {
            for (const DbTransLator& dbTranslator : queryResults.rows())
            {
                queryResultValues.push_back(std::make_shared<ListType>(dbTranslator));
            }
        }

        return queryResultValues;
    }

    /*
     * The query is expecting exactly one model to be returned, such as select
     * by the primarykey.
     */
    virtual std::shared_ptr<ListType> getOneStaticResult(boost::mysql::static_results<boost::mysql::pfr_by_name<DbTransLator>>& localResult)
    {
        std::vector<std::shared_ptr<ListType>> shouldHaveOnlyOne = processStaticResults(localResult);
        std::shared_ptr<ListType> found = nullptr;

        if (shouldHaveOnlyOne.empty())
        {
            std::string emsg(modelName);
            emsg.append(" not found!");
            appendErrorMessage(emsg);
            return found;
        }

        if (shouldHaveOnlyOne.size() > 1)
        {
            std::string emsg("Too many ");
            emsg.append(modelName);
            emsg.append("s found to process!");
            appendErrorMessage(emsg);
            return found;
        }

        found = shouldHaveOnlyOne[0];

        return found;
    }

    virtual bool processStringOnlyResults(boost::mysql::results& results)
    {
        if (m_selfTest)
        {
            stringOnlyResults.push_back("Test String");
            return true;
        }

        if (results.rows().empty())
        {
            return true;
        }

        for (auto row: results.rows())
        {
            boost::mysql::field_view fieldView = row.at(0);
            if (fieldView.kind() == boost::mysql::field_kind::string)
            {
                std::string returnedValue = fieldView.as_string();
                stringOnlyResults.push_back(returnedValue);
            }
    
        }

        return true;
    }

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept = 0;

    virtual void selfTestResetAllValues() noexcept
    {
        m_selfTest = true;
        m_formatOpts.reset();
        clearErrorMessages();
    }

    virtual TestStatus testListExceptionHandling() noexcept
    {
        std::cout << "\n\nRunning Exception Handling Test Cases " << listTypeName << "\n";

        selfTestResetAllValues();

        bool globalForceException = CoreDBInterface::m_forceException;

        std::vector<ListExceptionTestElement> exceptionTests = initListExceptionTests();

        TestStatus exceptionHandlingPassed = forceListExceptionsLoop(exceptionTests);

        CoreDBInterface::m_forceException = globalForceException;

        std::cout << "\n\n";

        return exceptionHandlingPassed;
    }

    virtual TestStatus forceListExceptionsLoop(std::vector<ListExceptionTestElement> exceptionTests) noexcept
    {
        TestStatus testAllExceptionHandling = TESTPASSED;
        const char* failFunction;
        
        try
        {
            for (auto exceptionTest: exceptionTests)
            {
                failFunction = exceptionTest.functionUnderTest;
                if (exceptionTest.testExceptionFunction() == TESTFAILED)
                {
                    std::cerr << std::format("{}::{}: Exception Test FAILED\n",
                        listTypeName, failFunction);
                    testAllExceptionHandling = TESTFAILED;
                }
            }
        }

        catch (std::exception &uncaughtException)
        {
            std::cerr << std::format("{}::{}:: Caught Unhandled Exception!! Test FAILED!\n{}",
                listTypeName, failFunction, uncaughtException.what());
            testAllExceptionHandling = TESTFAILED;
        }

        return testAllExceptionHandling;
    }

    /*
     * Report any failures in an exception test.
     */
    TestStatus testListExceptionReportFailure(bool expectSuccess, bool isBool, const char* testExceptionFuncName) noexcept
    {
        std::string reportFailure = std::format("In {}::{}: ", listTypeName, testExceptionFuncName);

        if (expectSuccess)
        {
            reportFailure.append(std::format("expected success returned {} \n", isBool? "false" : "empty vector"));
            reportFailure.append(CoreDBInterface::m_errorMessages);
            reportFailure.append("\n");
        }
        else
        {
            reportFailure.append(std::format("expected failure returned {} ", isBool? "true" : "vector with values"));
        }
        reportFailure.append("\nException Test FAILED");

        std::cerr << reportFailure << std::endl;

        return TESTFAILED;
    }

    /*
     * Preform an exception test on any function that returns a vector of shared
     * pointers of a model. Used to exercise as many paths through a function as
     * possible for unit testing (self testing).
     */
    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    TestStatus testListExceptionAndSuccessNArgs(const char* funcName, F funcUnderTest, Ts... args) noexcept
    {
        std::cout << std::format("\nException Test for {}", funcName) << std::endl;
        std::string debugName(funcName);

        m_forceException = true;
        if (!funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(false, false, funcName);
        }

        selfTestResetAllValues();
        m_forceException = false;
        if (funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(true, false, funcName);
        }

        return TESTPASSED;
    }

    /*
     * Preform an exception test on any function that returns a single shared
     * pointer of a model. Used to exercise as many paths through a function as
     * possible for unit testing (self testing).
     */
    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    TestStatus testExceptionAndSuccessNArgs(const char* funcName, F funcUnderTest, Ts... args) noexcept
    {
        CoreDBInterface::m_forceException = true;
        if (funcUnderTest(args...))
        {
            return testListExceptionReportFailure(false, true, funcName);
        }

        CoreDBInterface::m_forceException = false;
        if (!funcUnderTest(args...))
        {
            return testListExceptionReportFailure(true, true, funcName);
        }

        return TESTPASSED;
    }



    static const std::size_t IndexNotSet = 0xffff;

    std::string listTypeName;
    std::string modelName;
    std::vector<std::string> stringOnlyResults;
};

#endif // QUERYPROCESSOR_H_


