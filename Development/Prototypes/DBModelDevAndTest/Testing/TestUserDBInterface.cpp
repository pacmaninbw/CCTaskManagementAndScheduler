// Project Header Files
#include "CommandLineParser.h"
#include "commonTestValues.h"
#include "commonUtilities.h"
#include "CSVReader.h"
#include "TestUserDBInterface.h"
#include "UserQueryProcessor.h"
#include "UserModel.h"

// C++ Header Files
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

TestUserDBInterface::TestUserDBInterface(std::string userFileName)
: TestDBInterfaceCore("user")
{
    m_dataFileName = userFileName;
    
    m_positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByLoginName, this, std::placeholders::_1));
    m_positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByLoginAndPassword, this, std::placeholders::_1));
    m_positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByFullName, this, std::placeholders::_1));
    m_positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByEmail, this, std::placeholders::_1));
    m_positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testUpdateUserPassword, this, std::placeholders::_1));

    m_negativePathTestFuncsNoArgs.push_back(std::bind(&TestUserDBInterface::testnegativePathNotModified, this));
    m_negativePathTestFuncsNoArgs.push_back(std::bind(&TestUserDBInterface::testNegativePathAlreadyInDataBase, this));
}

TestStatus TestUserDBInterface::runPositivePathTests()
{
    UserQueryProcessor databaseEntries;
    UserModelList userProfileTestData = databaseEntries.getAllUsers();

    if (!loadTestUsersFromFile(userProfileTestData))
    {
        return TESTFAILED;
    }


    bool allTestsPassed = true;

    for (auto user: userProfileTestData)
    {
        // Exclude real data from production testing in user integration testing.
        // The real data is know to work and inserting it again causes errors.
        if (user->isInDataBase())
        {
            if (user->insert())
            {
                std::cerr << "Inserted user already in database.\n" << *user << "\n";
            }
        }
        else
        {
            user->insert();
            // if adding the user was successful, perform other tests with the data.
            if (user->isInDataBase())
            {
                for (auto test: m_positiveTestFuncs)
                {
                    if (!test(user))
                    {
                        allTestsPassed = false;
                    }
                }
            }
            else
            {
                std::cerr << "Primary key for user: " << user->getLastName() << ", " << user->getFirstName() << " not set!\n";
                std::cerr << user->getAllErrorMessages() << "\n";
                if (m_verboseOutput)
                {
                    std::cout << *user << "\n\n";
                }
                allTestsPassed = false;
            }
        }
    }

    if (allTestsPassed)
    {
        allTestsPassed = testGetAllUsers(userProfileTestData);
    }

    userProfileTestData.clear();

    reportTestStatus(allTestsPassed? TESTPASSED : TESTFAILED, "positive");
    return allTestsPassed? TESTPASSED : TESTFAILED;
}

bool TestUserDBInterface::testGetUserByLoginName(UserModel_shp insertedUser)
{
    UserQueryProcessor userQueryProcessor;
    UserModel_shp retrievedUser = userQueryProcessor.getUserByLoginName(insertedUser->getLoginName());
    if (retrievedUser && retrievedUser->isInDataBase())
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
        std::cerr << "testGetUserByLoginNamen(user->getLoginName()) FAILED!\n" <<
            userQueryProcessor.getAllErrorMessages() << "\n";
        return false;
    }
}

bool TestUserDBInterface::testGetUserByLoginAndPassword(UserModel_shp insertedUser)
{
    std::string testName = insertedUser->getLoginName();
    std::string testPassword = insertedUser->getPassword();

    UserQueryProcessor userQueryProcessor;
    UserModel_shp retrievedUser = userQueryProcessor.getUserByLoginAndPassword(testName, testPassword);
    if (retrievedUser && retrievedUser->isInDataBase())
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
        std::cerr << "testGetUserByLoginAndPassword(user->getLoginName()) FAILED!\n" <<
            userQueryProcessor.getAllErrorMessages() << "\n";
        std::cerr << "Expected user name " << testName << "\n";
        std::cerr << "Expected user password " << testPassword << "\n";
        return false;
    }

    retrievedUser = userQueryProcessor.getUserByLoginAndPassword(testName, "NotThePassword");
    if (retrievedUser)
    {
        std::cout << "retrievedUser->selectByLoginAndPassword(user->getLoginName()) Found user with fake password!\n";
        return false;
    }

    return true;
}

bool TestUserDBInterface::testGetUserByFullName(UserModel_shp insertedUser)
{
    UserQueryProcessor userQueryProcessor;
    UserModel_shp retrievedUser = userQueryProcessor.getUserByFullName(insertedUser->getLastName(), insertedUser->getFirstName(),
        insertedUser->getMiddleInitial());
    if (retrievedUser && retrievedUser->isInDataBase())
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
        std::cerr << "retrievedUser->selectByFullName FAILED!\n" <<
            userQueryProcessor.getAllErrorMessages() << "\n";
        return false;
    }
}

bool TestUserDBInterface::testGetUserByEmail(UserModel_shp insertedUser)
{
    UserQueryProcessor userQueryProcessor;
    UserModel_shp retrievedUser = userQueryProcessor.getUserByEmail(insertedUser->getEmail());
    if (retrievedUser && retrievedUser->isInDataBase())
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
        std::cerr << "retrievedUser->getUserByEmail FAILED!\n" <<
            userQueryProcessor.getAllErrorMessages() << "\n";
        return false;
    }
}


bool TestUserDBInterface::testUpdateUserPassword(UserModel_shp insertedUser)
{
    bool testPassed = true;
    UserModel oldUserValues = *insertedUser;
    std::string newPassword = "MyNew**&pAs5Word" + std::to_string(oldUserValues.getUserID());

    insertedUser->setPassword(newPassword);
    if (!insertedUser->save())
    {
        std::cerr << "insertedUser->save()() FAILED" << insertedUser->getAllErrorMessages() << "\n";
        return false;
    }

    UserQueryProcessor userQueryProcessor;
    UserModel_shp newUserValues = userQueryProcessor.getUserByID(insertedUser->getUserID());
    if (!newUserValues)
    {
        std::cerr << "UserQueryProcessor.getUserByID() FAILED!\n";
        std::cerr << userQueryProcessor.getAllErrorMessages() << std::endl;
        return false;
    }

    if (oldUserValues == *newUserValues)
    {
        std::cerr << std::format("Password update for user {} FAILED!\n", oldUserValues.getUserID());
        testPassed = false;
    }

    return testPassed;
}

bool TestUserDBInterface::loadTestUsersFromFile(UserModelList& userProfileTestData)
{
    std::ifstream userData(m_dataFileName);

    if (!userData.is_open())
    {
        std::cout << "Can't open \"" << m_dataFileName << "\" for input!" << std::endl;
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
        userIn->setCreationDate(commonTestTimeStampValue);
        userProfileTestData.push_back(userIn);
    }

    if (userData.bad())
    {
        std::cout << "Fatal error with file stream: \"" << m_dataFileName << "\"" << std::endl;
        return false;
    }

    return true;
}

bool TestUserDBInterface::testGetAllUsers(UserModelList userProfileTestData)
{
    bool testPassed = false;
    UserQueryProcessor testULists;
    UserModelList allUsers = testULists.getAllUsers();

    if ((userProfileTestData.size() == allUsers.size()) &&
        std::equal(userProfileTestData.begin(), userProfileTestData.end(), allUsers.begin(),
            [](const UserModel_shp a, const UserModel_shp b) { return *a == *b; }))
    {
        testPassed = true;
    }
    else
    {
        std::cerr << "Get All users FAILED! " << allUsers.size() << "\n";
        if (userProfileTestData.size() != allUsers.size())
        {
            std::cout << std::format("Size differs: userProfileTestData.size({}) != llUsers.size({})",
                userProfileTestData.size(), allUsers.size());
        }
        else
        {
            for (std::size_t userLisetIdx = 0; userLisetIdx < userProfileTestData.size(); ++userLisetIdx)
            {
                if (*userProfileTestData[userLisetIdx] != *allUsers[userLisetIdx])
                {
                    std::cout << std::format("Original Data [{}]", userLisetIdx) << "\n" <<
                        *userProfileTestData[userLisetIdx] << std::format("Database Data [{}]", userLisetIdx) << 
                        "\n" << *allUsers[userLisetIdx] << "\n";
                }
            }
        }
    }

    allUsers.clear();

    return testPassed;
}

TestStatus TestUserDBInterface::testnegativePathNotModified()
{
    UserQueryProcessor userQueryProcessor;
    UserModel_shp userNotModified = userQueryProcessor.getUserByID(1);
    if (userNotModified == nullptr || !userNotModified->isInDataBase())
    {
        std::cout << "User 1 not found in database!!\n" << userNotModified->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    userNotModified->setUserID(0); // Force it to check modified rather than Already in DB.
    userNotModified->clearModified();
    std::vector<std::string> expectedErrors = {"not modified!"};
    return testInsertionFailureMessages(userNotModified, expectedErrors);
}

TestStatus TestUserDBInterface::testNegativePathAlreadyInDataBase()
{
    UserQueryProcessor userQueryProcessor;
    UserModel_shp userAlreadyInDB = userQueryProcessor.getUserByID(1);
    if (userAlreadyInDB == nullptr || !userAlreadyInDB->isInDataBase())
    {
        std::cout << "User 1 not found in database!!\n" << userAlreadyInDB->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(userAlreadyInDB, expectedErrors);
}

