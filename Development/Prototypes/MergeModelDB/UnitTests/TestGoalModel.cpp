#include <functional>
#include <string>
#include "TestGoalModel.h"
#include "TestDBInterfaceCore.h"
#include "UserGoalModel.h"
#include "UserModel.h"
#include <vector>

TestGoalModel::TestGoalModel()
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

TestDBInterfaceCore::TestStatus TestGoalModel::runPositivePathTests()
{
    return TESTFAILED;
}