#ifndef TESTGOALMODEL_H_
#define TESTGOALMODEL_H_

class UserModel;
// Project Header Files
#include "TestDBInterfaceCore.h"
#include "UserGoalList.h"
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

    bool verboseOutput;
    std::vector<std::function<bool(UserGoalModel_shp)>>positiveTestFuncs;
    std::vector<std::function<bool(void)>> negativePathTestFuncs;
    std::vector<TestGoalInput> testInput;
    std::shared_ptr<UserModel> userOne;
};

#endif // TESTGOALMODEL_H_

