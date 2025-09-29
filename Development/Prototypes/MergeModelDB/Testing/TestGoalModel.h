#ifndef TESTGOALMODEL_H_
#define TESTGOALMODEL_H_

#include <functional>
#include <string>
#include "TestDBInterfaceCore.h"
#include "UserGoalList.h"
#include "UserGoalModel.h"
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
    TestDBInterfaceCore::TestStatus testInsertAndGetParent(TestGoalInput testGoal);
    TestDBInterfaceCore::TestStatus testPositivePathGoalInsertions();
    TestDBInterfaceCore::TestStatus testNegativePathAlreadyInDataBase();
    TestDBInterfaceCore::TestStatus testnegativePathNotModified();
    TestDBInterfaceCore::TestStatus negativePathMissingRequiredFields();
    TestDBInterfaceCore::TestStatus testMissingRequiredFieldsAddUserID(
        UserGoalModel& testGoal, std::vector<std::string>& expectedErrors);
    TestDBInterfaceCore::TestStatus testMissingRequiredFieldsAddDescription(
        UserGoalModel& testGoal, std::vector<std::string>& expectedErrors);

    bool verboseOutput;
    std::vector<std::function<bool(UserGoalModel_shp)>>positiveTestFuncs;
    std::vector<std::function<bool(void)>> negativePathTestFuncs;
    std::vector<TestGoalInput> testInput;
};

#endif // TESTGOALMODEL_H_

