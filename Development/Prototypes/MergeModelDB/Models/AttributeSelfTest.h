#ifndef ATTRIBUTESELFTEST_H_
#define ATTRIBUTESELFTEST_H_

// Project Header Files
#include "ModelDBInterface.h"
#include "TestStatus.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <concepts>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

/*
 * The AttributeSelfTest template class provides unit testing for the attributes
 * of the model being tested. The tests include setting the attribute value
 * through an attribute accessor function, getting the attrribute value with an
 * accessor function, and testing if the model modified attribute or "dirty" is 
 * set when the value is set.
 * 
 * This class is expected to be used by a class explicitly for testinmg a particular
 * model. The class that tests a particular model will create a list of attribute
 * tests that use the template test methods in this class. 
 */

 using AttributeTestFunction = std::function<TestStatus(void)>;

template<class Model>
requires std::is_base_of_v<ModelDBInterface, Model>
class AttributeSelfTest : public virtual Model
{
protected:
/*****
 * These methods must be implemented.
 * The tests they implement require the details of the particular model being tested.
 * 
 * Init functions should call the local version of selfTestResetAllValues().
 */
    virtual std::vector<AttributeTestFunction> initAttributeAccessTests() noexcept = 0;

public:

    AttributeSelfTest() : Model() { }

    virtual ~AttributeSelfTest() = default;

protected:

/*
 * In some instances a text field is used to contain a list of values. This
 * function provides the ability to test those text fields.
 */
    virtual TestStatus testTextFieldManipulation()
    {
        TestStatus testStatus = TESTPASSED;
        int testFieldCount = 31;
        std::vector<std::string> testInputFields;
        std::string expectedImplodeOutput;

        for (std::size_t i : std::views::iota(1, testFieldCount))
        {
            std::string fieldName = std::format("InputField{}", i);
            testInputFields.push_back(fieldName);
            expectedImplodeOutput.append(fieldName);
            expectedImplodeOutput += ModelDBInterface::delimiter;
        }

        std::string implodeOutput = ModelDBInterface::implodeTextField(testInputFields);
        if (implodeOutput != expectedImplodeOutput)
        {
            testStatus = TESTFAILED;
            std::cerr << std::format("Unit test of implodeTextField() FAILED!\nExpected Output = {}\nActual Output ={}\n",
                expectedImplodeOutput, implodeOutput);
        }

        if (testStatus == TESTPASSED)
        {
            std::vector<std::string> explodeOutput = ModelDBInterface::explodeTextField(implodeOutput);
            if (explodeOutput != testInputFields)
            {
                testStatus = TESTFAILED;
                std::cerr << "Unit test of explodeTextField() FAILED!\n";
            }
        }

        return testStatus;
    }

    virtual TestStatus testAttributeAccessFunctions() noexcept
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

    /*
     * Test the attribute access functions for a particular attribute. Used
     * to test required attributes in a model.
     */
    template <typename U>
    TestStatus testAccessorFunctions(U testValue, U* member, std::string_view memberName,
         std::function<void(U)>setFunct, std::function<U(void)>getFunct) noexcept
    {
        std::string_view modelName(ModelDBInterface::modelName);

        std::cout << "Running self test on set and get functions for " << modelName << "::" << memberName << "\n";

        ModelDBInterface::modified = false;

        setFunct(testValue);
        if (!ModelDBInterface::isModified())
        {
            std::cerr << "In self test for: " << modelName << " Set function for " << memberName << " FAILED to set modified\n";
            return TESTFAILED;
        }

        if (*member != testValue)
        {
            std::cerr  << "In self test for: " << modelName << "Set function for " << memberName << " FAILED to set member value\n";
            return TESTFAILED;
        }

        if (getFunct() != testValue)
        {
            std::cerr  << "In self test for: " << modelName << "Get function for " << memberName << " FAILED\n";
            return TESTFAILED;
        }

        std::cout << "Self test on set and get functions for " << modelName << "::" << memberName << " PASSED\n";

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
        std::string_view modelName(ModelDBInterface::modelName);

        std::cout << "Running self test on set and get functions for " << modelName << "::" << memberName << "\n";

        ModelDBInterface::modified = false;

        setFunct(testValue.value());
        if (!ModelDBInterface::isModified())
        {
            std::cerr << "In self test for: " << modelName << " set function for " << memberName << " FAILED to set modified\n";
            return TESTFAILED;
        }

        if (*member != testValue)
        {
            if (!member->has_value())
            {
                std::cerr  << "In self test for: " << modelName << "Set function for " << memberName << " FAILED to set member value\n";
            }
            if (member->value() != testValue.value())
            {
                std::cerr  << "In self test for: " << modelName << " expected value: " << testValue.value()
                         << "actual value: " << member->value() << " FAILED to set member value\n";
            }
            return TESTFAILED;
        }

        std::optional<U> returnValue = getFunct();
        if (returnValue != testValue)
        {
            std::cerr  << "In self test for: " << modelName << "Get function for " << memberName << " FAILED\n";
            return TESTFAILED;
        }

        std::cout << "Self test on set and get functions for " << modelName << "::" << memberName << " PASSED\n";

        return TESTPASSED;
    }

    /*
     * TimeStamp attributes are required, but the only way to test if the have a
     * value is to store it as an optional value. This method tests the access to
     * TimeStamp attributes.
     */
    TestStatus testTimeStampAccessorFunctions(std::chrono::system_clock::time_point testValue,
        std::optional<std::chrono::system_clock::time_point>* member,
        std::string_view memberName,
        std::function<void(std::chrono::system_clock::time_point)>setFunct,
        std::function<std::chrono::system_clock::time_point(void)>getFunct) noexcept
    {
        std::string_view modelName(ModelDBInterface::modelName);
        std::cout << "Running self test on set and get functions for " << modelName << "::" << memberName << "\n";

        ModelDBInterface::modified = false;

        setFunct(testValue);
        if (!ModelDBInterface::isModified())
        {
            std::cerr << "In self test for: " << modelName << " set function for " << memberName << " FAILED to set modified\n";
            return TESTFAILED;
        }

        if (!member->has_value() || member->value() != testValue)
        {
            std::cerr  << "In self test for: " << modelName << "Set function for " << memberName << " FAILED to set member value\n";
            return TESTFAILED;
        }

        if (getFunct() != testValue)
        {
            std::cerr  << "In self test for: " << modelName << "Get function for " << memberName << " FAILED\n";
            return TESTFAILED;
        }

        std::cout << "Self test on set and get functions for " << modelName << "::" << memberName << " PASSED\n";

        return TESTPASSED;
    }

};

#endif // ATTRIBUTESELFTEST_H_


