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
}

bool TestUserDBInterface::runAllTests()
{
    if (negativePathTests())
    {
        return positivePathTests();
    }
    
    return false;
}

bool TestUserDBInterface::negativePathTests()
{
    std::cerr << "TestUserDBInterface::negativePathTests() NOT IMPLEMENTED!!\n";

    return true;
}

bool TestUserDBInterface::positivePathTests()
{
    // Test one case of the alternate constructor.
    UserList userProfileTestData = {{std::make_shared<UserModel>("PacMan", "IN", "BW", "pacmaninbw@gmail.com")}};
    userProfileTestData[0]->autoGenerateLoginAndPassword();

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
