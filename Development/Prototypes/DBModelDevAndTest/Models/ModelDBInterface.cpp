// Project Header Files
#include "CommandLineParser.h"
#include "commonTestValues.h"
#include "ModelDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

ModelDBInterface::ModelDBInterface(std::string modelNameIn, std::string primaryKeyNameIn)
: CoreDBInterface(), m_primaryKey{0}, m_modified{false}, m_deleted{false}, m_lastModifiedByUser{0}
{
    m_modelName = modelNameIn;
    m_primaryKeyName = primaryKeyNameIn;
    m_delimiter = ';';  
}

bool ModelDBInterface::save() noexcept
{
    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified, no changes to save", m_modelName));
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

bool ModelDBInterface::insert() noexcept
{
    if (!preInsertCheck())
    {
        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatInsertStatement());
        m_primaryKey =  getPrimaryKeyValue(localResult);
        m_modified = false;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.insert : {}", m_modelName, e.what()));
        return false;
    }

    return true;
}

bool ModelDBInterface::update() noexcept
{
    if (!preUpdateCheck())
    {
        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatUpdateStatement());
        m_modified = false;
            
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.update : {}", m_modelName, e.what()));
        return false;
    }
}

bool ModelDBInterface::hide([[maybe_unused]]std::size_t userRequestingDelete) noexcept
{
    clearErrorMessages();

    if (!isInDataBase())
    {
        appendErrorMessage(std::format("{} not in Database, nothing to delete!", m_modelName));

        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatDeleteStatement());

        m_deleted = true;
        
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.hide() : {}", m_modelName, e.what()));
        return false;
    }
}


bool ModelDBInterface::hasRequiredValues()
{
    initRequiredFields();
    for (const auto& fieldTest: m_missingRequiredFieldsTests)
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
    for (const auto& testAndReport: m_missingRequiredFieldsTests)
    {
        if (testAndReport.errorCondition())
        {
            appendErrorMessage(std::format("Missing {} required {}!", m_modelName, testAndReport.fieldName));
        }
    }
}

void ModelDBInterface::setPrimaryKey(std::size_t primaryKey, bool setModified)
{
    m_modified = setModified;
    if (!m_forceError)
    {
        m_primaryKey = primaryKey;
    }
}

void ModelDBInterface::setLastModifiedBy(std::size_t userId)
{
    m_modified = true;
    m_lastModifiedByUser = userId;
}

void ModelDBInterface::setLastModifiedTimeStamp(std::chrono::system_clock::time_point lastModifiedTS)
{
    m_modified = true;
    m_lastUpdateTimeStamp = lastModifiedTS;
}

void ModelDBInterface::setCreatedTimeStamp(std::chrono::system_clock::time_point createdTS)
{
    m_modified = true;
    m_createdTimeStamp = createdTS;
}

TestStatus ModelDBInterface::runSelfTest() noexcept
{
    m_selfTest = true;
    TestStatus selfTestStatus = TESTPASSED;

    std::cout << "\nRunning " << getModelName() << " Self Test\n";

    m_forceError = true;
    common::separateTestCaseOutput("*");
    std::cerr << "Forcing Errors" << std::endl;
    selfTestStatus = selfTestSinglePass();

    common::separateTestCaseOutput("*");
    std::cerr << "NO Errors Forced" << std::endl;
    m_forceError = false;
    selfTestStatus = selfTestSinglePass();

    m_selfTest = false;
    
    if (selfTestStatus == TESTPASSED)
    {
        std::cout <<  std::format("{} Self Test {}", getModelName(), "PASSED") << std::endl;
    }
    else
    {
        std::cerr <<  std::format("{} Self Test {}\n", getModelName(), "FAILED");
    }

    return selfTestStatus;
}

TestStatus ModelDBInterface::selfTestSinglePass() noexcept
{
    TestStatus selfTestStatus = TESTPASSED;

    if (testExceptionHandling()!= TESTPASSED)
    {
        std::cerr  << getModelName() << "::runSelfTest: Exception handling FAILED!\n";
        selfTestStatus = TESTFAILED;
    }
    
    if (testSave() == TESTFAILED)
    {
        selfTestStatus = TESTFAILED;
    }

    if (testAttributeAccessFunctions() == TESTFAILED)
    {
        std::cerr << getModelName() << "::runSelfTest: One or more get or set functions FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testEqualityOperator() == TESTFAILED)
    {
        std::cerr << std::format("Equality Operator Test: Comparing 2 {}s FAILED!\n", getModelName());
        selfTestStatus = TESTFAILED;
    }

    testOutput();

    if (testAllInsertFailures() != TESTPASSED)
    {
        std::cerr << "Test of all insertion failures FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Insertion Failure Test PASSED!\n";
    }

    if (testCommonUpdateFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Update Failure Test PASSED!\n";
    }

    return selfTestStatus;
}

std::string ModelDBInterface::commonCoreVariablesOutput() const
{
    std::string output;
    constexpr const char* outFmtStr = "\t{}:\t{}\n";

    output.append("Common Core Variables:\n");
    output.append(std::format(outFmtStr, "Model Name", m_modelName));
    output.append(std::format(outFmtStr, "Primary Key", m_primaryKey));
    output.append(std::format(outFmtStr, "Primary Key Name", m_primaryKeyName));
    output.append(std::format(outFmtStr, "Modified", m_modified? "TRUE" : "FALSE"));
    output.append(std::format(outFmtStr, "Deleted", m_deleted? "TRUE" : "FALSE"));
    output.append(std::format(outFmtStr, "Last Modified by User ID", m_lastModifiedByUser));

    output.append(conditionallyOutputCoreTimeStamps());
    output.append("Model Specific Attributes:\n");

    return output;
}

std::string ModelDBInterface::conditionallyOutputCoreTimeStamps() const
{
    std::string output;
    auto showTime = [&](const std::string& label, std::optional<std::chrono::system_clock::time_point> timeStamp)
    {
        output.append(std::format("\t{}: ", label));
        if (timeStamp.has_value())
        {
            output.append(std::format("{}", timeStamp.value()));
        }
        else
        {
            output.append("NO TIMESTAMP VALUE");
        }
        output.append("\n");
    };

    if (m_showTimeStamps || m_forceError)
    {
        showTime("Created", m_createdTimeStamp);
        showTime("Last Updated", m_lastUpdateTimeStamp);
    }

    return output;
}

std::size_t ModelDBInterface::getPrimaryKeyValue(boost::mysql::results &dbResultSet)
{
    // If this is self test then we don't actually connect to the database, the code that
    // tests insert statements still needs a value returned.
    if (m_selfTest)
    {
        return 1;
    }

    std::size_t pKeyValue = dbResultSet.last_insert_id();

    return pKeyValue;
}

bool ModelDBInterface::preInsertCheck() noexcept
{
    clearErrorMessages();

    if (isInDataBase())
    {
        appendErrorMessage(std::format("{} already in Database, use Update!", m_modelName));
        return false;
    }

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", m_modelName));
        return false;
    }

    if (!hasRequiredValues())
    {
        appendErrorMessage(std::format("{} is missing required values!", m_modelName));
        reportMissingFields();
        return false;
    }

    return true;
}

bool ModelDBInterface::preUpdateCheck() noexcept
{
    clearErrorMessages();

    if (!isInDataBase())
    {
        appendErrorMessage(std::format("{} not in Database, use Insert!", m_modelName));
        return false;
    }

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", m_modelName));
        return false;
    }

    return true;
}

void ModelDBInterface::selfTestResetAllValues() noexcept
{
    m_primaryKey = 0;
    m_modified = false;
    m_lastModifiedByUser = 0;
/*****
 * format_opts needs to be reset before any exception test
 */
    m_formatOpts.reset();
    clearErrorMessages();
}

std::vector<AttributeTestFunction> ModelDBInterface::initAttributeAccessTests() noexcept
{
    return std::vector<AttributeTestFunction>();
}

TestStatus ModelDBInterface::formattedAttributeFailureMessage(bool isSetter, std::string_view memberName, std::string_view message) const noexcept
{
        std::string_view setOrGet = isSetter? "Set" : "Get";

        std::cerr << std::format("In self test for:{} {} function for {} {}\n", m_modelName, setOrGet, memberName, message);

        return TESTFAILED;
}

TestStatus ModelDBInterface::testAttributeAccessFunctions() noexcept
{
    TestStatus testStatus = TESTPASSED;
    std::vector<AttributeTestFunction> attributeAccessTests = initAttributeAccessTests();

    for (const auto& attributeAccessTest: attributeAccessTests)
    {
        if (attributeAccessTest() == TESTFAILED)
        {
            testStatus = TESTFAILED;
        }
    }

    return testStatus ;
}

TestStatus ModelDBInterface::testPrimaryKeyAccessFunctions(std::size_t testPrimaryKey, std::function<void(std::size_t)> setFunct, std::function<std::size_t(void)> getFunct) noexcept
{
        std::cout << "Running self test on set and get functions for " << m_modelName << "::" << m_primaryKeyName << "(Primary Key)\n";

        m_modified = false;

        setFunct(testPrimaryKey);
        if (!isModified())
        {
            return formattedAttributeFailureMessage(true, m_primaryKeyName, "FAILED to set modified");
        }

        if (m_primaryKey != testPrimaryKey)
        {
            return formattedAttributeFailureMessage(true, m_primaryKeyName, "FAILED to set primary key value");
        }

        if (getFunct() != testPrimaryKey)
        {
            return formattedAttributeFailureMessage(false, m_primaryKeyName, "FAILED");
        }

        std::cout << std::format("Self test on access functions for {}::{} (Primary Key) PASSED\n", m_modelName, m_primaryKeyName);

        return TESTPASSED;
}

TestStatus ModelDBInterface::testLastModifiedByAccess(std::size_t testUserIdFK) noexcept
{
    std::string_view memberName("Last Modified by UserId");

    std::cout << "Running self test on set and get functions for " << m_modelName << "::" << memberName << "\n";

    m_modified = false;

    setLastModifiedBy(testUserIdFK);
    if (!isModified())
    {
        return formattedAttributeFailureMessage(true, memberName, "FAILED to set modified");
    }

    if (m_lastModifiedByUser != testUserIdFK)
    {
        return formattedAttributeFailureMessage(true, memberName, "FAILED to set member value");
    }

    if (getLastModifiedBy() != testUserIdFK)
    {
        return formattedAttributeFailureMessage(false, memberName, "FAILED");
    }

    std::cout <<  std::format("Self test on access functions for {}::{} PASSED\n", m_modelName, memberName);

    return TESTPASSED;
}

TestStatus ModelDBInterface::testForeignKeyFields(std::size_t testForeignKey, std::size_t *member, std::string_view memberName, std::function<void(std::size_t)> setFunct, std::function<std::size_t(void)> getFunct) noexcept
{
        std::cout << "Running self test on set and get functions for " << m_modelName << "::" << memberName << "(Foreign Key)\n";

        m_modified = false;

        setFunct(testForeignKey);
        if (!isModified())
        {
            return formattedAttributeFailureMessage(true, memberName, "FAILED to set modified");
        }

        if (*member != testForeignKey)
        {
            return formattedAttributeFailureMessage(true, memberName, "FAILED to set member value");
        }

        if (getFunct() != testForeignKey)
        {
            return formattedAttributeFailureMessage(false, memberName, "FAILED");
        }

        std::cout <<  std::format("Self test on access functions for {}::{} (Foreign Key) PASSED\n", m_modelName, memberName);

        return TESTPASSED;
}

/*
 * TimeStamp attributes are required, but the only way to test if the have a
 * value is to store it as an optional value. This method tests the access to
 * TimeStamp attributes.
 */
TestStatus ModelDBInterface::testTimeStampAccessorFunctions(std::chrono::system_clock::time_point testValue, std::optional<std::chrono::system_clock::time_point> *member, std::string_view memberName, std::function<void(std::chrono::system_clock::time_point)> setFunct, std::function<std::chrono::system_clock::time_point(void)> getFunct) noexcept
{
    std::cout << "Running self test on set and get functions for " << m_modelName << "::" << memberName << "\n";

    m_modified = false;

    setFunct(testValue);
    if (!isModified())
    {
        std::cerr << "In self test for: " << m_modelName << " set function for " << memberName << " FAILED to set modified\n";
        return TESTFAILED;
    }

    if (!member->has_value() || member->value() != testValue)
    {
        std::cerr  << "In self test for: " << m_modelName << "Set function for " << memberName << " FAILED to set member value\n";
        return TESTFAILED;
    }

    if (getFunct() != testValue)
    {
        std::cerr  << "In self test for: " << m_modelName << "Get function for " << memberName << " FAILED\n";
        return TESTFAILED;
    }

    std::cout << "Self test on set and get functions for " << m_modelName << "::" << memberName << " PASSED\n";

    return TESTPASSED;
}

TestStatus ModelDBInterface::testSave()
{
    selfTestResetAllValues();

    TestStatus testStatus = TESTPASSED;

    m_modified = false;

    m_primaryKey = 0;
    std::string_view modelName(m_modelName);
    m_deleted = false;

    if (save())
    {
        std::cout << modelName << "::save worked without being modified\n";
        testStatus = TESTFAILED;
    }
    else
    {
        if (hasErrorMessage() == TESTFAILED)
        {
            std::cout << modelName << "::save Missing expected error messages\n";
            testStatus = TESTFAILED;
        }
        else
        {
            if (wrongErrorMessage("not modified, no changes to save") != TESTPASSED)
            {
                std::cout << modelName << "::save Wrong error message\n";
                testStatus = TESTFAILED;
            }
        }
    }

    return testStatus;
}

TestStatus ModelDBInterface::hasErrorMessage() const
{
    std::string testErrorMessage = getAllErrorMessages();
    if (testErrorMessage.empty())
    {
        std::cerr << "No error message generated! TEST FAILED!\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus ModelDBInterface::wrongErrorMessage(std::string expectedString) const
{
    std::size_t found = m_errorMessages.find(expectedString);
    if (found == std::string::npos)
    {
        std::cerr << "Wrong message generated! TEST FAILED!\n";
        std::cerr << m_errorMessages << "\n";
        std::cerr << "Missing expected: " << expectedString << "\n";
        std::cerr << "Error Messages:" << m_errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus ModelDBInterface::testInsertionFailureMessages(std::vector<std::string> expectedErrors)
{
    if (insert())
    {
        std::cerr << std::format("Inserted {} missing required fields!  TEST FAILED\n", m_modelName);
        return TESTFAILED;
    }

    if (hasErrorMessage() == TESTFAILED)
    {
        return TESTFAILED;
    }

    for (const auto& expectedError: expectedErrors)
    {
        if (wrongErrorMessage(expectedError) == TESTFAILED)
        {
            return TESTFAILED;
        }
    }

    return TESTPASSED;
}

TestStatus ModelDBInterface::testUpdateFailureMessages(std::vector<std::string> expectedErrors)
{
    if (update())
    {
        std::cerr << std::format("Update successful with expected errors!  TEST FAILED\n", m_modelName);
        return TESTFAILED;
    }

    if (hasErrorMessage() == TESTFAILED)
    {
        return TESTFAILED;
    }

    for (const auto& expectedError: expectedErrors)
    {
        if (wrongErrorMessage(expectedError) == TESTFAILED)
        {
            return TESTFAILED;
        }
    }

    return TESTPASSED;
}

TestStatus ModelDBInterface::testCommonInsertFailurePath()
{
    selfTestResetAllValues();

    m_primaryKey = 1;
    m_modified = true;

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

TestStatus ModelDBInterface::testCommonUpdateFailurePath()
{
    selfTestResetAllValues();

    std::vector<std::string> notInDB = {"not in Database"};
    if (testUpdateFailureMessages(notInDB) != TESTPASSED)
    {
        return TESTFAILED;
    }

    m_primaryKey = 1;
    std::vector<std::string> notModified = {"not modified!"};
    if (testUpdateFailureMessages(notModified) != TESTPASSED)
    {
        return TESTFAILED;
    }

    return TESTPASSED;
}

std::vector<ExceptionTestElement> ModelDBInterface::initExceptionTests() noexcept
{
    return std::vector<ExceptionTestElement>();
}

TestStatus ModelDBInterface::testExceptionHandling() noexcept
{
    std::cout << "\n\nRunning Exception Handling Test Cases\n";

    selfTestResetAllValues();

    bool globalForceException = m_forceException;

    std::vector<ExceptionTestElement> exceptionTests = initExceptionTests();

    TestStatus exceptionHandlingPassed = forceExceptionsLoop(exceptionTests);

    m_forceException = globalForceException;

    std::cout << "\n\n";

    return exceptionHandlingPassed;
}

TestStatus ModelDBInterface::forceExceptionsLoop(std::vector<ExceptionTestElement> exceptionTests) const noexcept
{
    TestStatus testAllExceptionHandling = TESTPASSED;
    std::string_view failFunction;
    
    try
    {
        for (const auto& exceptionTest: exceptionTests)
        {
            failFunction = exceptionTest.functionUnderTest;
            if (exceptionTest.testExceptionFunction() == TESTFAILED)
            {
                std::cerr << std::format("{}::{} returned true with exception: Exception Test Failed\n",
                    m_modelName, failFunction);
                testAllExceptionHandling = TESTFAILED;
            }
        }
    }

    catch (std::exception &uncaughtException)
    {
        std::cerr << std::format("{}::{}:: Caught Unhandled Exception!! Test FAILED!\n{}",
            m_modelName, failFunction, uncaughtException.what());
        testAllExceptionHandling = TESTFAILED;
    }

    return testAllExceptionHandling;
}

TestStatus ModelDBInterface::testExceptionReportFailure(bool expectSuccess, bool isBool, std::string_view testExceptionFuncName) const noexcept
{
    std::string reportFailure = std::format("In {}::{}: ", m_modelName, testExceptionFuncName);

    if (expectSuccess)
    {
        reportFailure.append(std::format("expected success returned {} \n", isBool? "false" : "empty string"));
        reportFailure.append(m_errorMessages);
        reportFailure.append("\n");
    }
    else
    {
        reportFailure.append(std::format("expected failure returned {} ", isBool? "true" : "formatted string"));
    }

    std::cout << reportFailure << "\n";

    return TESTFAILED;
}
