#include "CommandLineParser.h"
#include "commonUtilities.h"
#include "CSVReader.h"
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include "TestTaskDBInterface.h"
#include "TaskDbInterface.h"
#include "TaskModel.h"
#include "UserDbInterface.h"
#include "UserModel.h"
#include <vector>

TestTaskDBInterface::TestTaskDBInterface(std::string taskFileName)
: dataFileName{taskFileName}, verboseOutput{programOptions.verboseOutput}
{
    positiveTestFuncs.push_back(std::bind(&TestTaskDBInterface::testGetTaskByID, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestTaskDBInterface::testGetTaskByDescription, this, std::placeholders::_1));

    testsWithoutParameters.push_back(std::bind(&TestTaskDBInterface::testGetUnstartedTasks, this));
    testsWithoutParameters.push_back(std::bind(&TestTaskDBInterface::testTaskUpdates, this));
}

bool TestTaskDBInterface::runAllTests()
{
    userDBInterface.initFormatOptions();
    taskDBInteface.initFormatOptions();

    userOne = userDBInterface.getUserByUserID(1);
    if (!userOne)
    {
        std::cerr << "Failed to retrieve userOne from DataBase!\n";
        return false;
    }

    if (runNegativePathTests())
    {
        return runPositivePathTests();
    }

    return false;
}

bool TestTaskDBInterface::runNegativePathTests()
{
    std::cerr << "\n\nTestTaskDBInterface::runNegativePathTests() NOT IMPLEMENTED!!\n\n";
    return true;
}

bool TestTaskDBInterface::runPositivePathTests()
{
    bool allTestsPassed = true;
    TaskList userTaskTestData = loadTasksFromDataFile();

    for (auto testTask: userTaskTestData)
    {
        testTask->setTaskID(taskDBInteface.insert(testTask));
        if (testTask->isInDatabase())
        {
            for (auto test: positiveTestFuncs)
            {
                if (!test(testTask))
                {
                    allTestsPassed = false;
                }
            }
        }
        else
        {
            std::cerr << taskDBInteface.getAllErrorMessages() << *testTask << "\n";
            std::clog << "Primary key for task: " << testTask->getTaskID() << ", " << testTask->getDescription() <<
            " not set!\n";
            if (programOptions.verboseOutput)
            {
                std::clog << *testTask << "\n\n";
            }
            allTestsPassed = false;
        }
    }

    for (auto test: testsWithoutParameters)
    {

        if (allTestsPassed && !test())
        {
            allTestsPassed = false;
        }
    }

    if (allTestsPassed)
    {
        std::clog << "All Task insertions and retrival tests PASSED\n";
    }
    else
    {
        std::clog << "Some or all Task related tests FAILED!\n";
    }

    return allTestsPassed;
}

bool TestTaskDBInterface::testGetTaskByDescription(TaskModel_shp insertedTask)
{
    TaskModel_shp retrievedTask = taskDBInteface.getTaskByDescriptionAndAssignedUser(insertedTask->getDescription(), *userOne);
    if (retrievedTask != nullptr)
    {
        if (*retrievedTask == *insertedTask)
        {
            return true;
        }
        else
        {
            std::clog << "Inserted and retrieved Task are not the same! Test FAILED!\n";
            if (verboseOutput)
            {
                std::clog << "Inserted Task:\n" << *insertedTask << "\n" "Retreived Task:\n" << *retrievedTask << "\n";
            }
            return false;
        }
    }
    else
    {
        std::cerr << "getTaskByDescription(task.getDescription())) FAILED!\n" 
            << taskDBInteface.getAllErrorMessages() << "\n";
        return false;
    }
}

bool TestTaskDBInterface::testGetTaskByID(TaskModel_shp insertedTask)
{
    TaskModel_shp retrievedTask = taskDBInteface.getTaskByTaskID(insertedTask->getTaskID());
    if (retrievedTask)
    {
        if (*retrievedTask == *insertedTask)
        {
            return true;
        }
        else
        {
            std::clog << "Inserted and retrieved Task are not the same! Test FAILED!\n";
            if (verboseOutput)
            {
                std::clog << "Inserted Task:\n" << *insertedTask << "\n" "Retreived Task:\n" << *retrievedTask << "\n";
            }
            return false;
        }
    }
    else
    {
        std::cerr << "getTaskByDescription(task.getTaskByTaskID())) FAILED!\n" 
            << taskDBInteface.getAllErrorMessages() << "\n";
        return false;
    }
}

TaskList TestTaskDBInterface::loadTasksFromDataFile()
{
    std::size_t lCount = 0;
    TaskList inputTaskData;

    std::ifstream taskDataFile(dataFileName);
    
    for (auto row: CSVRange(taskDataFile))
    {
        // Try both constructors on an alternating basis.
        TaskModel_shp testTask = (lCount & 0x000001)? creatOddTask(row) : creatEvenTask(row);
        inputTaskData.push_back(testTask);
        ++lCount;
    }

    return inputTaskData;
}

static constexpr std::size_t CSV_MajorPriorityColIdx = 0;
static constexpr std::size_t CSV_MinorPriorityColIdx = 1;
static constexpr std::size_t CSV_DescriptionColIdx = 2;
static constexpr std::size_t CSV_RequiredByColIdx = 3;
static constexpr std::size_t CSV_EstimatedEffortColIdx = 4;
static constexpr std::size_t CSV_ActualEffortColIdx = 5;
static constexpr std::size_t CSV_ParentTaskColIdx = 6;
static constexpr std::size_t CSV_StatusColIdx = 7;
static constexpr std::size_t CSV_ScheduledStartDateColIdx = 8;
static constexpr std::size_t CSV_ActualStartDateColIdx = 9;
static constexpr std::size_t CSV_CreatedDateColIdx = 10;
static constexpr std::size_t CSV_DueDate2ColIdx = 11;
static constexpr std::size_t CSV_EstimatedCompletionDateColIdx = 12;

void TestTaskDBInterface::commonTaskInit(TaskModel_shp newTask, CSVRow taskData)
{
    // Required fields first.
    newTask->setEstimatedEffort(std::stoi(taskData[CSV_EstimatedEffortColIdx]));
    newTask->setActualEffortToDate(std::stod(taskData[CSV_ActualEffortColIdx]));
    newTask->setDueDate(stringToDate(taskData[CSV_RequiredByColIdx]));
    newTask->setScheduledStart(stringToDate(taskData[CSV_ScheduledStartDateColIdx]));
    newTask->setStatus(taskData[CSV_StatusColIdx]);
    newTask->setPriorityGroup(taskData[CSV_MajorPriorityColIdx][0]);
    newTask->setPriority(std::stoi(taskData[CSV_MinorPriorityColIdx]));
    newTask->setPercentageComplete(0.0);

    // Optional fields
    if (!taskData[CSV_ParentTaskColIdx].empty())
    {
        newTask->setParentTaskID(std::stoi(taskData[CSV_ParentTaskColIdx]));
    }

    if (!taskData[CSV_ActualStartDateColIdx].empty())
    {
        newTask->setactualStartDate(stringToDate(taskData[CSV_ActualStartDateColIdx]));
    }
    
    if (taskData.size() > CSV_EstimatedCompletionDateColIdx)
    {
        newTask->setEstimatedCompletion(stringToDate(taskData[CSV_EstimatedCompletionDateColIdx]));
    }
    
    if (!taskData[CSV_CreatedDateColIdx].empty())
    {
        // Override the auto date creation with the actual creation date.
        newTask->setCreationDate(stringToDate(taskData[CSV_CreatedDateColIdx]));
    }
}

TaskModel_shp TestTaskDBInterface::creatOddTask(CSVRow taskData)
{
    TaskModel_shp newTask = std::make_shared<TaskModel>(userOne, taskData[CSV_DescriptionColIdx]);
    commonTaskInit(newTask, taskData);

    return newTask;
}

TaskModel_shp TestTaskDBInterface::creatEvenTask(CSVRow taskData)
{
    TaskModel_shp newTask = std::make_shared<TaskModel>(userOne);
    newTask->setDescription(taskData[CSV_DescriptionColIdx]);
    commonTaskInit(newTask, taskData);

    return newTask;
}

bool TestTaskDBInterface::testGetUnstartedTasks()
{
    TaskList notStartedList = taskDBInteface.getUnstartedDueForStartForAssignedUser(userOne);
    if (!notStartedList.empty())
    {
        std::clog << std::format("Find unstarted tasks for user({}) PASSED!\n", userOne->getUserID());
        
        if (verboseOutput)
        {
            std::clog << std::format("User {} has {} unstarted tasks\n",
                userOne->getUserID(), notStartedList.size());
            for (auto task: notStartedList)
            {
                std::clog << *task << "\n";
            }
        }
        return true; 
    }

    std::cerr << std::format("taskDBInterface.getUnstartedDueForStartForAssignedUser({}) FAILED!\n", userOne->getUserID()) <<
        taskDBInteface.getAllErrorMessages() << "\n";

    return false;
}

bool TestTaskDBInterface::testTaskUpdates()
{
    TaskModel_shp firstTaskToChange = taskDBInteface.getTaskByDescriptionAndAssignedUser("Archive BHHS74Reunion website to external SSD", *userOne);
    firstTaskToChange->addEffortHours(5.0);
    firstTaskToChange->markComplete();
    if (!testTaskUpdate(firstTaskToChange))
    {
        return false;
    }

    if (!testAddDepenedcies())
    {
        return false;
    }

    std::clog << "All update task tests PASSED!\n";

    return true;
}

bool TestTaskDBInterface::testTaskUpdate(TaskModel_shp changedTask)
{
    bool testPassed = true;
    std::size_t taskID = changedTask->getTaskID();
    TaskModel_shp original = taskDBInteface.getTaskByTaskID(taskID);

    if (!taskDBInteface.update(changedTask))
    {
        std::cerr << std::format("taskDBInteface.update({}) failed execution!\n: {}\n",
            taskID, taskDBInteface.getAllErrorMessages());
        return false;
    }

    TaskModel_shp shouldBeDifferent = taskDBInteface.getTaskByTaskID(taskID);
    if (*original == *shouldBeDifferent)
    {
        std::clog << std::format("Task update test FAILED for task: {}\n", taskID);
        testPassed = false;
    }

    return testPassed;
}

bool TestTaskDBInterface::testAddDepenedcies()
{
    std::string dependentDescription("Install a WordPress Archive Plugin");
    std::string mostDependentTaskDesc("Log into PHPMyAdmin and save Database to disk");
    std::vector<std::string> taskDescriptions = {
        {"Check with GoDaddy about providing service to archive website to external SSD"},
        dependentDescription,
        {"Have GoDaddy install PHPMyAdmin"},
        {"Run Archive Plugin"}
    };

    TaskModel_shp depenedentTask = taskDBInteface.getTaskByDescriptionAndAssignedUser(taskDescriptions[1], *userOne);
    depenedentTask->addDependency(taskDBInteface.getTaskByDescriptionAndAssignedUser(taskDescriptions[0], *userOne));
    if (!taskDBInteface.update(depenedentTask))
    {
        std::clog << std::format("Update to add depenency to '{}' FAILED\n", taskDescriptions[0]);
        return false;
    }

    std::vector<std::size_t> comparison;
    TaskModel_shp mostDepenedentTask = taskDBInteface.getTaskByDescriptionAndAssignedUser(mostDependentTaskDesc, *userOne);
    for (auto task: taskDescriptions)
    {
        TaskModel_shp dependency = taskDBInteface.getTaskByDescriptionAndAssignedUser(task, *userOne);
        comparison.push_back(dependency->getTaskID());
        mostDepenedentTask->addDependency(dependency);
    }
    if (!taskDBInteface.update(mostDepenedentTask))
    {
        std::clog << std::format("Update to add depenency to '{}' FAILED\n", mostDependentTaskDesc);
        return false;
    }

    TaskModel_shp testDepenedenciesInDB = taskDBInteface.getTaskByTaskID(mostDepenedentTask->getTaskID());
    std::vector<std::size_t> dbValue = testDepenedenciesInDB->getDependencies();
    if (comparison != dbValue)
    {
        std::cerr << "Retrival of task dependencies differ, Test FAILED\n";
        return false;
    }

    return true;
}

std::chrono::year_month_day TestTaskDBInterface::stringToDate(std::string dateString)
{
    std::chrono::year_month_day dateValue = getTodaysDate();

    // First try the ISO standard date.
    std::istringstream ss(dateString);
    ss >> std::chrono::parse("%Y-%m-%d", dateValue);
    if (!ss.fail())
    {
        return dateValue;
    }

    // The ISO standard didn't work, try some local dates
    std::locale usEnglish("en_US.UTF-8");
    std::vector<std::string> legalFormats = {
        {"%B %d, %Y"},
        {"%m/%d/%Y"},
        {"%m-%d-%Y"}
    };

    ss.imbue(usEnglish);
    for (auto legalFormat: legalFormats)
    {
        ss >> std::chrono::parse(legalFormat, dateValue);
        if (!ss.fail())
        {
            return dateValue;
        }
    }

    return dateValue;
}
