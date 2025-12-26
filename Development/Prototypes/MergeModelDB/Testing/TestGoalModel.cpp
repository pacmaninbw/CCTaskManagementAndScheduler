// Project Header Files
#include "TestGoalModel.h"
#include "TestDBInterfaceCore.h"
#include "UserGoalList.h"
#include "UserGoalModel.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <string>
#include <vector>

TestGoalModel::TestGoalModel()
: TestDBInterfaceCore("Goal")
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
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testPositivePathGetListofChildrenFromParent, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testPositivePathGetAllGoalsForUser, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testPositivePathFindGoalsWithSimilarDescription, this));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::negativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testNegativePathAlreadyInDataBase, this));
}

TestStatus TestGoalModel::testInsertAndGetParent(TestGoalInput testGoal)
{
    UserModel_shp userOne = std::make_shared<UserModel>();
    userOne->selectByUserID(1);
    UserGoalModel newGoal;
    newGoal.setUserId(userOne->getUserID());
    newGoal.setDescription(testGoal.description);
    newGoal.setPriority(testGoal.priority);
    newGoal.setCreationTimeStamp(commonTestTimeStampValue);
    if (!testGoal.parentDescription.empty())
    {
        UserGoalModel parentGoal;
        if (!parentGoal.selectByUserIDAndDescription(userOne->getUserID(), testGoal.parentDescription))
        {
            std::cerr << "Failed to find Parent Goal! Test FAILED\n";
            std::cerr << parentGoal.getAllErrorMessages() << "\n";
            return TESTFAILED;
        }
        newGoal.setParentID(parentGoal.getGoalId());
    }

    if (!newGoal.insert())
    {
        std::cout << "Insertion failed for Goal: " << newGoal << " :\n";
        std::cout << newGoal.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus TestGoalModel::testPositivePathGoalInsertions()
{
    TestStatus testStatus = TESTPASSED;

    for (auto testGoal: testInput)
    {
        TestStatus currentResult = testInsertAndGetParent(testGoal);
        if (testStatus == TESTPASSED)
        {
            testStatus = currentResult;
        }
    }

    if (testStatus == TESTFAILED)
    {
        std::cerr << "Some or all Goal Insertions FAILED\n";
    }

    return testStatus;
}

TestStatus TestGoalModel::testPositivePathGetListofChildrenFromParent()
{
    UserGoalModel parentGoal;
    if (!parentGoal.selectByUserIDAndDescription(1, "Get a Job in Software Engineering"))
    {
        std::cerr << "Failed to find Parent Goal! Test FAILED\n";
        std::cerr << parentGoal.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    UserGoalList goalListTestInterface;
    UserGoalListValues jobRelatedgoals = goalListTestInterface.getAllChildrenFromParent(parentGoal);

    if (jobRelatedgoals.empty())
    {
        std::cerr << "test of goalListTestInterface.getAllChildrenFromParent() FAILED\n" <<
            goalListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << parentGoal << "\n";
        std::cout << std::format("Find all child goals for({}) PASSED!\n", parentGoal.getGoalId());
        std::cout << std::format("Goal {} has {} child goals\n", parentGoal.getGoalId(), jobRelatedgoals.size());
        for (auto goal: jobRelatedgoals)
        {
            std::cout << *goal << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestGoalModel::testPositivePathGetAllGoalsForUser()
{
    UserGoalList goalListTestInterface;
    UserGoalListValues allUserGoals = goalListTestInterface.getAllGoalsForUser(1);

    if (allUserGoals.empty())
    {
        std::cerr << "test of goalListTestInterface.getAllGoalsForUser() FAILED\n" <<
            goalListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all goals for user ({}) PASSED!\n", 1);
        std::cout << std::format("User {} has {} goals\n", 1, allUserGoals.size());
        for (auto goal: allUserGoals)
        {
            std::cout << *goal << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestGoalModel::testPositivePathFindGoalsWithSimilarDescription()
{
    UserGoalList goalListTestInterface;
    std::string searchString("Maintain");
    std::size_t userId = 1;

    UserGoalListValues goalsWithSimilarDescription = goalListTestInterface.findGoalsByUserIdAndSimilarDescription(userId, searchString);
    if (goalsWithSimilarDescription.empty())
    {
        std::cerr << "test of goalListTestInterface.findGoalsByUserIdAndSimilarDescription() FAILED\n" <<
            goalListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all goals for user ({}) containing {} PASSED!\n", userId, searchString);
        std::cout << std::format("User {} has {} goals\n", 1, goalsWithSimilarDescription.size());
        for (auto goal: goalsWithSimilarDescription)
        {
            std::cout << *goal << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestGoalModel::testNegativePathAlreadyInDataBase()
{
    UserGoalModel_shp goalAlreadyInDB = std::make_shared<UserGoalModel>();
    goalAlreadyInDB->setGoalId(1);
    if (!goalAlreadyInDB->retrieve())
    {
        std::cout << "Goal 1 not found in database!!\n";
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(goalAlreadyInDB, expectedErrors);
}

TestStatus TestGoalModel::testnegativePathNotModified()
{
    UserGoalModel_shp goalNotModified = std::make_shared<UserGoalModel>();
    goalNotModified->setGoalId(1);
    if (!goalNotModified->retrieve())
    {
        std::cout << "Goal 1 not found in database!!\n";
        return TESTFAILED;
    }

    goalNotModified->setGoalId(0); // Force it to check modified rather than Already in DB.
    goalNotModified->clearModified();
    std::vector<std::string> expectedErrors = {"not modified!"};
    return testInsertionFailureMessages(goalNotModified, expectedErrors);
}

TestStatus TestGoalModel::negativePathMissingRequiredFields()
{
    TestStatus testStatus = TESTPASSED;

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Description", "missing required values"
    };

    UserGoalModel testGoal;
    testGoal.setGoalId(0);  // prevent the not modified error.

    std::vector<std::function<TestStatus(UserGoalModel&, std::vector<std::string>&)>> fieldTestUpdate =
    {
        {std::bind(&TestGoalModel::testMissingRequiredFieldsAddUserID, this, std::placeholders::_1, std::placeholders::_2)},
        {std::bind(&TestGoalModel::testMissingRequiredFieldsAddDescription, this, std::placeholders::_1, std::placeholders::_2)},
    };

    for (auto fieldTest: fieldTestUpdate)
    {
        TestStatus thisTestStatus = fieldTest(testGoal, expectedErrors);
        if (testStatus == TESTPASSED)
        {
            testStatus = thisTestStatus;
        }
    }

    testGoal.setCreationTimeStamp(commonTestTimeStampValue);
    testGoal.save();
    if (!testGoal.isInDataBase())
    {
        std::cout << testGoal.getAllErrorMessages() << testGoal << "\n";
        std::cout << "Primary key for user: " << testGoal.getGoalId() << " not set!\n";
        if (verboseOutput)
        {
            std::cout << testGoal << "\n\n";
        }
        testStatus = TESTFAILED;
    }

    return testStatus;
}

TestStatus TestGoalModel::testMissingRequiredFieldsAddUserID(
    UserGoalModel &testGoal, std::vector<std::string>& expectedErrors)
{
    TestStatus testStatus =
        testInsertionFailureMessages(&testGoal, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testGoal.setUserId(1);

    return testStatus;
}

TestStatus TestGoalModel::testMissingRequiredFieldsAddDescription(
    UserGoalModel &testGoal, std::vector<std::string>& expectedErrors)
{
    TestStatus testStatus =
        testInsertionFailureMessages(&testGoal, expectedErrors);

    expectedErrors.erase(expectedErrors.begin());
    testGoal.setDescription("Test missing field Description");

    return testStatus;
}

