// Project Header Files
#include "CommandLineParser.h"
#include "commonTestValues.h"
#include "ModelDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <iostream>
#include <optional>
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
        clearModified();
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
        clearModified();
            
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

bool ModelDBInterface::isInDataBase() const noexcept
{
    return !m_forceError? (m_primaryKey > 0) : false;
}

bool ModelDBInterface::isModified() const noexcept
{
    return !isForcingAttributeError(ForceAttributeErrors::ForceNotModified)? m_modified: false;
}

void ModelDBInterface::setModified() noexcept
{
    if (m_selfTest && isForcingAttributeError(ForceAttributeErrors::ForceNotModified))
    {
        return;
    }

    m_modified = true;
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
    if (!isForcingAttributeError(ForceAttributeErrors::ForceNotModified))
    {
        m_modified = setModified;
    }
    if (!isForcingAttributeError(ForceAttributeErrors::ForceValueNotSet))
    {
        m_primaryKey = primaryKey;
    }
}

std::size_t ModelDBInterface::getPrimaryKey() const noexcept
{
    if (isForcingAttributeError(ForceAttributeErrors::ForceGetValueError))
    {
        return ~m_primaryKey;
    }

    return m_primaryKey;
}

std::size_t ModelDBInterface::getLastModifiedBy() const
{
    return !isForcingAttributeError(ForceAttributeErrors::ForceGetValueError)? m_lastModifiedByUser : ~m_lastModifiedByUser;   
}

void ModelDBInterface::setLastModifiedBy(std::size_t userId) noexcept
{
    setModified();

    if (!isForcingAttributeError(ForceAttributeErrors::ForceValueNotSet))
    {
        m_lastModifiedByUser = userId;
    }
}

void ModelDBInterface::setLastModifiedTimeStamp(std::chrono::system_clock::time_point lastModifiedTS)
{
    setModified();

    if (!isForcingAttributeError(ForceAttributeErrors::ForceValueNotSet))
    {
        m_lastUpdateTimeStamp = lastModifiedTS;
    }
}

std::chrono::system_clock::time_point ModelDBInterface::getLastModifiedValue()
{
    if (m_forceError)
    {
        modifyTimeStampForNegativePathTest(m_lastUpdateTimeStamp);
    }

    return m_lastUpdateTimeStamp.value();
}

void ModelDBInterface::setCreatedTimeStamp(std::chrono::system_clock::time_point createdTS)
{
    setModified();

    if (!isForcingAttributeError(ForceAttributeErrors::ForceValueNotSet))
    {
        m_createdTimeStamp = createdTS;
    }
}

std::chrono::system_clock::time_point ModelDBInterface::getCreatedTSValue()
{
    if (m_forceError)
    {
        modifyTimeStampForNegativePathTest(m_createdTimeStamp);
    }

    return m_createdTimeStamp.value();
}

/*
 * The self test is unit testing.
 */
TestStatus ModelDBInterface::runSelfTest() noexcept
{
    m_selfTest = true;
    TestStatus selfTestStatus = TESTPASSED;

    std::cout << "\nRunning " << getModelName() << " Self Test\n";

    common::separateTestCaseOutput("*");
    std::cerr << "Running Core Error Check" << std::endl;
    selfTestStatus = testNegativePathInCoreTests();

#if 0
/*
 * To improve test coverage and to ensure that the negative path through each
 * function is tested, force errors. Then test the positive path through the
 * functions.
 */
    m_forceError = true;
    common::separateTestCaseOutput("*");
    std::cerr << "Forcing Errors" << std::endl;
    if (selfTestSinglePass() == TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
#endif 
    common::separateTestCaseOutput("*");
    std::cerr << "NO Errors Forced" << std::endl;
    m_forceError = false;
    if (selfTestSinglePass() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }

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
    clearModified();
    m_lastModifiedByUser = 0;
    m_createdTimeStamp.reset();
    m_lastUpdateTimeStamp.reset();
/*****
 * format_opts needs to be reset before any exception test
 */
    m_formatOpts.reset();
    clearErrorMessages();
}

TestStatus ModelDBInterface::testNegativePathInCoreTests() noexcept
{
    TestStatus negativePathsAllFail = TESTPASSED;

    std::vector<std::function<TestStatus(void)>> negativePathTestFunctions;
    negativePathTestFunctions.push_back({std::bind(&ModelDBInterface::testNegativePathAttributeTesting, this)});

    for (auto negativeTest: negativePathTestFunctions)
    {
        if (negativeTest() != TESTPASSED)
        {
            negativePathsAllFail = TESTFAILED;
        }

    }

    return negativePathsAllFail;
}

TestStatus ModelDBInterface::testNegativePathAttributeTesting() noexcept
{
    TestStatus testStatus = TESTPASSED;
    bool tempM_forceError = m_forceError;

    std::vector<AttributeTestFunction> negativeAttributePathTestFunctions;
    negativeAttributePathTestFunctions.push_back({std::bind(&ModelDBInterface::testTestPrimaryKeyAttribute, this)});
    negativeAttributePathTestFunctions.push_back({std::bind(&ModelDBInterface::testTestLastModifiedByUser, this)});
    negativeAttributePathTestFunctions.push_back({std::bind(&ModelDBInterface::testTestForeignKeyFields, this)});
    negativeAttributePathTestFunctions.push_back({std::bind(&ModelDBInterface::testCreatedTimeStampAccess, this)});
    negativeAttributePathTestFunctions.push_back({std::bind(&ModelDBInterface::testLastModifiedTimeStampAccess, this)});
    negativeAttributePathTestFunctions.push_back({std::bind(&ModelDBInterface::testTestOptionalAccessorFunctions, this)});

    std::vector<ForceAttributeErrors> forceErrorTypes =
    {
        ForceAttributeErrors::ForceNotModified,
        ForceAttributeErrors::ForceValueNotSet,
        ForceAttributeErrors::ForceGetValueError,
        ForceAttributeErrors::ForceExceptionOptionGetValue,
        ForceAttributeErrors::NoForceError
    };

    for (const auto& testTheTest: negativeAttributePathTestFunctions)
    {
        for (const auto& errorType: forceErrorTypes)
        {
            m_forceError = errorType != ForceAttributeErrors::NoForceError? true : false;
            m_attributeForceErrorTestIndex = errorType;
            if (testTheTest() == TESTPASSED && errorType == ForceAttributeErrors::NoForceError)
            {
                testStatus = TESTPASSED;
            }
            else
            {
                testStatus = TESTFAILED;
            }
        }
    }

    m_forceError = tempM_forceError;

    return testStatus;
}

TestStatus ModelDBInterface::testTestPrimaryKeyAttribute() noexcept
{
    selfTestResetAllValues();
    
    std::size_t testPrimaryKeyValue = 0xFF;

    return testPrimaryKeyAccessFunctions(testPrimaryKeyValue,
        std::bind(&ModelDBInterface::negativePathSetprimaryKey, this, std::placeholders::_1),
        std::bind(&ModelDBInterface::getPrimaryKey, this));
 
}

TestStatus ModelDBInterface::testTestLastModifiedByUser() noexcept
{
    selfTestResetAllValues();
    
    std::size_t testUserID = 0xFE;

    return testLastModifiedByAccess(testUserID);
}

TestStatus ModelDBInterface::testTestForeignKeyFields() noexcept
{
    selfTestResetAllValues();
    m_testTheTestSize_t = 0;

    std::size_t testValue = 0xFD;

    return testForeignKeyFields(testValue, &m_testTheTestSize_t, "Foreign Key Test",
        std::bind(&ModelDBInterface::foreignKeySetForNegativeTest, this, std::placeholders::_1),
        std::bind(&ModelDBInterface::foreignKeyGetForNegativeTest, this));
}

TestStatus ModelDBInterface::testTestOptionalAccessorFunctions() noexcept
{
    selfTestResetAllValues();
    m_testTheOptionalTestSize.reset();

    std::size_t testValue = 0xFC;

    return testOptionalAccessorFunctions<std::size_t>(testValue, &m_testTheOptionalTestSize, "Test Core Optional access test",
        std::bind(&ModelDBInterface::negativePathSetOptionalSize_t, this, std::placeholders::_1),
        std::bind(&ModelDBInterface::negativePatGetOptionalSize_t, this));
}

void ModelDBInterface::foreignKeySetForNegativeTest(std::size_t testValue) noexcept
{
    setModified();

    if (!isForcingAttributeError(ForceAttributeErrors::ForceValueNotSet))
    {
        m_testTheTestSize_t = testValue;
    }

}

std::size_t ModelDBInterface::foreignKeyGetForNegativeTest() const noexcept
{
    return !isForcingAttributeError(ForceAttributeErrors::ForceGetValueError)? m_testTheTestSize_t : ~m_testTheTestSize_t;
}

void ModelDBInterface::negativePathSetOptionalSize_t(std::size_t testValue) noexcept
{
    setModified();
    if (!isForcingAttributeError(ForceAttributeErrors::ForceValueNotSet))
    {
        m_testTheOptionalTestSize = testValue;
    }
    if (isForcingAttributeError(ForceAttributeErrors::ForceGetValueError))
    {
        m_testTheOptionalTestSize = ~testValue;
    }
    if (isForcingAttributeError(ForceAttributeErrors::ForceExceptionOptionGetValue))
    {
        m_testTheOptionalTestSize.reset();
    }
}

std::size_t ModelDBInterface::negativePatGetOptionalSize_t()
{
    if (!isForcingAttributeError(ForceAttributeErrors::ForceGetValueError))
    {
        if (m_testTheOptionalTestSize.has_value())
        {
            m_testTheOptionalTestSize = ~m_testTheOptionalTestSize.value();
        }
    }

    return m_testTheOptionalTestSize.value_or(0);
}

bool ModelDBInterface::isForcingAttributeError(ForceAttributeErrors currentError) const
{
    return (m_forceError && m_attributeForceErrorTestIndex == currentError);
}

void ModelDBInterface::modifyTimeStampForNegativePathTest(std::optional<std::chrono::system_clock::time_point> &timeStamp)
{
    if (isForcingAttributeError(ForceAttributeErrors::ForceGetValueError))
    {
        if (timeStamp.has_value())
        {
            if (m_forceException)
            {
                timeStamp.reset();
            }
            else
            {
                timeStamp = std::chrono::system_clock::now();
            }
        } else
        {
            timeStamp = common::TestTimeStampValue;
        }
    }

    if (isForcingAttributeError(ForceAttributeErrors::ForceExceptionOptionGetValue))
    {
        timeStamp.reset();
    }
}

std::vector<AttributeTestFunction> ModelDBInterface::initAttributeAccessTests() noexcept
{
    return std::vector<AttributeTestFunction>();
}

TestStatus ModelDBInterface::formattedAttributeFailureMessage(bool isSetter, std::string_view memberName, std::string_view message) const noexcept
{
        std::string_view setOrGet = isSetter? "Set" : "Get";

        std::cerr << std::format("In self test for {} {} function for {} {}\n", m_modelName, setOrGet, memberName, message);

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
    std::string alteredMemberName = std::format("{} (Primary Key)", m_primaryKeyName);

    return testAccessorFunctions<std::size_t>(testPrimaryKey, &m_primaryKey, alteredMemberName, setFunct, getFunct);
}

TestStatus ModelDBInterface::testLastModifiedByAccess(std::size_t testUserIdFK) noexcept
{
    selfTestResetAllValues();

    std::string_view memberName("Last Modified by UserId");

    return testForeignKeyFields(testUserIdFK, &m_lastModifiedByUser, memberName,
        std::bind(&ModelDBInterface::setLastModifiedBy, this, std::placeholders::_1),
        std::bind(&ModelDBInterface::getLastModifiedBy, this));
}

TestStatus ModelDBInterface::testForeignKeyFields(std::size_t testForeignKey, std::size_t *member, std::string_view memberName, std::function<void(std::size_t)> setFunct, std::function<std::size_t(void)> getFunct) noexcept
{
    std::string alteredMemberName = std::format("{} (Foreign Key)", memberName);

    return testAccessorFunctions<std::size_t>(testForeignKey, member, alteredMemberName, setFunct, getFunct);
}

/*
 * TimeStamp attributes are required, but the only way to test if the have a
 * value is to store it as an optional value. This method tests the access to
 * TimeStamp attributes.
 */
TestStatus ModelDBInterface::testTimeStampAccessorFunctions(
    std::chrono::system_clock::time_point testValue,
    std::optional<std::chrono::system_clock::time_point> *member,
    std::string_view memberName,
    std::function<void(std::chrono::system_clock::time_point)> setFunct,
    std::function<std::chrono::system_clock::time_point(void)> getValueFunct) noexcept
{
    std::cout << "Running self test on access functions for " << m_modelName << "::" << memberName << "\n";

    clearModified();

    setFunct(testValue);
    if (!isModified())
    {
        return formattedAttributeFailureMessage(true, memberName, "FAILED to set modified");
    }

    if (!member->has_value() || member->value() != testValue)
    {
        return formattedAttributeFailureMessage(true, memberName, "FAILED to set member value");
    }

    try {
        if (getValueFunct() != testValue)
        {
            return formattedAttributeFailureMessage(false, memberName, "FAILED");
        }
    }
    catch (const std::bad_optional_access& e)
    {
        std::cerr  << "In self test for: " << m_modelName << " Get function for " << memberName << " FAILED " << e.what() << "\n";
        return TESTFAILED;
    }

    std::cout << "Self test on access functions for " << m_modelName << "::" << memberName << " PASSED\n";

    return TESTPASSED;
}

TestStatus ModelDBInterface::testCreatedTimeStampAccess() noexcept
{
    selfTestResetAllValues();

    return testTimeStampAccessorFunctions(common::TestTimeStampValue, &m_createdTimeStamp, "Creation Time Stamp",
        std::bind(&ModelDBInterface::setCreatedTimeStamp, this, std::placeholders::_1),
        std::bind(&ModelDBInterface::getCreatedTSValue, this));

}

TestStatus ModelDBInterface::testLastModifiedTimeStampAccess() noexcept
{
    selfTestResetAllValues();

    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_lastUpdateTimeStamp, "Last Modified Time Stamp",
        std::bind(&ModelDBInterface::setLastModifiedTimeStamp, this, std::placeholders::_1),
        std::bind(&ModelDBInterface::getLastModifiedValue, this));
}

TestStatus ModelDBInterface::testSave()
{
    TestStatus testStatus = TESTPASSED;

    try
    {
        selfTestResetAllValues();

        clearModified();

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
    }
    catch(const std::exception& e)
    {
        testStatus = TESTFAILED;
        std::cerr << std::format("Caught exception in {}::{} {}\n", getModelName(), __func__, e.what());
        std::cerr << std::format("m_forceError = {}\n", m_forceError? "TRUE" : "FALSE");
        std::cerr << std::format("m_forceException = {}\n", m_forceException? "TRUE" : "FALSE");
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
