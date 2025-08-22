#ifndef TESTTASKDBINTERFACE_H_
#define TESTTASKDBINTERFACE_H_

#include <chrono>
#include "CSVReader.h"
#include <functional>
#include <string>
#include "TaskDbInterface.h"
#include "TaskModel.h"
#include "UserDbInterface.h"
#include <vector>

class TestTaskDBInterface
{
public:
    TestTaskDBInterface(std::string taskFileName);
    ~TestTaskDBInterface() = default;
    bool runAllTests();

private:
    bool runNegativePathTests();
    bool runPositivePathTests();
    bool testGetTaskByDescription(TaskModel_shp task);
    bool testGetTaskByID(TaskModel_shp task);
    TaskList loadTasksFromDataFile();
    void commonTaskInit(TaskModel_shp newTask, CSVRow taskData);
    TaskModel_shp creatOddTask(CSVRow taskData);
    TaskModel_shp creatEvenTask(CSVRow taskData);
    bool testGetUnstartedTasks();
    bool testTaskUpdates();
    bool testTaskUpdate(TaskModel_shp changedTask);
    bool testAddDepenedcies();
    std::chrono::year_month_day stringToDate(std::string dateString);
    bool testnegativePathNotModified();
    bool testNegativePathAlreadyInDataBase();
    bool testMissingReuqiredField(TaskModel_shp taskMissingFields);
    bool wrongErrorMessage(std::string expectedString);
    bool hasErrorMessage();
    bool insertionWasSuccessfull(std::size_t taskID, std::string logMessage);
    bool testNegativePathMissingRequiredFields();

    UserDbInterface userDBInterface;
    TaskDbInterface taskDBInteface;
    std::string dataFileName;
    bool verboseOutput;
    std::vector<std::function<bool(TaskModel_shp)>> positiveTestFuncs;
    std::vector<std::function<bool(void)>> testsWithoutParameters;
    std::vector<std::function<bool(void)>> negativeTestsWithoutParameters;
    UserModel_shp userOne;
};

#endif // TESTTASKDBINTERFACE_H_
