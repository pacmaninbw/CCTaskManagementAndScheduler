#ifndef TESTTASKDBINTERFACE_H_
#define TESTTASKDBINTERFACE_H_

#include <chrono>
#include "CSVReader.h"
#include <functional>
#include <string>
#include "TaskList.h"
#include "TaskModel.h"
#include "TestDBInterfaceCore.h"
#include "UserModel.h"
#include <vector>
class TestTaskDBInterface : public TestDBInterfaceCore
{
public:
    TestTaskDBInterface(std::string taskFileName);
    ~TestTaskDBInterface() = default;
    virtual TestDBInterfaceCore::TestStatus runAllTests() override;

private:
    bool testGetTaskByDescription(TaskModel_shp task);
    bool testGetTaskByID(TaskModel_shp task);
    TaskListValues loadTasksFromDataFile();
    void commonTaskInit(TaskModel_shp newTask, CSVRow taskData);
    TaskModel_shp creatOddTask(CSVRow taskData);
    TaskModel_shp creatEvenTask(CSVRow taskData);
    TestDBInterfaceCore::TestStatus testGetUnstartedTasks();
    TestDBInterfaceCore::TestStatus testGetActiveTasks();
    TestDBInterfaceCore::TestStatus testTaskUpdates();
    bool testTaskUpdate(TaskModel_shp changedTask);
    bool testAddDepenedcies();
    bool testGetCompletedList();
    std::chrono::year_month_day stringToDate(std::string dateString);
    TestDBInterfaceCore::TestStatus testnegativePathNotModified();
    TestDBInterfaceCore::TestStatus testNegativePathAlreadyInDataBase();
    TestDBInterfaceCore::TestStatus testMissingReuqiredField(TaskModel taskMissingFields);
    TestDBInterfaceCore::TestStatus testNegativePathMissingRequiredFields();
    TestDBInterfaceCore::TestStatus testTasksFromDataFile();
    TestDBInterfaceCore::TestStatus testSharedPointerInteraction();
    TestDBInterfaceCore::TestStatus insertShouldPass(TaskModel_shp newTask);

    std::string dataFileName;
    std::vector<std::function<bool(TaskModel_shp)>> positiveTestFuncs;
    UserModel_shp userOne;
};

#endif // TESTTASKDBINTERFACE_H_
