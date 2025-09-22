#include <functional>
#include <string>
#include "TestGoal.h"
#include "TestDBInterfaceCore.h"
#include "UserGoalModel.h"
#include "UserModel.h"
#include <vector>


TestGoalDBInterface::TestGoalDBInterface()
: TestDBInterfaceCore(programOptions.verboseOutput, "user")
{
#if 0
    positiveTestFuncs.push_back(std::bind(&TestGoalDBInterface::testGetUserByLoginName, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestGoalDBInterface::testGetUserByLoginAndPassword, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestGoalDBInterface::testGetUserByFullName, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestGoalDBInterface::testUpdateUserPassword, this, std::placeholders::_1));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalDBInterface::negativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalDBInterface::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestGoalDBInterface::testNegativePathAlreadyInDataBase, this));
#endif
}

TestDBInterfaceCore::TestStatus TestGoalDBInterface::runPositivePathTests()
{
    return TESTFAILED;
}