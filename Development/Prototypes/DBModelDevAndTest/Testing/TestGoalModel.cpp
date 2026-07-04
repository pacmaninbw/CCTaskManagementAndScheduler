// Project Header Files
#include "commonUtilities.h"
#include "TestGoalModel.h"
#include "TestDBInterfaceCore.h"
#include "GoalQueryProcessor.h"
#include "UserGoalModel.h"
#include "UserModel.h"
#include "UserQueryProcessor.h"

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
        m_TestInput.push_back(testGoal);
    }

    m_PositiviePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testPositivePathGoalInsertions, this));
    m_PositiviePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testPositivePathGetListofChildrenFromParent, this));
    m_PositiviePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testPositivePathGetAllGoalsForUser, this));
    m_PositiviePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testPositivePathFindGoalsWithSimilarDescription, this));
    m_PositiviePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testPositivePathDeleteGoal, this));

    m_NegativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::negativePathMissingRequiredFields, this));
    m_NegativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testnegativePathNotModified, this));
    m_NegativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalModel::testNegativePathAlreadyInDataBase, this));

    UserQueryProcessor userQueryProcessor;
    m_UserOne = userQueryProcessor.getUserByFullName("One", "User", "P");
    if (!m_UserOne)
    {
        std::cerr << std::format("Failed to find userOne!:{}", userQueryProcessor.getAllErrorMessages());
    }
}

TestStatus TestGoalModel::testInsertAndGetParent(TestGoalInput testGoal)
{
    UserGoalModel newGoal;
    newGoal.setUserId(m_UserOne->getUserID());
    newGoal.setDescription(testGoal.description);
    newGoal.setPriority(testGoal.priority);
    newGoal.setCreationTimeStamp(commonTestTimeStampValue);
    if (!testGoal.parentDescription.empty())
    {
        GoalQueryProcessor goalQueryProcessor;
        UserGoalModel_shp parentGoal = goalQueryProcessor.findGoalByUserIdAndExactDescription(m_UserOne->getUserID(), testGoal.parentDescription);
        if (parentGoal == nullptr)
        {
            std::cerr << "Failed to find Parent Goal! Test FAILED\n";
            std::cerr << goalQueryProcessor.getAllErrorMessages() << "\n";
            return TESTFAILED;
        }
        newGoal.setParentID(parentGoal->getGoalId());
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

    for (auto testGoal: m_TestInput)
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
    GoalQueryProcessor goalQueryProcessor;
    UserGoalModel_shp parentGoal = goalQueryProcessor.findGoalByUserIdAndExactDescription(m_UserOne->getUserID(), "Get a Job in Software Engineering");
    if (parentGoal == nullptr)
    {
        std::cerr << "Failed to find Parent Goal! Test FAILED\n";
        std::cerr << goalQueryProcessor.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    GoalQueryProcessor goalListTestInterface;
    UserGoalList jobRelatedgoals = goalListTestInterface.getAllChildrenFromParent(*parentGoal);

    if (jobRelatedgoals.empty())
    {
        std::cerr << "test of goalListTestInterface.getAllChildrenFromParent() FAILED\n" <<
            goalListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << *parentGoal << "\n";
        std::cout << std::format("Find all child goals for({}) PASSED!\n", parentGoal->getGoalId());
        std::cout << std::format("Goal {} has {} child goals\n", parentGoal->getGoalId(), jobRelatedgoals.size());
        for (auto goal: jobRelatedgoals)
        {
            std::cout << *goal << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestGoalModel::testPositivePathGetAllGoalsForUser()
{
    GoalQueryProcessor goalListTestInterface;
    UserGoalList allUserGoals = goalListTestInterface.getAllGoalsForUser(m_UserOne->getUserID());

    if (allUserGoals.empty())
    {
        std::cerr << "test of goalListTestInterface.getAllGoalsForUser() FAILED\n" <<
            goalListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all goals for user ({}) PASSED!\n", m_UserOne->getUserID());
        std::cout << std::format("User {} has {} goals\n", m_UserOne->getUserID(), allUserGoals.size());
        for (auto goal: allUserGoals)
        {
            std::cout << *goal << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestGoalModel::testPositivePathFindGoalsWithSimilarDescription()
{
    GoalQueryProcessor goalListTestInterface;
    std::string searchString("Maintain");
    std::size_t userId = m_UserOne->getUserID();

    UserGoalList goalsWithSimilarDescription = goalListTestInterface.findGoalsByUserIdAndSimilarDescription(userId, searchString);
    if (goalsWithSimilarDescription.empty())
    {
        std::cerr << "test of goalListTestInterface.findGoalsByUserIdAndSimilarDescription() FAILED\n" <<
            goalListTestInterface.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("Find all goals for user ({}) containing {} PASSED!\n", userId, searchString);
        std::cout << std::format("User {} has {} goals\n", m_UserOne->getUserID(), goalsWithSimilarDescription.size());
        for (auto goal: goalsWithSimilarDescription)
        {
            std::cout << *goal << "\n";
        }
    }
    
    return TESTPASSED;
}

TestStatus TestGoalModel::testPositivePathDeleteGoal()
{
    std::string funcUnderTest("Delete Goal");

    std::chrono::year_month_day testDate(constantStringToChronoDate("2026-03-08"));
    GoalQueryProcessor goalListTestInterface;
    UserGoalList testGoalList = goalListTestInterface.getAllGoalsForUser(m_UserOne->getUserID());
    if (testGoalList.empty())
    {
        std::cerr << std::format("{}: {} {} FAILED\n", funcUnderTest, "userOne goal list is empty", goalListTestInterface.getAllErrorMessages());
        return TESTFAILED;
    }

    std::size_t itemToHideIndex = testGoalList.size() > 3? testGoalList.size() - 2 : testGoalList.size() - 1;
    UserGoalModel_shp goalToHide = testGoalList[itemToHideIndex];
    if (!goalToHide->hide(m_UserOne->getUserID()))
    {
        std::cerr << std::format("itemToHide->hide({}) FAILED!", m_UserOne->getUserID()) << goalToHide->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (!goalToHide->isDeleted())
    {
        std::cerr << std::format("{}: goal ({}) was not marked as deleted {} FAILED\n", funcUnderTest,
            goalToHide->getGoalId(), goalToHide->getAllErrorMessages());
        return TESTFAILED;
    }

    UserGoalList alteredList = goalListTestInterface.getAllGoalsForUser(m_UserOne->getUserID());
    if (!(alteredList.size() < testGoalList.size()))
    {
        std::cerr << std::format("Deleted goal ({}) did not decrease the size of the user goal list. TEST FAILED\n",
            goalToHide->getGoalId());
        return TESTFAILED;
    }

    for (auto itemInList: alteredList)
    {
        if (itemInList->getGoalId() == goalToHide->getGoalId())
        {
            std::cerr << "The wrong goal was deleted. TEST FAILED\n";
            return TESTFAILED;
        }
    }

    if (programOptions.verboseOutput)
    {
        std::cout << "Original goal list size: " << testGoalList.size() << " Altered goal list size: " << alteredList.size() << "\n";
        std::cout << std::format("goal ({}) for user ({}) marked Deleted. TEST PASSED\n",
            goalToHide->getGoalId(), m_UserOne->getUserID());
    }

    return TESTPASSED;
}

TestStatus TestGoalModel::testNegativePathAlreadyInDataBase()
{
    GoalQueryProcessor goalQueryProcessor;
    UserGoalModel_shp goalAlreadyInDB = goalQueryProcessor.getGoalById(1);
    if (goalAlreadyInDB == nullptr || !goalAlreadyInDB->isInDataBase())
    {
        std::cout << "Goal 1 not found in database!!\n";
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(goalAlreadyInDB, expectedErrors);
}

TestStatus TestGoalModel::testnegativePathNotModified()
{
    GoalQueryProcessor goalQueryProcessor;
    UserGoalModel_shp goalNotModified = goalQueryProcessor.getGoalById(1);
    if (goalNotModified == nullptr || !goalNotModified->isInDataBase())
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
        if (m_verboseOutput)
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

