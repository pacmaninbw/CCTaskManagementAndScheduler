#include "CommandLineParser.h"
#include "commonUtilities.h"
#include "CSVReader.h"
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include "TestUserDBInterface.h"
#include "UserList.h"
#include "UserModel.h"
#include <vector>

TestUserDBInterface::TestUserDBInterface(std::string userFileName)
: TestDBInterfaceCore(programOptions.verboseOutput, "user")
{
    dataFileName = userFileName;
    
    positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByLoginName, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByLoginAndPassword, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testGetUserByFullName, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestUserDBInterface::testUpdateUserPassword, this, std::placeholders::_1));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestUserDBInterface::negativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestUserDBInterface::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestUserDBInterface::testNegativePathAlreadyInDataBase, this));
}

TestDBInterfaceCore::TestStatus TestUserDBInterface::runPositivePathTests()
{
    UserListValues userProfileTestData;
    addFirstUser(userProfileTestData);

    if (!loadTestUsersFromFile(userProfileTestData))
    {
        return TESTFAILED;
    }


    bool allTestsPassed = true;

    for (auto user: userProfileTestData)
    {
        user->insert();
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
            std::clog << user->getAllErrorMessages() << "\n";
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

    reportTestStatus(allTestsPassed? TESTPASSED : TESTFAILED, "positive");
    return allTestsPassed? TESTPASSED : TESTFAILED;
}

bool TestUserDBInterface::testGetUserByLoginName(UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = std::make_shared<UserModel>();
    if (retrievedUser->selectByLoginName(insertedUser->getLoginName()))
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
            retrievedUser->getAllErrorMessages() << "\n";
        return false;
    }
}

bool TestUserDBInterface::testGetUserByLoginAndPassword(UserModel_shp insertedUser)
{
    std::string_view testName = insertedUser->getLoginName();
    std::string_view testPassword = insertedUser->getPassword();

    UserModel_shp retrievedUser = std::make_shared<UserModel>();
    if (retrievedUser->selectByLoginAndPassword(testName, testPassword))
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
            retrievedUser->getAllErrorMessages() << "\n";
        return false;
    }


    if (retrievedUser->selectByLoginAndPassword(testName, "NotThePassword"))
    {
        std::cerr << "retrievedUser->selectByLoginAndPassword(user->getLoginName()) Found user with fake password!\n";
        return false;
    }

    return true;
}

bool TestUserDBInterface::testGetUserByFullName(UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = std::make_shared<UserModel>();
    if (retrievedUser->selectByFullName(insertedUser->getLastName(), insertedUser->getFirstName(),
        insertedUser->getMiddleInitial()))
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
            retrievedUser->getAllErrorMessages() << "\n";
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

    UserModel_shp newUserValues = std::make_shared<UserModel>();
    newUserValues->setUserID(insertedUser->getUserID());
    newUserValues->retrieve();
    if (oldUserValues == *newUserValues)
    {
        std::clog << std::format("Password update for user {} FAILED!\n", oldUserValues.getUserID());
        testPassed = false;
    }

    return testPassed;
}

bool TestUserDBInterface::loadTestUsersFromFile(UserListValues& userProfileTestData)
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
        userIn->setCreationDate(getTodaysDate());
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

bool TestUserDBInterface::testGetAllUsers(UserListValues userProfileTestData)
{
    bool testPassed = false;
    UserList testULists;
    UserListValues allUsers = testULists.getAllUsers();

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

TestDBInterfaceCore::TestStatus TestUserDBInterface::negativePathMissingRequiredFields()
{
    std::vector<std::string> expectedErrors =
    {
        "Last Name", "First Name", "Login Name", "Password", "Date Added", "User is missing required values"
    };

    UserModel newuser;
    newuser.setUserID(0);   // Force a modification so that missing fields can be tested.

    std::vector<std::function<void(std::string)>> fieldSettings = 
    {
        std::bind(&UserModel::setLastName, &newuser, std::placeholders::_1),
        std::bind(&UserModel::setFirstName, &newuser, std::placeholders::_1),
        std::bind(&UserModel::setLoginName, &newuser, std::placeholders::_1),
        std::bind(&UserModel::setPassword, &newuser, std::placeholders::_1)
    };

    for (auto setField: fieldSettings)
    {
        if (testInsertionFailureMessages(&newuser, expectedErrors) != TESTPASSED)
        {
            return TESTFAILED;
        }
        expectedErrors.erase(expectedErrors.begin());
        setField("teststringvalue");
    }

    expectedErrors.clear();

    newuser.setCreationDate(getTodaysDate());

    newuser.save();
    if (!newuser.isInDataBase())
    {
        std::cerr << newuser.getAllErrorMessages() << newuser << "\n";
        std::clog << "Primary key for user: " << newuser.getUserID() << " not set!\n";
        if (verboseOutput)
        {
            std::clog << newuser << "\n\n";
        }
        return TESTFAILED;
    }

    return TESTPASSED;
}

void TestUserDBInterface::addFirstUser(UserListValues& TestUsers)
{
    // Test one case of the alternate constructor.
    UserModel_shp firstUser = std::make_shared<UserModel>("PacMan", "IN", "BW", "pacmaninbw@gmail.com");
    firstUser->autoGenerateLoginAndPassword();
    TestUsers.push_back(firstUser);
}

TestDBInterfaceCore::TestStatus TestUserDBInterface::testnegativePathNotModified()
{
    UserModel_shp userNotModified = std::make_shared<UserModel>();
    userNotModified->setUserID(1);
    if (!userNotModified->retrieve())
    {
        std::cerr << "User 1 not found in database!!\n" << userNotModified->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    userNotModified->setUserID(0); // Force it to check modified rather than Already in DB.
    userNotModified->clearModified();
    std::vector<std::string> expectedErrors = {"not modified!"};
    return testInsertionFailureMessages(userNotModified, expectedErrors);
}

TestDBInterfaceCore::TestStatus TestUserDBInterface::testNegativePathAlreadyInDataBase()
{
    UserModel_shp userAlreadyInDB = std::make_shared<UserModel>();
    userAlreadyInDB->setUserID(1);
    if (!userAlreadyInDB->retrieve())
    {
        std::cerr << "User 1 not found in database!!\n" << userAlreadyInDB->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(userAlreadyInDB, expectedErrors);
}
