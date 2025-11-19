#ifndef FUNCTIONALSELFTEST_H_
#define FUNCTIONALSELFTEST_H_

// Project Header Files
#include "ModelDBInterface.h"
#include "TestStatus.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <concepts>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

/*
 * The FunctionalSelfTest class provides unit testing for methods common to all 
 * models that are not tested by either the attribute testing or the exception
 * testing. Each model has an equality operator that needs to be tested. Each
 * model has an output operator ("<<"). The ModelDBInterface provides methods
 * such as insert(), update() and and retrieve() that are implemented within
 * each model due to the different attributes and requirements of the model.
 * 
 * This class provides the unit tests for those methods.
 */

template<class Model>
requires std::is_base_of_v<ModelDBInterface, Model>
class FunctionalSelfTest :  public virtual Model
{
protected:
/*****
 * These methods must be implemented.
 * The tests they implement require the details of the particular model being tested.
 * 
 * Init functions should call the local version of selfTestResetAllValues().
 */
    virtual void selfTestResetAllValues() noexcept = 0;
    virtual TestStatus testAllInsertFailures() = 0;
    virtual TestStatus testEqualityOperator() noexcept = 0;
    virtual void testOutput() noexcept = 0;

public:

    FunctionalSelfTest() : Model() { }

    virtual ~FunctionalSelfTest() = default;

protected:

    virtual TestStatus testSave()
    {
        selfTestResetAllValues();

        TestStatus testStatus = TESTPASSED;
        ModelDBInterface::modified = false;
        ModelDBInterface::primaryKey = 0;
        std::string_view modelName(ModelDBInterface::modelName);

        if (ModelDBInterface::save())
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

    TestStatus hasErrorMessage()
    {
        std::string testErrorMessage = CoreDBInterface::getAllErrorMessages();
        if (testErrorMessage.empty())
        {
            std::cerr << "No error message generated! TEST FAILED!\n";
            return TESTFAILED;
        }

        return TESTPASSED;
    }

    TestStatus wrongErrorMessage(std::string expectedString)
    {
        std::size_t found = CoreDBInterface::errorMessages.find(expectedString);
        if (found == std::string::npos)
        {
            std::cerr << "Wrong message generated! TEST FAILED!\n";
            std::cerr << CoreDBInterface::errorMessages << "\n";
            std::cerr << "Missing expected: " << expectedString << "\n";
            std::cerr << "Error Messages:" << CoreDBInterface::errorMessages << "\n";
            return TESTFAILED;
        }

        return TESTPASSED;
    }

    virtual TestStatus testInsertionFailureMessages(std::vector<std::string> expectedErrors)
    {
        if (ModelDBInterface::insert())
        {
            std::cerr << std::format("Inserted {} missing required fields!  TEST FAILED\n", ModelDBInterface::modelName);
            return TESTFAILED;
        }

        if (hasErrorMessage() == TESTFAILED)
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

    virtual TestStatus testUpdateFailureMessages(std::vector<std::string> expectedErrors)
    {
        if (ModelDBInterface::update())
        {
            std::cerr << std::format("Update successful with expected errors!  TEST FAILED\n", ModelDBInterface::modelName);
            return TESTFAILED;
        }

        if (hasErrorMessage() == TESTFAILED)
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

    TestStatus testCommonInsertFailurePath()
    {
        selfTestResetAllValues();

        ModelDBInterface::primaryKey = 1;
        ModelDBInterface::modified = true;

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

    TestStatus testCommonUpdateFailurePath()
    {
        selfTestResetAllValues();

        std::vector<std::string> notInDB = {"not in Database"};
        if (testUpdateFailureMessages(notInDB) != TESTPASSED)
        {
            return TESTFAILED;
        }

        ModelDBInterface::primaryKey = 1;
        std::vector<std::string> notModified = {"not modified!"};
        if (testUpdateFailureMessages(notModified) != TESTPASSED)
        {
            return TESTFAILED;
        }

        return TESTPASSED;
    }
    
};

#endif // FUNCTIONALSELFTEST_H_


