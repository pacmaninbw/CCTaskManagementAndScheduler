// Project Header Files
#include "ModelDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <iostream>
#include <ranges>
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

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatInsertStatement());
        primaryKey =  (!inSelfTest)? localResult.last_insert_id() : 1;
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
    errorMessages.clear();

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
    errorMessages.clear();

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
            appendErrorMessage(std::format("Missing {} required {}!", modelName, testAndReport.fieldName));
        }
    }
}

bool ModelDBInterface::processResult(boost::mysql::results& results)
{
    if (inSelfTest)
    {
        return true;
    }

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
    inSelfTest = true;
    verboseOutput = true;
    bool allSelfTestsPassed = true;

    if (verboseOutput)
    {
        std::clog << "Running " << modelName << " Self Test\n";
    }

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        allSelfTestsPassed = false;
    }
    else
    {
        std::clog << "Common Insertion Failure Test PASSED!\n";
    }

    if (testCommonUpdateFailurePath() != TESTPASSED)
    {
        allSelfTestsPassed = false;
    }
    else
    {
        std::clog << "Common Update Failure Test PASSED!\n";
    }

    if (!testSave())
    {
        allSelfTestsPassed = false;
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

    if (!testTextFieldManipulation())
    {
        allSelfTestsPassed = false;
    }

    inSelfTest = false;
    verboseOutput = false;
    
    return allSelfTestsPassed;
}

boost::mysql::date ModelDBInterface::stdchronoDateToBoostMySQLDate(const std::chrono::year_month_day &source) noexcept
{
    std::chrono::sys_days tp = source;
    return boost::mysql::date(tp);
}

std::chrono::year_month_day ModelDBInterface::boostMysqlDateToChronoDate(const boost::mysql::date &source) noexcept
{
    const std::chrono::year year{source.year()};
    const std::chrono::month month{source.month()};
    const std::chrono::day day{source.day()};
    return std::chrono::year_month_day{year, month, day};
}

boost::mysql::datetime ModelDBInterface::stdChronoTimePointToBoostDateTime(std::chrono::system_clock::time_point source) noexcept
{
    return boost::mysql::datetime(std::chrono::time_point_cast<boost::mysql::datetime::time_point::duration>(source));
}

std::chrono::system_clock::time_point ModelDBInterface::boostMysqlDateTimeToChronoTimePoint(boost::mysql::datetime dbDateTime)
{
    return std::chrono::time_point_cast<std::chrono::system_clock::time_point::duration>(dbDateTime.as_time_point());
}

std::optional<boost::mysql::date> ModelDBInterface::optionalDateConversion(std::optional<std::chrono::year_month_day> optDate)
{
    std::optional<boost::mysql::date> mySqlDate;

    if (optDate.has_value())
    {
        mySqlDate = stdchronoDateToBoostMySQLDate(optDate.value());
    }

    return mySqlDate;
}
std::optional<boost::mysql::datetime> ModelDBInterface::optionalDateTimeConversion(std::optional<std::chrono::system_clock::time_point> optDateTime)
{
    std::optional<boost::mysql::datetime> timeStamp;

    if (optDateTime.has_value())
    {
        timeStamp = stdChronoTimePointToBoostDateTime(optDateTime.value());
    }

    return timeStamp;
};

void ModelDBInterface::selfTestResetAllValues()
{
    primaryKey = 0;
    modified = false;
    errorMessages.clear();

    std::clog << modelName << "::selfTestResetAllValues() NOT IMPLEMENTED!\n";
}

bool ModelDBInterface::testAccessorFunctionsPassed()
{
    selfTestResetAllValues();
    std::clog << modelName << "::testAccessorFunctionsPassed() NOT IMPLEMENTED Forced FAIL!\n";
    return false;
}

bool ModelDBInterface::testExceptionHandling()
{
    selfTestResetAllValues();
    std::clog << modelName << "::testExceptionHandling() NOT IMPLEMENTED Forced FAIL!\n";
    return false;
}

bool ModelDBInterface::testSave()
{
    selfTestResetAllValues();

    bool testPassed = true;
    modified = false;
    primaryKey = 0;

    if (save())
    {
        std::clog << modelName << "::save worked without being modified\n";
        testPassed = false;
    }
    else
    {
        if (!hasErrorMessage())
        {
            testPassed = false;
        }
        else
        {
            if (wrongErrorMessage("not modified, no changes to save") != TESTPASSED)
            {
                testPassed = false;
            }
        }
    }

    return testPassed;
}

bool ModelDBInterface::testExceptionInsert()
{
    selfTestResetAllValues();
    std::clog << std::format("{}::testExceptionInsert() NOT IMPLEMENTED! Test FAILED\n", modelName);
    return false;
}

bool ModelDBInterface::testExceptionUpdate()
{
    selfTestResetAllValues();
    std::clog << std::format("{}::testExceptionUpdate() NOT IMPLEMENTED! Test FAILED\n", modelName);
    return false;
}

bool ModelDBInterface::hasErrorMessage()
{
    if (errorMessages.empty())
    {
        std::clog << "No error message generated! TEST FAILED!\n";
        return false;
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
        std::clog << "Error Messages:" << errorMessages << "\n";
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

ModelDBInterface::ModelTestStatus ModelDBInterface::testUpdateFailureMessages(std::vector<std::string> expectedErrors)
{
    if (update())
    {
        std::clog << std::format("Update successful with expected errors!  TEST FAILED\n", modelName);
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
    selfTestResetAllValues();
    std::clog << std::format("{}::testAllInsertFailures() NOT IMPLEMENTED! Test FAILED\n", modelName);
    return TESTFAILED;
}

bool ModelDBInterface::forceExceptionsLoop(std::vector<ExceptionTestElement> exceptionTests)
{
    bool exceptionHandlingPassed = true;
    
    try
    {
        for (auto exceptionTest: exceptionTests)
        {
            if (!exceptionTest.testExceptionFunction())
            {
                std::clog << std::format("{}::{} returned true with exception: Exception Test Failed\n",
                    modelName, exceptionTest.functionUnderTest);
                exceptionHandlingPassed = false;
            }
        }
    }

    catch (std::exception &uncaughtException)
    {
        std::clog << modelName << "::testExceptionHandling():: Caught Unhandled Exception!! Test FAILED!\n";
        std::clog << uncaughtException.what() << "\n";
        exceptionHandlingPassed = false;
    }

    return exceptionHandlingPassed;
}

bool ModelDBInterface::testExceptionRetrieve()
{
    selfTestResetAllValues();
    primaryKey = 1;
    return retrieve() != true;
}

bool ModelDBInterface::testTextFieldManipulation()
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
        expectedImplodeOutput += delimiter;
    }

    std::string implodeOutput = implodeTextField(testInputFields);
    if (implodeOutput != expectedImplodeOutput)
    {
        textFieldManipulationPassed = false;
        std::clog << std::format("Unit test of implodeTextField() FAILED!\nExpected Output = {}\nActual Output ={}\n",
            expectedImplodeOutput, implodeOutput);
    }

    if (textFieldManipulationPassed)
    {
        std::vector<std::string> explodeOutput = explodeTextField(implodeOutput);
        if (explodeOutput != testInputFields)
        {
            textFieldManipulationPassed = false;
            std::clog << "Unit test of explodeTextField() FAILED!\n";
        }
    }

    return textFieldManipulationPassed;
}

bool ModelDBInterface::testExceptionReportFailure(bool expectSuccess, bool isBool, std::string_view testExceptionFuncName)
{
    std::string reportFailure = std::format("In {}::{}: ", modelName, testExceptionFuncName);

    if (verboseOutput)
    {
        if (expectSuccess)
        {
            reportFailure.append(std::format("expected success {} returned ", isBool? "false" : "empty string"));
        }
        else
        {
            reportFailure.append(std::format("expected failure {} returned ", isBool? "true" : "formatted string"));
        }
    }

    std::clog << reportFailure << "\n";

    return false;
}

ModelDBInterface::ModelTestStatus ModelDBInterface::testCommonInsertFailurePath()
{
    selfTestResetAllValues();

    primaryKey = 1;
    modified = true;

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

ModelDBInterface::ModelTestStatus ModelDBInterface::testCommonUpdateFailurePath()
{
    selfTestResetAllValues();

    std::vector<std::string> notInDB = {"not in Database"};
    if (testUpdateFailureMessages(notInDB) != TESTPASSED)
    {
        return TESTFAILED;
    }

    primaryKey = 1;
    std::vector<std::string> notModified = {"not modified!"};
    if (testUpdateFailureMessages(notModified) != TESTPASSED)
    {
        return TESTFAILED;
    }

    return TESTPASSED;
}

