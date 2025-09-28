#include <chrono>
#include <functional>
#include <string>
#include "TestGoalModel.h"
#include "TestDBInterfaceCore.h"
#include "UserGoalList.h"
#include "UserGoalModel.h"
#include "UserModel.h"
#include <vector>

TestGoalModel::TestGoalModel()
: TestDBInterfaceCore(programOptions.verboseOutput, "Goal")
{
    std::vector<TestGoalInput> testGoalInput =
    {
        {"Get a Job in Software Engineering", 1, ""},
        {"Maintain and improve my software engineering skills", 1, "Get a Job in Software Engineering"},
        {"Maintain and improve my C++ programming skills", 1, "Get a Job in Software Engineering"},
        {"Maintain and improve my  C programming skills", 2, "Get a Job in Software Engineering"},
        {"Maintain or archive the BHHS 74 Reunion website", 2, ""},
        {"Maintain and improve my SQL skills", 3, "Get a Job in Software Engineering"},
        {"Learn QT graphic system", 4, "Get a Job in Software Engineering"},
        {"Improve my Linux Programming skills", 5, "Get a Job in Software Engineering"},
        {"Get an embedded programming certificate", 4, "Get a Job in Software Engineering"},
        {"Learn open source Linux Core Programming", 4, "Get a Job in Software Engineering"},
        {"Maintain my health", 2, ""},
        {"Eat as healthy as possible", 3, "Maintain my health"},
        {"Walk 30 minutes a day", 4, "Maintain my health"},
        {"Track my vital signs", 3, "Maintain my health"},
        {"Get my weight down to 190 pounds", 4, "Maintain my health"},
        {"Be more organized", 1, ""},
        {"Effectively moderate the code review site on stack exchange", 1, ""}
    };

    for (auto testGoal: testGoalInput)
    {
        testInput.push_back(testGoal);
    }

    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testPositivePathGoalInsertions, this));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::negativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testNegativePathAlreadyInDataBase, this));
}

TestDBInterfaceCore::TestStatus TestGoalModel::testInsertAndGetParent(TestGoalInput testGoal)
{
    UserModel_shp userOne = std::make_shared<UserModel>();
    userOne->selectByUserID(1);
    UserGoalModel newGoal;
    newGoal.setUserId(userOne->getUserID());
    newGoal.setDescription(testGoal.description);
    newGoal.setPriority(testGoal.priority);
    newGoal.setCreationTimeStamp(std::chrono::system_clock::now());
    newGoal.setLastUpdateTimeStamp(std::chrono::system_clock::now());
    if (!testGoal.parentDescription.empty())
    {
        UserGoalModel parentGoal;
        if (!parentGoal.selectByUserIDAndDescription(userOne->getUserID(), testGoal.parentDescription))
        {
            std::clog << "Failed to find Parent Goal! Test FAILED\n";
            std::clog << parentGoal.getAllErrorMessages() << "\n";
            return TESTFAILED;
        }
        newGoal.setParentID(parentGoal.getGoalId());
    }

    if (!newGoal.insert())
    {
        std::clog << "Insertion failed for Goal: " << newGoal << " :\n";
        std::clog << newGoal.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestDBInterfaceCore::TestStatus TestGoalModel::testPositivePathGoalInsertions()
{
    TestDBInterfaceCore::TestStatus testStatus = TESTPASSED;

    for (auto testGoal: testInput)
    {
        TestDBInterfaceCore::TestStatus currentResult = testInsertAndGetParent(testGoal);
        if (testStatus == TESTPASSED)
        {
            testStatus = currentResult;
        }
    }

    if (testStatus == TESTFAILED)
    {
        std::clog << "Some or all Goal Insertions FAILED\n";
    }

    return testStatus;
}

TestDBInterfaceCore::TestStatus TestGoalModel::testNegativePathAlreadyInDataBase()
{
    UserGoalModel_shp goalAlreadyInDB = std::make_shared<UserGoalModel>();
    goalAlreadyInDB->setGoalId(1);
    if (!goalAlreadyInDB->retrieve())
    {
        std::cerr << "Goal 1 not found in database!!\n";
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(goalAlreadyInDB, expectedErrors);
}

TestDBInterfaceCore::TestStatus TestGoalModel::testnegativePathNotModified()
{
    UserGoalModel_shp goalNotModified = std::make_shared<UserGoalModel>();
    goalNotModified->setGoalId(1);
    if (!goalNotModified->retrieve())
    {
        std::cerr << "Goal 1 not found in database!!\n";
        return TESTFAILED;
    }

    goalNotModified->setGoalId(0); // Force it to check modified rather than Already in DB.
    goalNotModified->clearModified();
    std::vector<std::string> expectedErrors = {"not modified!"};
    return testInsertionFailureMessages(goalNotModified, expectedErrors);
}

TestDBInterfaceCore::TestStatus TestGoalModel::negativePathMissingRequiredFields()
{
    TestDBInterfaceCore::TestStatus testStatus = TESTPASSED;

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Description", "missing required values"
//        "User ID", "Description", "Creation Timestamp", "Last Update", "missing required values"
    };

    UserGoalModel testGoal;
    testGoal.setGoalId(0);

    std::vector<std::function<TestDBInterfaceCore::TestStatus(UserGoalModel&, std::vector<std::string>&)>> fieldTestUpdate =
    {
        {std::bind(&TestGoalModel::testMissingRequiredFieldsAddUserID, this, std::placeholders::_1, std::placeholders::_2)},
        {std::bind(&TestGoalModel::testMissingRequiredFieldsAddDescription, this, std::placeholders::_1, std::placeholders::_2)},
//        {std::bind(&TestGoalModel::testMissingRequiredFieldsAddCreationTS, this, std::placeholders::_1, std::placeholders::_2)},
//        {std::bind(&TestGoalModel::testMissingRequiredFieldsAddLastUpdateTS, this, std::placeholders::_1, std::placeholders::_2)},
    };

    for (auto fieldTest: fieldTestUpdate)
    {
        TestDBInterfaceCore::TestStatus thisTestStatus = fieldTest(testGoal, expectedErrors);
        if (testStatus == TESTPASSED)
        {
            testStatus = thisTestStatus;
        }
    }

    testGoal.save();
    if (!testGoal.isInDataBase())
    {
        std::cerr << testGoal.getAllErrorMessages() << testGoal << "\n";
        std::clog << "Primary key for user: " << testGoal.getGoalId() << " not set!\n";
        if (verboseOutput)
        {
            std::clog << testGoal << "\n\n";
        }
        testStatus = TESTFAILED;
    }

    return testStatus;
}

TestDBInterfaceCore::TestStatus TestGoalModel::testMissingRequiredFieldsAddUserID(
    UserGoalModel &testGoal, std::vector<std::string>& expectedErrors)
{
    TestDBInterfaceCore::TestStatus testStatus =
        testInsertionFailureMessages(&testGoal, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testGoal.setUserId(1);

    return testStatus;
}

TestDBInterfaceCore::TestStatus TestGoalModel::testMissingRequiredFieldsAddDescription(
    UserGoalModel &testGoal, std::vector<std::string>& expectedErrors)
{
    TestDBInterfaceCore::TestStatus testStatus =
        testInsertionFailureMessages(&testGoal, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testGoal.setDescription("Test missing field Description");

    return testStatus;
}

TestDBInterfaceCore::TestStatus TestGoalModel::testMissingRequiredFieldsAddCreationTS(
    UserGoalModel &testGoal, std::vector<std::string>& expectedErrors)
{
    TestDBInterfaceCore::TestStatus testStatus =
        testInsertionFailureMessages(&testGoal, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testGoal.setCreationTimeStamp(std::chrono::system_clock::now());

    return testStatus;
}

TestDBInterfaceCore::TestStatus TestGoalModel::testMissingRequiredFieldsAddLastUpdateTS(
    UserGoalModel &testGoal, std::vector<std::string>& expectedErrors)
{
    TestDBInterfaceCore::TestStatus testStatus =
        testInsertionFailureMessages(&testGoal, expectedErrors);
        
    expectedErrors.erase(expectedErrors.begin());
    testGoal.setLastUpdateTimeStamp(std::chrono::system_clock::now());

    return testStatus;
}
