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
#include <iostream>
#include <memory>
#include <optional>
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

struct ColumnNameToIndexmapping
{
    std::string columnName;
    std::optional<std::size_t> columnIndex;
    ColumnNameToIndexmapping(std::string name) : columnName{name}{};
};


template<typename ListType>
requires std::is_base_of<ModelDBInterface, ListType>::value
class QueryProcessor : public CoreDBInterface
{
public:
    QueryProcessor()
    : CoreDBInterface()
    {
        queryGenerator = std::make_unique<ListType>();

        std::string tempListType = queryGenerator->getModelName();
        tempListType.append("QueryProcessor");
        listTypeName = tempListType;
    }
    virtual ~QueryProcessor() = default;

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

    bool runStringOnlyQuery()
    {
        errorMessages.clear();

        try
        {
            stringOnlyResults.clear();
            boost::mysql::results localResult = runQueryAsync(firstFormattedQuery);
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


    virtual void fillRequiredIndexes() = 0;

    /*
    * Map the column names to the indexes for the columns, the order of the columns
    * in the result set is not guarenteed. Ensure that the columns necessary to fill
    * the model are present. Currently there is no error if there are too many fields.
    */
    void mapColumnNameToIndex(boost::mysql::resultset_view &noteQueryresultSet)
    {
        std::vector<std::string> columnNames;
        bool hasAllRequiredColumns = true;

        for (auto metaIter: noteQueryresultSet.meta())
        {
            columnNames.push_back(metaIter.column_name());
        }

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
            }
        }

        if (!hasAllRequiredColumns)
        {
            throw std::out_of_range("Results missing required fields");
        }

        fillRequiredIndexes();
    }

    virtual std::shared_ptr<ListType> processResultRow(boost::mysql::row_view &queryRow) = 0;

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
        if (inSelfTest)
        {
            // In self test if we are not forcing an exception, the function must return
            // something. The processResultRow function will fail in self test in the 
            // boost::mysql code, where it will fail an assert().
            if (forceException)
            {
                throw std::out_of_range("Forced exception");
            }
            if (forceError)
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

    virtual bool processStringOnlyResults(boost::mysql::results& results)
    {
        if (inSelfTest)
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
        inSelfTest = true;
        format_opts.reset();
        errorMessages.clear();

        primaryKeyResults.clear();
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
        std::cout << std::format("Force Exception {} Force Error {}", (forceException? "TRUE" : "FALSE"), (forceException? "TRUE" : "FALSE")) << std::endl;
        if (!funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(false, false, funcName);
        }

        selfTestResetAllValues();
        forceException = false;
        std::cout << std::format("Force Exception {} Force Error {}", (forceException? "TRUE" : "FALSE"), (forceException? "TRUE" : "FALSE")) << std::endl;
        if (funcUnderTest(args...).empty())
        {
            return testListExceptionReportFailure(true, false, funcName);
        }

        return TESTPASSED;
    }

    static const std::size_t IndexNotSet = 0xffff;

    std::unique_ptr<ListType> queryGenerator;
    std::string listTypeName;
    std::string firstFormattedQuery;
    std::vector<std::size_t> primaryKeyResults;
    std::vector<std::string> stringOnlyResults;
    std::vector<ColumnNameToIndexmapping> columnToIndexMap;
    std::vector<std::string> requiredColumns;
};

#endif // QUERYPROCESSOR_H_


