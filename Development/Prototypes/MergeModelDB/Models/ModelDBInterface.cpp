// Project Header Files
#include "ModelDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

ModelDBInterface::ModelDBInterface(std::string_view modelNameIn)
: CoreDBInterface()
{
    primaryKey = 0;
    modelName = modelNameIn;
    modified = false;
    delimiter = ';';  
}

bool ModelDBInterface::save()
{
    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified, no changes to save", modelName));
        return false;
    }

    if (isInDataBase())
    {
        return update();
    }
    else
    {
        return insert();
    }
}

bool ModelDBInterface::insert()
{
    errorMessages.clear();

    if (isInDataBase())
    {
        appendErrorMessage(std::format("{} already in Database, use Update!", modelName));
        return false;
    }

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", modelName));
        return false;
    }

    if (!hasRequiredValues())
    {
        appendErrorMessage(std::format("{} is missing required values!", modelName));
        reportMissingFields();
        return false;
    }

    prepareForRunQueryAsync();

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatInsertStatement());
        primaryKey = localResult.last_insert_id();
        modified = false;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.insert : {}", modelName, e.what()));
        return false;
    }

    return true;
}

bool ModelDBInterface::update()
{
    prepareForRunQueryAsync();

    if (!isInDataBase())
    {
        appendErrorMessage(std::format("{} not in Database, use Insert!", modelName));
        return false;
    }

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", modelName));
        return false;
    }

    try
    {

        boost::mysql::results localResult = runQueryAsync(formatUpdateStatement());
        modified = false;
            
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.update : {}", modelName, e.what()));
        return false;
    }
}

bool ModelDBInterface::retrieve()
{
    prepareForRunQueryAsync();

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatSelectStatement());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.retrieve() : {}", modelName, e.what()));
        return false;
    }
}

bool ModelDBInterface::hasRequiredValues()
{
    initRequiredFields();
    for (auto fieldTest: missingRequiredFieldsTests)
    {
        if (fieldTest.errorCondition())
        {
            return false;
        }
    }
    
    return true;
}

/*
 * Assumes that ModelDBInterface::hasRequiredValues() was called previously and that
 * initRequiredFields() has been called.
 */
void ModelDBInterface::reportMissingFields() noexcept
{
    for (auto testAndReport: missingRequiredFieldsTests)
    {
        if (testAndReport.errorCondition())
        {
            appendErrorMessage(std::format("Missing {} required {}!\n", modelName, testAndReport.fieldName));
        }
    }
}

bool ModelDBInterface::processResult(boost::mysql::results& results)
{
    if (results.rows().empty())
    {
        appendErrorMessage(std::format("{} not found!", modelName));
        return false;
    }

    if (results.rows().size() > 1)
    {
        appendErrorMessage(std::format("Too many {}s found to process!", modelName));
        return false;
    }

    boost::mysql::row_view rv = results.rows().at(0);
    processResultRow(rv);

    return true;
}

std::vector<std::string> ModelDBInterface::explodeTextField(std::string const& textField) noexcept
{
    std::vector<std::string> subFields;
    std::istringstream iss(textField);

    for (std::string token; std::getline(iss, token, delimiter); )
    {
        subFields.push_back(std::move(token));
    }
    return subFields;
}

std::string ModelDBInterface::implodeTextField(std::vector<std::string> &fields) noexcept
{
    std::string textField;

    for (auto field: fields)
    {
        textField.append(field);
        textField += delimiter;
    }

    return textField;
}

bool ModelDBInterface::runSelfTest()
{
    bool allSelfTestsPassed = true;

    if (verboseOutput)
    {
        std::clog << "Running " << modelName << " Self Test\n";
    }

    if (!testAccessorFunctionsPassed())
    {
        std::clog << modelName << "::runSelfTest: One or more get or set functions FAILED!\n";
        allSelfTestsPassed = false;
    }

    if (!testExceptionHandling())
    {
        std::clog  << modelName << "::runSelfTest: Exception handling FAILED!\n";
        allSelfTestsPassed = false;
    }
    
    if (testAllInsertFailures() != TESTPASSED)
    {
        std::clog << "Test of all insertion failures FAILED!\n";
    }

    return allSelfTestsPassed;
}

bool ModelDBInterface::testAccessorFunctionsPassed()
{
    std::clog << modelName << "::testAccessorFunctionsPassed() NOT IMPLEMENTED Forced FAIL!\n";
    return false;
}

bool ModelDBInterface::testExceptionHandling()
{
    std::clog << modelName << "::testExceptionHandling() NOT IMPLEMENTED Forced FAIL!\n";
    return false;
}

bool ModelDBInterface::hasErrorMessage()
{
    if (errorMessages.empty())
    {
        std::clog << "No error message generated! TEST FAILED!\n";
        return false;
    }

    if (verboseOutput)
    {
        std::clog << "Expected error was; " << errorMessages << "\n";
    }

    return true;
}

ModelDBInterface::ModelTestStatus ModelDBInterface::wrongErrorMessage(std::string expectedString)
{
    std::size_t found = errorMessages.find(expectedString);
    if (found == std::string::npos)
    {
        std::clog << "Wrong message generated! TEST FAILED!\n";
        std::clog << errorMessages << "\n";
        std::clog << "Missing expected: " << expectedString << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

ModelDBInterface::ModelTestStatus ModelDBInterface::testInsertionFailureMessages(std::vector<std::string> expectedErrors)
{
    if (insert())
    {
        std::clog << std::format("Inserted {} missing required fields!  TEST FAILED\n", modelName);
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

ModelDBInterface::ModelTestStatus ModelDBInterface::testAllInsertFailures()
{
    std::clog << std::format("{}::testAllInsertFailures() NOT IMPLEMENTED! Test FAILED\n", modelName);
    return TESTFAILED;
}
