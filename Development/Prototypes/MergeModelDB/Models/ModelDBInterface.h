#ifndef MODELDBINTERFACECORE_H_
#define MODELDBINTERFACECORE_H_

// Project Header Files
#include "CoreDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class ModelDBInterface : public CoreDBInterface
{
public:
    enum class ModelTestStatus {ModelTestPassed, ModelTestFailed};
    ModelDBInterface(std::string_view modelNameIn);
    virtual ~ModelDBInterface() = default;
    bool save();
    bool insert();
    bool update();
    bool retrieve();    // Only select object by object ID.
    bool isInDataBase() const noexcept { return (primaryKey > 0); };
    bool isModified() const noexcept { return modified; };
    void clearModified() { modified = false; };
    bool hasRequiredValues();
    void reportMissingFields() noexcept;
    std::string_view getModelName() const { return modelName; };
    virtual bool runSelfTest();

protected:
/*
 * Each model will have 1 or more required fields, the model must specify what those
 * fields are.
 */
    virtual void initRequiredFields() = 0;
/*
 * Each model must provide formating for Insert, Update and Select by object ID.
 * Additional select statements will be handled by each model as necessary.
 */
    virtual std::string formatInsertStatement() = 0;
    virtual std::string formatUpdateStatement() = 0;
    virtual std::string formatSelectStatement() = 0;
    virtual bool processResult(boost::mysql::results& results);
/*
 * Each model must provide the process by which the database information will
 * be translated into the specific model.
 */
    virtual void processResultRow(boost::mysql::row_view rv) = 0;

/*
 * To process TEXT fields that contain model fields.
 */
    std::vector<std::string> explodeTextField(std::string const& textField) noexcept;
    std::string implodeTextField(std::vector<std::string>& fields) noexcept;

/*
 * Conversions from std::chrono to boost::mysql
 */
    boost::mysql::date stdchronoDateToBoostMySQLDate(const std::chrono::year_month_day& source) noexcept;
    std::chrono::year_month_day boostMysqlDateToChronoDate(const boost::mysql::date& source) noexcept;
    boost::mysql::datetime stdChronoTimePointToBoostDateTime(std::chrono::system_clock::time_point source) noexcept;
    std::chrono::system_clock::time_point boostMysqlDateTimeToChronoTimePoint(boost::mysql::datetime dbDateTime);
    std::optional<boost::mysql::date> optionalDateConversion(std::optional<std::chrono::year_month_day> optDate);
    std::optional<boost::mysql::datetime> optionalDateTimeConversion(std::optional<std::chrono::system_clock::time_point> optDateTime);

// Unit Test Functions
    virtual void selfTestResetAllValues();
    virtual bool testAccessorFunctionsPassed();
    virtual bool testExceptionHandling();
    virtual bool testSave();
    virtual bool testExceptionInsert();
    virtual bool testExceptionUpdate();
    virtual bool testExceptionRetrieve();
    virtual bool testTextFieldManipulation();

    
    template <typename T>
    bool testAccessorFunctions(T testValue, T* member, std::string_view memberName, std::function<void(T)>setFunct, std::function<T(void)>getFunct)
    {
        if (verboseOutput)
        {
            std::clog << "Running self test on set and get functions for " << modelName << "::" << memberName << "\n";
        }

        modified = false;

        setFunct(testValue);
        if (!isModified())
        {
            std::clog << "In self test for: " << modelName << " set function for " << memberName << " FAILED to set modified\n";
            return false;
        }

        if (*member != testValue)
        {
            std::clog  << "In self test for: " << modelName << "Set function for " << memberName << " FAILED to set member value\n";
            return false;
        }

        if (getFunct() != testValue)
        {
            std::clog  << "In self test for: " << modelName << "Get function for " << memberName << " FAILED\n";
            return false;
        }

        if (verboseOutput)
        {
            std::clog << "Self test on set and get functions for " << modelName << "::" << memberName << " PASSED\n";
        }

        return true;
    }

    template <typename T>
    bool testOptionalAccessorFunctions(T testValue, std::optional<T>* member, std::string_view memberName,
        std::function<void(T)>setFunct, std::function<std::optional<T>(void)>getFunct)
    {
        if (verboseOutput)
        {
            std::clog << "Running self test on set and get functions for " << modelName << "::" << memberName << "\n";
        }

        modified = false;

        setFunct(testValue);
        if (!isModified())
        {
            std::clog << "In self test for: " << modelName << " set function for " << memberName << " FAILED to set modified\n";
            return false;
        }

        if (!member->has_value() || member->value() != testValue)
        {
            if (!member->has_value())
            {
                std::clog  << "In self test for: " << modelName << "Set function for " << memberName << " FAILED to set member value\n";
            }
            if (member->value() != testValue)
            {
                std::clog  << "In self test for: " << modelName << " expected value: " << testValue
                         << "actual value: " << member->value() << " FAILED to set member value\n";
            }
            return false;
        }

        std::optional<T> returnValue = getFunct();
        if (!returnValue.has_value() || returnValue.value() != testValue)
        {
            std::clog  << "In self test for: " << modelName << "Get function for " << memberName << " FAILED\n";
            return false;
        }

        if (verboseOutput)
        {
            std::clog << "Self test on set and get functions for " << modelName << "::" << memberName << " PASSED\n";
        }

        return true;
    }

    bool testTimeStampAccessorFunctions(std::chrono::system_clock::time_point testValue,
        std::optional<std::chrono::system_clock::time_point>* member,
        std::string_view memberName,
        std::function<void(std::chrono::system_clock::time_point)>setFunct,
        std::function<std::chrono::system_clock::time_point(void)>getFunct)
    {
        if (verboseOutput)
        {
            std::clog << "Running self test on set and get functions for " << modelName << "::" << memberName << "\n";
        }

        modified = false;

        setFunct(testValue);
        if (!isModified())
        {
            std::clog << "In self test for: " << modelName << " set function for " << memberName << " FAILED to set modified\n";
            return false;
        }

        if (!member->has_value() || member->value() != testValue)
        {
            std::clog  << "In self test for: " << modelName << "Set function for " << memberName << " FAILED to set member value\n";
            return false;
        }

        if (getFunct() != testValue)
        {
            std::clog  << "In self test for: " << modelName << "Get function for " << memberName << " FAILED\n";
            return false;
        }

        if (verboseOutput)
        {
            std::clog << "Self test on set and get functions for " << modelName << "::" << memberName << " PASSED\n";
        }

        return true;
    }

    bool hasErrorMessage();
    ModelDBInterface::ModelTestStatus wrongErrorMessage(std::string expectedString);
    virtual ModelDBInterface::ModelTestStatus testInsertionFailureMessages(std::vector<std::string> expectedErrors);
    virtual ModelDBInterface::ModelTestStatus testUpdateFailureMessages(std::vector<std::string> expectedErrors);
    virtual ModelDBInterface::ModelTestStatus testAllInsertFailures();
    struct ExceptionTestElement
    {
        std::function<bool(void)> testExceptionFunction;
        std::string_view functionUnderTest;
    };

    virtual bool forceExceptionsLoop(std::vector<ExceptionTestElement> exceptionTests);
    ModelDBInterface::ModelTestStatus testCommonInsertFailurePath();
    ModelDBInterface::ModelTestStatus testCommonUpdateFailurePath();

protected:
    std::size_t primaryKey;
    std::string_view modelName;
    bool modified;
    char delimiter;
    struct RequireField
    {
        std::function<bool(void)>errorCondition;
        std::string fieldName;
    };
    std::vector<RequireField> missingRequiredFieldsTests;


    const ModelDBInterface::ModelTestStatus TESTFAILED = ModelDBInterface::ModelTestStatus::ModelTestFailed;
    const ModelDBInterface::ModelTestStatus TESTPASSED = ModelDBInterface::ModelTestStatus::ModelTestPassed;

};

using AnyModel_shp = std::shared_ptr<ModelDBInterface>;

#endif // MODELDBINTERFACECORE_H_


