#ifndef LISTDBINTERFACECORE_H_
#define LISTDBINTERFACECORE_H_

// Project Header Files
#include "CoreDBInterface.h"
#include "ModelDBInterface.h"
#include "commonTestValues.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <concepts>
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

/*
 * Templated Class to handle model select queries with multiple results.
 * 
 * The model itself is used to create the select statement. The first query will
 * only return the primary key of each instance of the model that matches the
 * search parameters of the query. Once the list of primary keys is established
 * the model will be used to select the object by primary key for the full data.
 * 
 * This is done primarily to keep the table related information for a model in
 * the model. A second reason for this design is that the is a maximum size that
 * a boost::mysql::results class can reach; to reduce the possiblity of reaching
 * that maximum queries that can return a large amount of data will be reduced
 * to returning only the primary keys of the data.
 * 
 * This design may be revisited if there is too much a performance degradation.
 */
struct ListExceptionTestElement
{
    std::function<TestStatus(void)> testExceptionFunction;
    const char* functionUnderTest;
};

template<typename ListType>
requires std::is_base_of<ModelDBInterface, ListType>::value
class ListDBInterface : public CoreDBInterface
{
public:
    ListDBInterface()
    : CoreDBInterface()
    {
        queryGenerator = std::make_unique<ListType>();

        std::string tempListType = queryGenerator->getModelName();
        tempListType.append("List");
        listTypeName = tempListType;
    }
    virtual ~ListDBInterface() = default;

    std::string getListTypeName() const noexcept { return listTypeName; };
    
    bool runFirstQuery()
    {
        errorMessages.clear();

        try
        {
            primaryKeyResults.clear();
            boost::mysql::results localResult = runQueryAsync(firstFormattedQuery);
            primaryKeyResults = processFirstQueryResults(localResult);

            return primaryKeyResults.size() > 0;
        }

        catch(const std::exception& e)
        {
            appendErrorMessage(std::format("In {}List.runFirstQuery() : {}",
                listTypeName, e.what()));
            return false;
        }
    };

    virtual TestStatus runSelfTest() noexcept
    {
        selfTestResetAllValues();

        std::cout << "\nRunning " << listTypeName << " Self Test\n";

        return testListExceptionHandling();
    };

protected:
    void setFirstQuery(std::string formattedQueryStatement) noexcept
    {
        firstFormattedQuery = formattedQueryStatement;
    }
    
    virtual std::vector<std::size_t> processFirstQueryResults(boost::mysql::results& results)
    {
        if (inSelfTest)
        {
            primaryKeyResults.push_back(1);
            return primaryKeyResults;
        }

        if (results.rows().empty())
        {
            appendErrorMessage(std::format("No {}s found!", queryGenerator->getModelName()));
            return primaryKeyResults;
        }

        for (auto row: results.rows())
        {
            primaryKeyResults.push_back(row.at(0).as_uint64());
    
        }
        return primaryKeyResults;
    }

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept = 0;

    virtual void selfTestResetAllValues() noexcept
    {
        inSelfTest = true;
        format_opts.reset();
        errorMessages.clear();

        primaryKeyResults.clear();
        returnType.clear();
        queryGenerator->setSelfTest(true);
        queryGenerator->clearModified();
        queryGenerator->clearErrorMessages();
        queryGenerator->testResetFormatOpts();
    }

    virtual TestStatus testListExceptionHandling() noexcept
    {
        std::cout << "\n\nRunning Exception Handling Test Cases " << listTypeName << "\n";

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

    TestStatus testListExceptionReportFailure(bool expectSuccess, bool isBool, const char* testExceptionFuncName) noexcept
    {
        std::string reportFailure = std::format("In {}::{}: ", listTypeName, testExceptionFuncName);

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

        std::cerr << reportFailure << std::endl;

        return TESTFAILED;
    }

    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    TestStatus testListExceptionAndSuccessNArgs(const char* funcName, F funcUnderTest, Ts... args) noexcept
    {
        std::cout << std::format("\nException Test for {}", funcName) << std::endl;
        std::string debugName(funcName);

        forceException = true;
        queryGenerator->setForceExceptions(true);
        if (!funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(false, false, funcName);
        }

        selfTestResetAllValues();
        queryGenerator->setForceExceptions(false);
        if (!funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(false, false, funcName);
        }

        selfTestResetAllValues();
        forceException = false;
        queryGenerator->setForceExceptions(true);
        if (!funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(false, false, funcName);
        }

        selfTestResetAllValues();
        queryGenerator->setForceExceptions(false);
        if (funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(true, false, funcName);
        }

        return TESTPASSED;
    }

    std::unique_ptr<ListType> queryGenerator;
    std::string listTypeName;
    std::string firstFormattedQuery;
    std::vector<std::size_t> primaryKeyResults;
    std::vector<std::shared_ptr<ListType>> returnType;
};

#endif // LISTDBINTERFACECORE_H_


