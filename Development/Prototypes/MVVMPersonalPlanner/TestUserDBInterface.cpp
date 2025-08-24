#include "CommandLineParser.h"
#include "commonUtilities.h"
#include "CSVReader.h"
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include "TestUserDBInterface.h"
#include "UserDbInterface.h"
#include "UserModel.h"
#include <vector>

TestUserDBInterface::TestUserDBInterface(std::string userFileName)
: dataFileName{userFileName}, verboseOutput{programOptions.verboseOutput}
{
    positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByLoginName, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByLoginAndPassword, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByFullName, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testUpdateUserPassword, this, std::placeholders::_1));

    negativePathTestFuncs.push_back(std::bind(&TestUserDBInterface::negativePathMissingRequiredFields, this));
}

bool TestUserDBInterface::runAllTests()
{
    userDBInterface.initFormatOptions();
    
    if (positivePathTests())
    {
        return negativePathTests();
    }
    
    return false;
}

bool TestUserDBInterface::negativePathTests()
{
    std::cerr << "TestUserDBInterface::negativePathTests() NOT IMPLEMENTED!!\n";

    bool allTestsPassed = true;

    for (auto test: negativePathTestFuncs)
    {
        if (allTestsPassed)
        {
            allTestsPassed = test();
        }
    }

    if (allTestsPassed)
    {
        std::clog << "All negative path user tests PASSED\n";
    }
    else
    {
        std::clog << "Some or all negative user tests FAILED!\n";
    }

    return allTestsPassed;
}

bool TestUserDBInterface::positivePathTests()
{
    UserList userProfileTestData;
    addFirstUser(userProfileTestData);

    if (!loadTestUsersFromFile(userProfileTestData))
    {
        return false;
    }

    userDBInterface.initFormatOptions();

    bool allTestsPassed = true;

    for (auto user: userProfileTestData)
    {
        user->setUserID(userDBInterface.insert(user));
        if (user->isInDataBase())
        {
            for (auto test: positiveTestFuncs)
            {
                if (!test(user))
                {
                    allTestsPassed = false;
                }
            }
        }
        else
        {
            std::clog << "Primary key for user: " << user->getLastName() << ", " << user->getFirstName() << " not set!\n";
            if (verboseOutput)
            {
                std::clog << *user << "\n\n";
            }
            allTestsPassed = false;
        }
    }

    if (allTestsPassed)
    {
        allTestsPassed = testGetAllUsers(userProfileTestData);
    }

    userProfileTestData.clear();

    if (allTestsPassed)
    {
        std::clog << "Insertion and retrieval of users test PASSED!\n";
        return true;
    }
    else
    {
        std::cerr << "Some or all insertion and retrieval of users test FAILED!\n";
        return false;
    }
}

bool TestUserDBInterface::testGetUserByLoginName(UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = userDBInterface.getUserByLoginName(insertedUser->getLoginName());
    if (retrievedUser)
    {
        if (*retrievedUser == *insertedUser)
        {
            return true;
        }
        else
        {
            std::cerr << "Insertion user and retrieved User are not the same. Test FAILED!\nInserted User:\n" <<
            *insertedUser << "\n" "Retreived User:\n" << *retrievedUser << "\n";
            return false;
        }
    }
    else
    {
        std::cerr << "userDBInterface.getUserByLogin(user->getLoginName()) FAILED!\n" <<
            userDBInterface.getAllErrorMessages() << "\n";
        return false;
    }
}

bool TestUserDBInterface::testGetUserByLoginAndPassword(UserModel_shp insertedUser)
{
    std::string_view testName = insertedUser->getLoginName();
    std::string_view testPassword = insertedUser->getPassword();

    UserModel_shp retrievedUser = userDBInterface.getUserByLoginAndPassword(testName, testPassword);
    if (retrievedUser)
    {
        if (*retrievedUser != *insertedUser)
        {
            std::cerr << "Insertion user and retrieved User are not the same. Test FAILED!\nInserted User:\n" <<
            *insertedUser << "\n" "Retreived User:\n" << *retrievedUser << "\n";
            return false;
        }
    }
    else
    {
        std::cerr << "userDBInterface.getUserByLogin(user->getLoginName()) FAILED!\n" <<
            userDBInterface.getAllErrorMessages() << "\n";
        return false;
    }

    retrievedUser = userDBInterface.getUserByLoginAndPassword(testName, "NotThePassword");
    if (retrievedUser)
    {
        std::cerr << "userDBInterface.getUserByLogin(user->getLoginName()) Found user with fake password!\n";
        return false;
    }

    return true;
}

bool TestUserDBInterface::testGetUserByFullName(UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = userDBInterface.getUserByFullName(insertedUser->getLastName(),
        insertedUser->getFirstName(), insertedUser->getMiddleInitial());
    if (retrievedUser)
    {
        if (*retrievedUser == *insertedUser)
        {
            return true;
        }
        else
        {
            std::cerr << "Insertion user and retrieved User are not the same. Test FAILED!\nInserted User:\n" <<
            *insertedUser << "\n" "Retreived User:\n" << *retrievedUser << "\n";
            return false;
        }
    }
    else
    {
        std::cerr << "userDBInterface.getUserByFullName() FAILED!\n" <<
            userDBInterface.getAllErrorMessages() << "\n";
        return false;
    }
}

bool TestUserDBInterface::testUpdateUserPassword(UserModel_shp insertedUser)
{
    bool testPassed = true;
    UserModel oldUserValues = *insertedUser;
    std::string newPassword = "MyNew**&pAs5Word" + std::to_string(oldUserValues.getUserID());

    insertedUser->setPassword(newPassword);
    if (!userDBInterface.update(insertedUser))
    {
        std::cerr << "userDBInterface.update(insertedUser) FAILED" << userDBInterface.getAllErrorMessages() << "\n";
        return false;
    }

    UserModel_shp newUserValues = userDBInterface.getUserByUserID(insertedUser->getUserID());
    if (oldUserValues == *newUserValues)
    {
        std::clog << std::format("Password update for user {} FAILED!\n", oldUserValues.getUserID());
        testPassed = false;
    }

    return testPassed;
}

bool TestUserDBInterface::loadTestUsersFromFile(UserList &userProfileTestData)
{
    std::ifstream userData(dataFileName);

    if (!userData.is_open())
    {
        std::cerr << "Can't open \"" << dataFileName << "\" for input!" << std::endl;
        return false;
    }
    
    for (auto row: CSVRange(userData))
    {
        UserModel_shp userIn = std::make_shared<UserModel>(UserModel());
        userIn->setLastName(row[0]);
        userIn->setFirstName(row[1]);
        userIn->setMiddleInitial(row[2]);
        userIn->setEmail(row[3]);
        userIn->autoGenerateLoginAndPassword();
        userProfileTestData.push_back(userIn);
    }

    if (userData.bad())
    {
        std::cerr << "Fatal error with file stream: \"" << dataFileName << "\"" << std::endl;
        return false;
    }

    return true;
}

bool TestUserDBInterface::testGetAllUsers(UserList userProfileTestData)
{
    UserList allUsers = userDBInterface.getAllUsers();
    bool testPassed = false;

    if ((userProfileTestData.size() == allUsers.size()) &&
        std::equal(userProfileTestData.begin(), userProfileTestData.end(), allUsers.begin(),
            [](const UserModel_shp a, const UserModel_shp b) { return *a == *b; }))
    {
        testPassed = true;
    }
    else
    {
        std::clog << "Get All users FAILED! " << allUsers.size() << "\n";
        if (userProfileTestData.size() != allUsers.size())
        {
            std::clog << std::format("Size differs: userProfileTestData.size({}) != llUsers.size({})",
                userProfileTestData.size(), allUsers.size());
        }
        else
        {
            for (std::size_t userLisetIdx = 0; userLisetIdx < userProfileTestData.size(); ++userLisetIdx)
            {
                if (*userProfileTestData[userLisetIdx] != *allUsers[userLisetIdx])
                {
                    std::clog << std::format("Original Data [{}]", userLisetIdx) << "\n" <<
                        *userProfileTestData[userLisetIdx] << std::format("Database Data [{}]", userLisetIdx) << 
                        "\n" << *allUsers[userLisetIdx] << "\n";
                }
            }
        }
    }

    allUsers.clear();

    return testPassed;
}

bool TestUserDBInterface::testMissingRequiredField(UserModel &userMissingField, std::vector<std::string>&  expectedErrors)
{
    if (insertionWasSuccessfull(userDBInterface.insert(userMissingField),
        "Inserted User missing required fields!"))
    {
        return false;
    }

    if (!hasErrorMessage())
    {
        return false;
    }

    for (auto expectedError: expectedErrors)
    {
        if (!wrongErrorMessage(expectedError))
        {
            return false;
        }
    }
    return true;
}

bool TestUserDBInterface::wrongErrorMessage(std::string expectedString)
{
    std::string errorMessage = userDBInterface.getAllErrorMessages();
    std::size_t found = errorMessage.find(expectedString);
    if (found == std::string::npos)
    {
        std::clog << "Wrong message generated! TEST FAILED!\n";
        std::clog << errorMessage << "\n";
        return false;
    }

    return true;
}

bool TestUserDBInterface::hasErrorMessage()
{
    std::string errorMessage = userDBInterface.getAllErrorMessages();
    if (errorMessage.empty())
    {
        std::clog << "No error message generated! TEST FAILED!\n";
        return false;
    }

    return true;
}

bool TestUserDBInterface::insertionWasSuccessfull(std::size_t userID, std::string logMessage)
{
    if (userID > 0)
    {
        std::clog << logMessage << " TEST FAILED\n";
        return true;
    }

    return false;
}

bool TestUserDBInterface::negativePathMissingRequiredFields()
{
    std::vector<std::string> expectedErrors =
    {
        "Last Name", "First Name", "Login Name", "Password", "User is missing required values"
    };

    UserModel newuser;
    newuser.setUserID(0);   // Force a modification so that missing fields can be tested.
    if (!testMissingRequiredField(newuser, expectedErrors))
    {
        return false;
    }

    newuser.setLastName("TestUser");
    expectedErrors.erase(expectedErrors.begin());
    if (!testMissingRequiredField(newuser, expectedErrors))
    {
        return false;
    }

    newuser.setFirstName("Alternate");
    expectedErrors.erase(expectedErrors.begin());
    if (!testMissingRequiredField(newuser, expectedErrors))
    {
        return false;
    }

    newuser.setMiddleInitial("One");
    newuser.setLoginName("TestUserAlternateO");
    expectedErrors.erase(expectedErrors.begin());
    if (!testMissingRequiredField(newuser, expectedErrors))
    {
        return false;
    }

    newuser.setPassword("TestUserAlternateO");
    newuser.setUserID(userDBInterface.insert(newuser));
    if (!newuser.isInDataBase())
    {
        std::cerr << userDBInterface.getAllErrorMessages() << newuser << "\n";
        std::clog << "Primary key for task: " << newuser.getUserID() << " not set!\n";
        if (verboseOutput)
        {
            std::clog << newuser << "\n\n";
        }
        return false;
    }

    return true;
}

void TestUserDBInterface::addFirstUser(UserList &TestUsers)
{
    // Test one case of the alternate constructor.
    UserModel_shp firstUser = std::make_shared<UserModel>("PacMan", "IN", "BW", "pacmaninbw@gmail.com");
    firstUser->autoGenerateLoginAndPassword();
    TestUsers.push_back(firstUser);
}
