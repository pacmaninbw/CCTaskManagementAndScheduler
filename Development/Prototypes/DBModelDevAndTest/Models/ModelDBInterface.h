#ifndef MODELDBINTERFACECORE_H_
#define MODELDBINTERFACECORE_H_

// Project Header Files
#include "CoreDBInterface.h"
#ifdef SELFTEST
#include "TestStatus.h"
#endif // SELFTEST

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#ifdef SELFTEST
using AttributeTestFunction = std::function<TestStatus(void)>;
#endif // SELFTEST

class ModelDBInterface : public CoreDBInterface
{
public:
    ModelDBInterface(std::string modelNameIn, std::string primaryKeyNameIn);
    virtual ~ModelDBInterface() = default;
    virtual bool save() noexcept;
    virtual bool insert() noexcept;
    virtual bool update() noexcept;
    virtual bool hide(std::size_t userRequestingDelete) noexcept;
    bool isInDataBase() const noexcept { return (m_primaryKey > 0); };
    bool isModified() const noexcept { return m_modified; };
    void clearModified() { m_modified = false; };
    bool hasRequiredValues();
    void reportMissingFields() noexcept;
    std::string getModelName() { return m_modelName; };
    bool isDeleted() const noexcept { return m_deleted; };
    std::size_t getPrimaryKey() const noexcept { return m_primaryKey; };
    std::size_t getLastModifiedBy() const { return m_lastModifiedByUser; };   
    void setLastModifiedBy(std::size_t userId); 
    void setLastModifiedTimeStamp(std::chrono::system_clock::time_point lastModifiedTS);
    std::optional<std::chrono::system_clock::time_point> getLastModifiedTimeStamp() { return m_lastUpdateTimeStamp; };
    std::chrono::system_clock::time_point getLastModifiedValue() { return m_lastUpdateTimeStamp.value(); };
    void setCreatedTimeStamp(std::chrono::system_clock::time_point createdTS);
    std::optional<std::chrono::system_clock::time_point> getCreatedTimeStamp() { return m_createdTimeStamp; };
    std::chrono::system_clock::time_point getCreatedTSValue() { return m_createdTimeStamp.value(); };

protected:
/*
 * Each model will have 1 or more required fields, the model must specify what those
 * fields are.
 */
    virtual void initRequiredFields() = 0;
/*
 * Each model must provide formating for Delete, Insert, Update and Select by
 * object ID. Additional select statements will be handled by each model as necessary.
 */
    virtual std::string formatInsertStatement() = 0;
    virtual std::string formatUpdateStatement() = 0;
    virtual std::string formatDeleteStatement() = 0;

/*
 * Get the primary key value after a record is inserted in the database.
 */
    std::size_t getPrimaryKeyValue(boost::mysql::results& dbResultSet);
/*
 * Pre database action checks.
 */
    bool preInsertCheck() noexcept;
    bool preUpdateCheck() noexcept;

 /*
  * common protected variables used by all Models.
  */
    std::size_t m_primaryKey;
    std::string m_modelName;
    std::string m_primaryKeyName;
    bool m_modified;
    bool m_deleted;
    std::size_t m_lastModifiedByUser;
    std::optional<std::chrono::system_clock::time_point> m_createdTimeStamp;
    std::optional<std::chrono::system_clock::time_point> m_lastUpdateTimeStamp;
    char m_delimiter;
    struct RequireField
    {
        std::function<bool(void)> errorCondition;
        std::string fieldName;
    };
    std::vector<RequireField> m_missingRequiredFieldsTests;

#ifdef SELFTEST
/*
 * Attribute Access Testing.
 */
    virtual std::vector<AttributeTestFunction> initAttributeAccessTests() noexcept;
    TestStatus formattedAttributeFailureMessage(bool isSetter, std::string_view memberName, std::string_view message) noexcept;
    virtual TestStatus testAttributeAccessFunctions() noexcept;
    TestStatus testPrimaryKeyAccessFunctions(std::size_t testPrimaryKey, std::function<void(std::size_t)>setFunct,
        std::function<std::size_t(void)>getFunct) noexcept;
    TestStatus testLastModifiedByAccess(std::size_t testUserIdFK) noexcept;
    TestStatus testForeignKeyFields(std::size_t testForeignKey, std::size_t* member, std::string_view memberName,
        std::function<void(std::size_t)>setFunct, std::function<std::size_t(void)>getFunct) noexcept;
    TestStatus testTimeStampAccessorFunctions(std::chrono::system_clock::time_point testValue,
        std::optional<std::chrono::system_clock::time_point>* member, std::string_view memberName,
        std::function<void(std::chrono::system_clock::time_point)>setFunct,
        std::function<std::chrono::system_clock::time_point(void)>getFunct
    ) noexcept;

    /*
     * Test the attribute access functions for a particular attribute. Used
     * to test required attributes in a model.
     */
    template <typename U>
    TestStatus testAccessorFunctions(U testValue, U* member, std::string_view memberName,
         std::function<void(U)>setFunct, std::function<U(void)>getFunct) noexcept
    {
        std::cout << "Running self test on set and get functions for " << m_modelName << "::" << memberName << "\n";

        m_modified = false;

        setFunct(testValue);
        if (!isModified())
        {
            return formattedAttributeFailureMessage(true, memberName, "FAILED to set modified");
        }

        if (*member != testValue)
        {
            return formattedAttributeFailureMessage(true, memberName, "FAILED to set member value");
        }

        if (getFunct() != testValue)
        {
            return formattedAttributeFailureMessage(false, memberName, "FAILED");
        }

        std::cout << "Self test on set and get functions for " << m_modelName << "::" << memberName << " PASSED\n";

        return TESTPASSED;
    }

    /*
     * Test the attribute access functions for a particular optional attribute. Due
     * to the difference of how optional values are stored the method for required
     * attributes can't be used here.
     */
    template <typename U>
    TestStatus testOptionalAccessorFunctions(std::optional<U> testValue, std::optional<U>* member, std::string_view memberName,
        std::function<void(U)>setFunct, std::function<std::optional<U>(void)>getFunct) noexcept
    {
        std::cout << "Running self test on set and get functions for " << m_modelName << "::" << memberName << "\n";

        m_modified = false;

        setFunct(testValue.value());
        if (!isModified())
        {
            return formattedAttributeFailureMessage(true, memberName, "FAILED to set modified");
        }

        if (*member != testValue)
        {
            if (!member->has_value())
            {
                std::cerr  << "In self test for: " << m_modelName << "Set function for " << memberName << " FAILED to set member value\n";
            }
            if (member->value() != testValue.value())
            {
                std::cerr  << "In self test for: " << m_modelName << " expected value: " << testValue.value()
                         << "actual value: " << member->value() << " FAILED to set member value\n";
            }
            return TESTFAILED;
        }

        std::optional<U> returnValue = getFunct();
        if (returnValue != testValue)
        {
            return formattedAttributeFailureMessage(false, memberName, "FAILED");
        }

        std::cout << "Self test on set and get functions for " << m_modelName << "::" << memberName << " PASSED\n";

        return TESTPASSED;
    }

#endif // SELFTTEST
};

using AnyModel_shp = std::shared_ptr<ModelDBInterface>;

#endif // MODELDBINTERFACECORE_H_


