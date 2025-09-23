#include <functional>
#include <string>
#include "TestGoal.h"
#include "TestDBInterfaceCore.h"
#include "UserGoalModel.h"
#include <vector>

TestGoalDBInterface::TestGoalDBInterface()
: TestDBInterfaceCore(programOptions.verboseOutput, "user")
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
        {"", 0, ""},
        {"", 0, ""},
        {"", 0, ""},
        {"", 0, ""},
        {"", 0, ""},
        {"", 0, ""},
        {"", 0, ""},
        {"", 0, ""},
        {"", 0, ""},
        {"", 0, ""},
        {"", 0, ""},
    };

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