#ifndef TESTTASKDBINTERFACE_H_
#define TESTTASKDBINTERFACE_H_

// Project Header Files
#include "TaskList.h"
#include "TaskModel.h"
#include "TestDBInterfaceCore.h"
#include "UserModel.h"

// StandardS C++ Header Files
#include <chrono>
#include "CSVReader.h"
#include <functional>
#include <string>
#include <vector>
class TestTaskDBInterface : public TestDBInterfaceCore
{
public:
    TestTaskDBInterface(std::string taskFileName);
    ~TestTaskDBInterface() = default;
    virtual TestStatus runAllTests() override;

private:
    bool testGetTaskByDescription(TaskModel_shp task);
    bool testGetTaskByID(TaskModel_shp task);
    TaskListValues loadTasksFromDataFile();
    void commonTaskInit(TaskModel_shp newTask, CSVRow taskData);
    TaskModel_shp creatOddTask(CSVRow taskData);
    TaskModel_shp creatEvenTask(CSVRow taskData);
    TestStatus testGetUnstartedTasks();
    TestStatus testGetDefaultDashboardTaskList();
    TestStatus testGetActiveTasks();
    TestStatus testTaskUpdates();
    bool testTaskUpdate(TaskModel_shp changedTask);
    bool testAddDepenedcies();
    bool testGetCompletedList();
    std::chrono::year_month_day stringToDate(std::string dateString);
    TestStatus testnegativePathNotModified();
    TestStatus testNegativePathAlreadyInDataBase();
    TestStatus testMissingReuqiredField(TaskModel taskMissingFields);
    TestStatus testNegativePathMissingRequiredFields();
    TestStatus testTasksFromDataFile();
    TestStatus testSharedPointerInteraction();
    TestStatus insertShouldPass(TaskModel_shp newTask);

    std::string dataFileName;
    std::vector<std::function<bool(TaskModel_shp)>> positiveTestFuncs;
    UserModel_shp userOne;
};

#endif // TESTTASKDBINTERFACE_H_
