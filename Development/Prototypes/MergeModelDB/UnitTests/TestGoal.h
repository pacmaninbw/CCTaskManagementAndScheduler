#ifndef TESTUSERDBINTERFACE_H_
#define TESTUSERDBINTERFACE_H_

#include <functional>
#include <string>
#include "TestDBInterfaceCore.h"
#include "UserModel.h"
#include <vector>

class TestGoalDBInterface : public TestDBInterfaceCore
{
public:
    TestGoalDBInterface();
    ~TestGoalDBInterface() = default;
    virtual TestDBInterfaceCore::TestStatus runPositivePathTests() override;

private:

    bool verboseOutput;
    std::vector<std::function<bool(UserModel_shp)>>positiveTestFuncs;
    std::vector<std::function<bool(void)>> negativePathTestFuncs;
};

#endif // TESTUSERDBINTERFACE_H_

