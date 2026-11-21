#ifndef MODELDBINTERFACECORE_H_
#define MODELDBINTERFACECORE_H_

// Project Header Files
#include "CoreDBInterface.h"
#include "TestStatus.h"

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

using AttributeTestFunction = std::function<TestStatus(void)>;

struct ExceptionTestElement
{
    std::function<TestStatus(void)> testExceptionFunction;
    std::string_view functionUnderTest;
};


class ModelDBInterface : public CoreDBInterface
{
public:
    ModelDBInterface(std::string modelNameIn, std::string primaryKeyNameIn);
    virtual ~ModelDBInterface() = default;
    virtual bool save() noexcept;
    virtual bool insert() noexcept;
    virtual bool update() noexcept;
    virtual bool hide(std::size_t userRequestingDelete) noexcept;
    bool isInDataBase() const noexcept;
    bool isModified() const noexcept;
    void setModified() noexcept;
    void clearModified();
    bool hasRequiredValues();
    void reportMissingFields() noexcept;
    std::string getModelName() { return m_modelName; };
    bool isDeleted() const noexcept { return m_deleted; };
    void setPrimaryKey(std::size_t primaryKey, bool setModified);
    std::size_t getPrimaryKey() const noexcept;
    std::size_t getLastModifiedBy() const;
    void setLastModifiedBy(std::size_t userId) noexcept; 
    void setLastModifiedTimeStamp(std::chrono::system_clock::time_point lastModifiedTS);
    std::chrono::system_clock::time_point getLastModifiedValue();
    void setCreatedTimeStamp(std::chrono::system_clock::time_point createdTS);
    std::chrono::system_clock::time_point getCreatedTSValue();
    virtual TestStatus runSelfTest() noexcept;

protected:
    virtual TestStatus selfTestSinglePass() noexcept;
    std::string commonCoreVariablesOutput() const;
    std::string conditionallyOutputCoreTimeStamps() const;
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

/*
 * Model unit testing (self testing)
 *
 * Common self test functions
 *
 * The code selfTestResetAllValues() should be called by any override.
 * The override should provide resets for any of the model under tests attributes.
 */
    virtual void selfTestResetAllValues() noexcept;
/*
 * To ensure that the negative paths are also working as expected and to improve
 * test coverage we need to the negative paths in the self test functions provided
 * by the ModelDBInterface. Successful tests basically only test the positive
 * path through the test functions.
 */

    enum class ForceAttributeErrors {ForceNotModified, ForceValueNotSet, ForceGetValueError, ForceExceptionOptionGetValue, NoForceError};
    ForceAttributeErrors m_attributeForceErrorTestIndex = ForceAttributeErrors::NoForceError;
    std::size_t m_testTheTestSize_t;
    std::optional<std::size_t> m_testTheOptionalTestSize;
    TestStatus testNegativePathInCoreTests() noexcept;
    TestStatus testNegativePathAttributeTesting() noexcept;
    TestStatus testTestPrimaryKeyAttribute() noexcept;
    TestStatus testTestLastModifiedByUser() noexcept;
    TestStatus testTestForeignKeyFields() noexcept;
    TestStatus testTestOptionalAccessorFunctions() noexcept;
    void foreignKeySetForNegativeTest(std::size_t testValue) noexcept;
    std::size_t foreignKeyGetForNegativeTest() const noexcept;
    void negativePathSetprimaryKey(std::size_t testValue) { setPrimaryKey(testValue, true); };
    void negativePathSetOptionalSize_t(std::size_t testValue) noexcept;
    std::size_t negativePatGetOptionalSize_t();

    bool isForcingAttributeError(ForceAttributeErrors currentError) const;
    void modifyTimeStampForNegativePathTest(std::optional<std::chrono::system_clock::time_point>& timeStamp);

/*
 * Attribute Access Unit Testing.
 */
    virtual std::vector<AttributeTestFunction> initAttributeAccessTests() noexcept;
    TestStatus formattedAttributeFailureMessage(bool isSetter, std::string_view memberName, std::string_view message) const noexcept;
    TestStatus testAttributeAccessFunctions() noexcept;
    TestStatus testPrimaryKeyAccessFunctions(std::size_t testPrimaryKey, std::function<void(std::size_t)>setFunct,
        std::function<std::size_t(void)>getFunct) noexcept;
    TestStatus testLastModifiedByAccess(std::size_t testUserIdFK) noexcept;
    TestStatus testForeignKeyFields(std::size_t testForeignKey, std::size_t* member, std::string_view memberName,
        std::function<void(std::size_t)>setFunct, std::function<std::size_t(void)>getFunct) noexcept;
    TestStatus testTimeStampAccessorFunctions(std::chrono::system_clock::time_point testValue,
        std::optional<std::chrono::system_clock::time_point>* member, std::string_view memberName,
        std::function<void(std::chrono::system_clock::time_point)>setFunct,
        std::function<std::chrono::system_clock::time_point(void)>getValueFunct
    ) noexcept;
    TestStatus testCreatedTimeStampAccess() noexcept;
    TestStatus testLastModifiedTimeStampAccess() noexcept;
    /*
     * Test the attribute access functions for a particular attribute. Used
     * to test data attributes in a model. The set function is expected to
     * set the value of the attribute value and set the modified flag of the
     * class.
     */
    template <typename U>
    TestStatus testAccessorFunctions(U testValue, U* member, std::string_view memberName,
         std::function<void(U)>setFunct, std::function<U(void)>getFunct) noexcept
    {
        std::cout << "Running self test on access functions for " << m_modelName << "::" << memberName << "\n";

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

        std::cout << "Self test on access functions for " << m_modelName << "::" << memberName << " PASSED\n";

        return TESTPASSED;
    }

    /*
     * Test the attribute access functions for a particular optional data attribute.
     */
    template <typename U>
    TestStatus testOptionalAccessorFunctions(U testValue, std::optional<U>* member, std::string_view memberName,
        std::function<void(U)>setFunct, std::function<std::optional<U>(void)>getFunct) noexcept
    {
        std::cout << "Running self test on access functions for " << m_modelName << "::" << memberName << "\n";

        setFunct(testValue);
        if (!isModified())
        {
            return formattedAttributeFailureMessage(true, memberName, "FAILED to set modified");
        }

        if (*member != testValue)
        {
            if (!member->has_value())
            {
                std::cerr  << "In self test for: " << m_modelName << "Set function for " << memberName << " FAILED to set member value\n";
                return TESTFAILED;
            }
            if (member->value() != testValue)
            {
                std::cerr  << "In self test for: " << m_modelName << " expected value: " << testValue
                         << "actual value: " << member->value() << " FAILED to set member value\n";
            }
            return TESTFAILED;
        }

        std::optional<U> returnValue = getFunct();
        if (returnValue != testValue)
        {
            return formattedAttributeFailureMessage(false, memberName, "FAILED");
        }

        std::cout << "Self test on access get functions for " << m_modelName << "::" << memberName << " PASSED\n";

        return TESTPASSED;
    }

/*
 * Self tests of functionality, provides unit testing for methods common to all 
 * models that are not tested by either the attribute testing or the exception
 * testing. Each model has an equality operator that needs to be tested. Each
 * model has an output operator ("<<"). The ModelDBInterface provides methods
 * such as insert(), update() and and retrieve() that are implemented within
 * each model due to the different attributes and requirements of the model.
 */
    virtual TestStatus testAllInsertFailures() { return TESTFAILED; };
    virtual TestStatus testEqualityOperator() noexcept  { std::cerr << "Entered ModelDBInterface::testEqualityOperator()\n"; return TESTFAILED; };
    virtual void testOutput() noexcept  { };
    virtual TestStatus testSave();
    TestStatus hasErrorMessage() const;
    TestStatus wrongErrorMessage(std::string expectedString) const;
    virtual TestStatus testInsertionFailureMessages(std::vector<std::string> expectedErrors);
    virtual TestStatus testUpdateFailureMessages(std::vector<std::string> expectedErrors);
    TestStatus testCommonInsertFailurePath();
    TestStatus testCommonUpdateFailurePath();

/*
 * Each of the model classes will be used in application that provides a graphic
 * user interface. The application needs to be robust and handle exceptions as
 * they occur in a manor where the application will provide necessary information
 * to the user if an error occurs. Error messages returned by the libraries in
 * use may not be meaningful to a user. This class tests the functions in the
 * model that may receive an exception from the underlying libraries.
 * 
 * Exceptions are forced at what be the library level to test the exception
 * handling capability of the model.
 * 
 * An exception test should first test what the function will do if an exception
 * is thrown and then test to see that the function properly works otherwise.
 * 
 * A list of exception tests needs tyo be provided by the class that is testing
 * the model.
 */
    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept;
    virtual TestStatus testExceptionInsert() { return TESTFAILED; };     // Should be added to the vector returned by initExceptionTests()
    virtual TestStatus testExceptionUpdate() { return TESTFAILED; };     // Should be added to the vector returned by initExceptionTests()
    virtual TestStatus testExceptionHide() { return TESTFAILED; };   // Should be added to the vector returned by initExceptionTests()
    TestStatus testExceptionHandling() noexcept;
    TestStatus forceExceptionsLoop(std::vector<ExceptionTestElement> exceptionTests) const noexcept;
    TestStatus testExceptionReportFailure(bool expectSuccess, bool isBool, std::string_view testExceptionFuncName) const noexcept;
    TestStatus testCoreExceptionHandling() noexcept;
    TestStatus testNegativePathExceptionHandling() noexcept;
    bool mockBoolFunctionForExceptionTesting();
    std::string mockStringFunctionForExceptionTesting();

    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    TestStatus testExceptionAndSuccessNArgs(std::string_view funcName, F funcUnderTest, Ts... args) noexcept
    {
        std::cout << "Running exception test on " << funcName << "\n";
        m_forceException = true;
        if (funcUnderTest(args...))
        {
            return testExceptionReportFailure(false, true, funcName);
        }

        m_forceException = false;
        if (m_forceError)
        {
            if (funcUnderTest(args...))
            {
                return testExceptionReportFailure(false, true, funcName);
            }
        }

        std::cout << funcName << " Exception Test Passed\n";
        return TESTPASSED;
    }

/*
 * Used to test formatSQL functions that do not contain a try catch block.
 */
    template <typename F, typename... Ts>
    requires std::is_invocable_v<F, Ts...>
    TestStatus testFormatExceptionCatchSuccessNArgs(std::string_view funcName, F funcUnderTest, Ts... args) noexcept
    {
        std::string formattedQuery;

        try
        {
            m_forceException = true;
            formattedQuery = funcUnderTest(args...);
            return testExceptionReportFailure(false, false, funcName);    
        }
        catch(const std::exception& e)
        {
            std::cout << "Caught expected exception from " << funcName << "(): " << e.what() << "\n";
        }        

        return TESTPASSED;
    }

#ifdef DEBUG_FORCE_ATTRIBUTE_ERRORS
    void debugShowForceErrorType(ForceAttributeErrors currentError) const
    {
        std::string_view outputString;
        switch (currentError)
        {
            case ForceAttributeErrors::ForceNotModified:
                outputString = "Forcing Not Modified Error";
                break;
            
            case ForceAttributeErrors::ForceValueNotSet:
                outputString = "Forcing Value Not Set Error";
                break;

            case ForceAttributeErrors::ForceGetValueError:
                outputString = "Forcing Get Value Error";
                break;

            case ForceAttributeErrors::ForceExceptionOptionGetValue:
                outputString = "Forceing Exceptions on Get Optional Value";
                break;

            case ForceAttributeErrors::NoForceError:
                outputString = "Not Forcing any Errors";
                break;

            default:
                outputString = "Unknown force attribute error type";
                break;
        }

        std::cerr << std::format("Forcing error m_forceError {} attribute error type {}\n", m_forceError? "TRUE" : "FALSE", outputString);
    }
#endif

};

using AnyModel_shp = std::shared_ptr<ModelDBInterface>;

#endif // MODELDBINTERFACECORE_H_


