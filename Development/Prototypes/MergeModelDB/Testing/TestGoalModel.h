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

    bool verboseOutput;
    std::vector<std::function<bool(UserGoalModel_shp)>>positiveTestFuncs;
    std::vector<std::function<bool(void)>> negativePathTestFuncs;
    std::vector<TestGoalInput> testInput;
};

#endif // TESTGOALMODEL_H_

