// Project Header Files
#include "ModelDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <exception>
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
    for (auto fieldTest: m_missingRequiredFieldsTests)
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
    for (auto testAndReport: m_missingRequiredFieldsTests)
    {
        if (testAndReport.errorCondition())
        {
            appendErrorMessage(std::format("Missing {} required {}!", m_modelName, testAndReport.fieldName));
        }
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

#ifdef SELFTEST
std::vector<AttributeTestFunction> ModelDBInterface::initAttributeAccessTests() noexcept
{
    return std::vector<AttributeTestFunction>();
}

TestStatus ModelDBInterface::formattedAttributeFailureMessage(bool isSetter, std::string_view memberName, std::string_view message) noexcept
{
        std::string_view setOrGet = isSetter? "Set" : "Get";

        std::cerr << std::format("In self test for:{} {} function for {} {}\n", m_modelName, setOrGet, memberName, message);

        return TESTFAILED;
}

TestStatus ModelDBInterface::testAttributeAccessFunctions() noexcept
{
    TestStatus testStatus = TESTPASSED;
    std::vector<AttributeTestFunction> attributeAccessTests = initAttributeAccessTests();

    for (auto attributeAccessTest: attributeAccessTests)
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
        if (!m_modified)
        {
            return formattedAttributeFailureMessage(true, m_primaryKeyName, "FAILED to set modified");
        }

        if (m_primaryKey != testPrimaryKey)
        {
            return formattedAttributeFailureMessage(true, m_primaryKeyName, "FAILED to set member value");
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

        return TESTPASSED;}

#endif // SELFTEST
