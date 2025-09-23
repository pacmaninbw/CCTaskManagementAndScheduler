#ifndef TESTGOALMODEL_H_
#define TESTGOALMODEL_H_

#include <functional>
#include <string>
#include "TestDBInterfaceCore.h"
#include "UserGoalModel.h"
#include <vector>

class TestGoalDBInterface : public TestDBInterfaceCore
{
public:
    struct TestGoalInput
    {
        std::string description;
        unsigned int priority;
        std::string parentDescription;
    };

    TestGoalDBInterface();
    ~TestGoalDBInterface() = default;
    virtual TestDBInterfaceCore::TestStatus runPositivePathTests() override;

private:

    bool verboseOutput;
    std::vector<std::function<bool(UserGoalModel_shp)>>positiveTestFuncs;
    std::vector<std::function<bool(void)>> negativePathTestFuncs;
};

#endif // TESTGOALMODEL_H_

