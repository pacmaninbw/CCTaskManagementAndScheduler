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
    virtual bool databaseObjectInitAfterConstructor() override;
    TestStatus testInsertAndGetParent(TestGoalInput testGoal) const;
    TestStatus testPositivePathGoalInsertions() const;
    TestStatus testPositivePathGetListofChildrenFromParent() const;
    TestStatus testPositivePathGetAllGoalsForUser() const;
    TestStatus testPositivePathFindGoalsWithSimilarDescription() const;
    TestStatus testPositivePathDeleteGoal() const;
    TestStatus testNegativePathAlreadyInDataBase() const;
    TestStatus testnegativePathNotModified() const;
    TestStatus negativePathMissingRequiredFields() const;
    TestStatus testMissingRequiredFieldsAddUserID(
        UserGoalModel& testGoal, std::vector<std::string>& expectedErrors) const;
    TestStatus testMissingRequiredFieldsAddDescription(
        UserGoalModel& testGoal, std::vector<std::string>& expectedErrors) const;

    std::vector<TestGoalInput> m_testInput;
    std::shared_ptr<UserModel> m_userOne;
};

#endif // TESTGOALMODEL_H_

