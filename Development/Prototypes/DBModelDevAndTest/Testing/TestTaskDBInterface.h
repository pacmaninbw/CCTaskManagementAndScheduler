#ifndef TESTTASKDBINTERFACE_H_
#define TESTTASKDBINTERFACE_H_

// Project Header Files
#include "TaskQueryProcessor.h"
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
    bool testGetTaskByDescription(TaskModel_shp task) const;
    bool testGetTaskByID(TaskModel_shp task) const;
    TaskList loadTasksFromDataFile() noexcept;
    void commonTaskInit(TaskModel_shp newTask, CSVRow taskData);
    TaskModel_shp creatOddTask(CSVRow taskData);
    TaskModel_shp creatEvenTask(CSVRow taskData);
    TestStatus testGetUnstartedTasks() const;
    TestStatus testGetDefaultDashboardTaskList() const;
    TestStatus testGetActiveTasks() const;
    TestStatus testTaskUpdates();
    TestStatus testHideUnstartedTask() const;
    bool testTaskUpdate(TaskModel_shp changedTask) const;
    bool testGetCompletedList();
    std::chrono::year_month_day stringToDate(std::string dateString);
    TestStatus testnegativePathNotModified() const;
    TestStatus testNegativePathAlreadyInDataBase() const;
    TestStatus testMissingReuqiredField(TaskModel taskMissingFields) const;
    TestStatus testNegativePathMissingRequiredFields();
    TestStatus testTasksFromDataFile();
    TestStatus testSharedPointerInteraction();
    TestStatus insertShouldPass(TaskModel_shp newTask);

    std::string m_dataFileName;
    std::vector<std::function<bool(TaskModel_shp)>> m_positiveTestFuncs;
    UserModel_shp m_userOne;
};

#endif // TESTTASKDBINTERFACE_H_
