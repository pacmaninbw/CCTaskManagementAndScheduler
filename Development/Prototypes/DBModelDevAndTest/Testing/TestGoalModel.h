#ifndef TESTGOALMODEL_H_
#define TESTGOALMODEL_H_

class UserModel;
// Project Header Files
#include "TestDBInterfaceCore.h"
#include "GoalQueryProcessor.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <functional>
#include <memory>
#include <string>
#include <vector>

class TestGoalModel : public TestDBInterfaceCore
{
public:
    struct TestGoalInput
    {
        std::string description;
        unsigned int priority;
        std::string parentDescription;
    };

    TestGoalModel();
    ~TestGoalModel() = default;

private:
    TestStatus testInsertAndGetParent(TestGoalInput testGoal);
    TestStatus testPositivePathGoalInsertions();
    TestStatus testPositivePathGetListofChildrenFromParent();
    TestStatus testPositivePathGetAllGoalsForUser();
    TestStatus testPositivePathFindGoalsWithSimilarDescription();
    TestStatus testPositivePathDeleteGoal();
    TestStatus testNegativePathAlreadyInDataBase();
    TestStatus testnegativePathNotModified();
    TestStatus negativePathMissingRequiredFields();
    TestStatus testMissingRequiredFieldsAddUserID(
        UserGoalModel& testGoal, std::vector<std::string>& expectedErrors);
    TestStatus testMissingRequiredFieldsAddDescription(
        UserGoalModel& testGoal, std::vector<std::string>& expectedErrors);

    std::vector<TestGoalInput> m_testInput;
    std::shared_ptr<UserModel> m_userOne;
};

#endif // TESTGOALMODEL_H_

