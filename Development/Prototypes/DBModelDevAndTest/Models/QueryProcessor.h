#ifndef QUERYPROCESSOR_H_
#define QUERYPROCESSOR_H_

// Project Header Files
#include "CoreDBInterface.h"
#include "ModelDBInterface.h"
#include "commonTestValues.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <algorithm>
#include <concepts>
#include <exception>
#include <format>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <optional>
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

struct ColumnNameToIndexmapping
{
    /*
     * this struct is used while processing query results. boost::mysql returns
     * results that include the column names,
     */
    std::string columnName;
    std::optional<std::size_t> columnIndex;
    ColumnNameToIndexmapping(std::string name) : columnName{name}{};
};


template<typename ListType>
requires std::is_base_of<ModelDBInterface, ListType>::value
class QueryProcessor : public CoreDBInterface
{
protected:
/*
 * The details of each of the subclasses are required for the following functions,
 * each class must define these functions.
 */
    virtual void fillRequiredIndexes() = 0;
    virtual std::shared_ptr<ListType> processResultRow(boost::mysql::row_view &queryRow) = 0;

public:
    QueryProcessor(std::string modelname, std::initializer_list<std::string> requiredColumns)
    : CoreDBInterface()
    {
        /*
         * The listTypeName variable is used during testing and debugging.
         */
        modelName = modelname;
        std::string tempListType(modelname);
        tempListType.append("QueryProcessor");
        listTypeName = tempListType;

        for (auto columnName: requiredColumns)
        {
            columnToIndexMap.push_back(columnName);
        }

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
     * assignValueToIndex is called multiple times in the implementations of fillRequiredIndexes().
     */
    void assignValueToIndex(std::string columnName, std::size_t &columnIndex)
    {
        auto iterToIndex = std::find_if(columnToIndexMap.begin(), columnToIndexMap.end(),
            [columnName](const ColumnNameToIndexmapping& ctim){ return ctim.columnName == columnName; });
        if (iterToIndex != columnToIndexMap.end())
        {
            if (iterToIndex->columnIndex.has_value())
            {
                columnIndex = iterToIndex->columnIndex.value();
            }
            else
            {
                appendErrorMessage(std::format("NULL Value to index for {}", columnName));
            }
        }
        else
        {
            appendErrorMessage(std::format("Column Name: {} NOT FOUND in assignValueToIndex()", columnName));
        }
    }


    /*
    * Map the column names to the indexes for the columns, the order of the columns
    * in the result set is not guarenteed. Ensure that the columns necessary to fill
    * the model are present. Currently there is no error if there are too many fields.
    */
    void mapColumnNameToIndex(boost::mysql::resultset_view &noteQueryresultSet)
    {
        std::vector<std::string> columnNames;
        bool hasAllRequiredColumns = true;

        /*
         * Get all the column names in the result set.
         */
        for (auto metaIter: noteQueryresultSet.meta())
        {
            columnNames.push_back(metaIter.column_name());
        }

        /*
         * For each expected column, search the column names and set the index
         * based on the column name location.
         */
        for (std::size_t i = 0; i < columnToIndexMap.size(); ++i)
        {
            std::string nameToFind = columnToIndexMap[i].columnName;
            auto iterToIndex = std::find(columnNames.begin(), columnNames.end(), nameToFind);
            if (iterToIndex != columnNames.end())
            {
                columnToIndexMap[i].columnIndex = std::distance(columnNames.begin(), iterToIndex);
            }
            else
            {
                appendErrorMessage(std::format("Required field {} not found in results", nameToFind));
                hasAllRequiredColumns = false;
                std::cerr << std::format("Required field {} not found in:\n", nameToFind);
                for (auto columnName: columnNames)
                {
                    std::cerr << std::format("{}, ", columnName);
                }
                std::cerr << std::endl;
            }
        }

        if (!hasAllRequiredColumns)
        {
            throw std::out_of_range("Results missing required fields");
        }

        fillRequiredIndexes();
    }

    /*
    * Each boost::mysql result set contain a list of the column names in the result set.
    * Given the current implementation these columns should be in the same order for 
    * every result set, but we can't depend on that so we need to map the column names
    * to the indexes for every result set. For performance reasons we want to map the
    * indexes once per result set rather than doing a string search for each row of 
    * data. Create an object of the proper model for each row of data in the result set.
    */
    virtual std::vector<std::shared_ptr<ListType>> processResultSet(boost::mysql::resultset_view &queryresultSet)
    {
        std::vector<std::shared_ptr<ListType>> intermediateResults;

        if (queryresultSet.rows().num_columns() > 0)
        {
            mapColumnNameToIndex(queryresultSet);
            for (auto rv: queryresultSet.rows())
            {
                intermediateResults.push_back(processResultRow(rv));
            }
        }

        return intermediateResults;
    }

    /*
    * The boost::mysql::results class should contain all the results from the current
    * query, the results may span multiple result sets. The processResults method
    * should return all the results found in a single list.
    */
    virtual std::vector<std::shared_ptr<ListType>> processResults(boost::mysql::results &noteQueryResults)
    {
        std::vector<std::shared_ptr<ListType>> queryResultValues;
        queryResultValues.clear();
        if (m_SelfTest)
        {
            // In self test if we are not forcing an exception, the function must return
            // something. The processResultRow function will fail in self test in the 
            // boost::mysql code, where it will fail an assert().
            if (m_ForceException)
            {
                throw std::out_of_range("Forced exception");
            }
            if (m_ForceError)
            {
                return queryResultValues;
            }
            queryResultValues.push_back(std::make_shared<ListType>());
        }
        else {
            for (std::size_t i = 0; i < noteQueryResults.size(); ++i)
            {
                boost::mysql::resultset_view resultview = noteQueryResults[i];
                std::vector<std::shared_ptr<ListType>> intermediateResults = processResultSet(resultview);
                queryResultValues.insert(queryResultValues.end(), intermediateResults.begin(), intermediateResults.end());
            }
        }

        return queryResultValues;
    }

    virtual std::shared_ptr<ListType> getOneResult(boost::mysql::results& localResult)
    {
        std::vector<std::shared_ptr<ListType>> shouldHaveOnlyOne = processResults(localResult);
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
        if (m_SelfTest)
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
        m_SelfTest = true;
        format_opts.reset();
        clearErrorMessages();
    }

    virtual TestStatus testListExceptionHandling() noexcept
    {
        std::cout << "\n\nRunning Exception Handling Test Cases " << listTypeName << "\n";

        selfTestResetAllValues();

        bool globalForceException = CoreDBInterface::m_ForceException;

        std::vector<ListExceptionTestElement> exceptionTests = initListExceptionTests();

        TestStatus exceptionHandlingPassed = forceListExceptionsLoop(exceptionTests);

        CoreDBInterface::m_ForceException = globalForceException;

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
            reportFailure.append(CoreDBInterface::m_ErrorMessages);
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

        m_ForceException = true;
        if (!funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(false, false, funcName);
        }

        selfTestResetAllValues();
        m_ForceException = false;
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
        CoreDBInterface::m_ForceException = true;
        if (funcUnderTest(args...))
        {
            return testListExceptionReportFailure(false, true, funcName);
        }

        CoreDBInterface::m_ForceException = false;
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
    std::vector<ColumnNameToIndexmapping> columnToIndexMap;
};

#endif // QUERYPROCESSOR_H_


