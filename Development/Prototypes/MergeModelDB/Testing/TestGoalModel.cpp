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
#if 0
    positiveTestFuncs.push_back(std::bind(&TestGoalDBInterface::testGetUserByLoginAndPassword, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestGoalDBInterface::testGetUserByFullName, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestGoalDBInterface::testUpdateUserPassword, this, std::placeholders::_1));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalDBInterface::negativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalDBInterface::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalDBInterface::testNegativePathAlreadyInDataBase, this));
#endif
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
