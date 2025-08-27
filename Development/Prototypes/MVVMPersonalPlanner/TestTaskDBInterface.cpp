#include "CommandLineParser.h"
#include "commonUtilities.h"
#include "CSVReader.h"
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include "TestDBInterfaceCore.h"
#include "TestTaskDBInterface.h"
#include "TaskDbInterface.h"
#include "TaskModel.h"
#include "UserDbInterface.h"
#include "UserModel.h"
#include <vector>

TestTaskDBInterface::TestTaskDBInterface(std::string taskFileName)
: TestDBInterfaceCore(taskDBInteface, programOptions.verboseOutput, "task")
{
    dataFileName = taskFileName;
    positiveTestFuncs.push_back(std::bind(&TestTaskDBInterface::testGetTaskByID, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestTaskDBInterface::testGetTaskByDescription, this, std::placeholders::_1));

    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testTasksFromDataFile, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testGetUnstartedTasks, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testTaskUpdates, this));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testNegativePathAlreadyInDataBase, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testNegativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testSharedPointerInteraction, this));
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::runAllTests()
{
    userDBInterface.initFormatOptions();
    taskDBInteface.initFormatOptions();

    userOne = userDBInterface.getUserByUserID(1);
    if (!userOne)
    {
        std::cerr << "Failed to retrieve userOne from DataBase!\n";
        return TESTFAILED;
    }

    TestDBInterfaceCore::TestStatus positivePathPassed = runPositivePathTests();
    TestDBInterfaceCore::TestStatus negativePathPassed = runNegativePathTests();
    
    if (positivePathPassed == TESTPASSED && negativePathPassed == TESTPASSED)
    {
        std::clog << std::format(
            "All tests for database insertions and retrievals of {} PASSED!\n",
            modelUnderTest);
        return TESTPASSED;
    }

    std::clog << std::format(
        "Some or all tests for database insertions and retrievals of {} FAILED!\n",
        modelUnderTest);
    return TESTFAILED;
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
    newTask->setCreationDate(getTodaysDateMinus(5));

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

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testGetUnstartedTasks()
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
        return TESTPASSED; 
    }

    std::cerr << std::format("taskDBInterface.getUnstartedDueForStartForAssignedUser({}) FAILED!\n", userOne->getUserID()) <<
        taskDBInteface.getAllErrorMessages() << "\n";

    return TESTFAILED;
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testTaskUpdates()
{
    TaskModel_shp firstTaskToChange = taskDBInteface.getTaskByDescriptionAndAssignedUser("Archive BHHS74Reunion website to external SSD", *userOne);
    firstTaskToChange->addEffortHours(5.0);
    firstTaskToChange->markComplete();
    if (!testTaskUpdate(firstTaskToChange))
    {
        return TESTFAILED;
    }

    if (!testAddDepenedcies())
    {
        return TESTFAILED;
    }

    std::clog << "All update task tests PASSED!\n";

    return TESTPASSED;
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

    // Tests the use of both UserModel & and UserModel_shp 
    TaskModel_shp depenedentTask = taskDBInteface.getTaskByDescriptionAndAssignedUser(taskDescriptions[1], *userOne);
    depenedentTask->addDependency(taskDBInteface.getTaskByDescriptionAndAssignedUser(taskDescriptions[0], userOne));
    if (!taskDBInteface.update(depenedentTask))
    {
        std::clog << std::format("Update to add depenency to '{}' FAILED\n", taskDescriptions[0]);
        return false;
    }

    std::vector<std::size_t> comparison;
    TaskModel_shp mostDepenedentTask = taskDBInteface.getTaskByDescriptionAndAssignedUser(mostDependentTaskDesc, *userOne);
    for (auto task: taskDescriptions)
    {
        TaskModel_shp dependency = taskDBInteface.getTaskByDescriptionAndAssignedUser(task, userOne);
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

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testnegativePathNotModified()
{
    TaskModel_shp taskNotModified = taskDBInteface.getTaskByTaskID(1);
    if (taskNotModified == nullptr)
    {
        std::cerr << "Task 1 not found in database!!\n";
        return TESTFAILED;
    }

    taskNotModified->setTaskID(0); // Force it to check modified rather than Already in DB.
    taskNotModified->clearModified();
    std::vector<std::string> expectedErrors = {"not modified!"};
    return testInsertionFailureMessages(taskDBInteface.insert(taskNotModified), expectedErrors);
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testNegativePathAlreadyInDataBase()
{
    TaskModel_shp taskAlreadyInDB = taskDBInteface.getTaskByTaskID(1);
    if (taskAlreadyInDB == nullptr)
    {
        std::cerr << "Task 1 not found in database!!\n";
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(taskDBInteface.insert(taskAlreadyInDB), expectedErrors);
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testMissingReuqiredField(TaskModel& taskMissingFields)
{
    std::vector<std::string> expectedErrors = {"missing required values!"};
    return testInsertionFailureMessages(taskDBInteface.insert(taskMissingFields), expectedErrors);
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testNegativePathMissingRequiredFields()
{
    TaskModel newTask(userOne);
    if (testMissingReuqiredField(newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask.setDescription("Test missing required fields: Set Description");
    if (testMissingReuqiredField(newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask.setEstimatedEffort(3);
    if (testMissingReuqiredField(newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask.setCreationDate(getTodaysDateMinus(2));
    if (testMissingReuqiredField(newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask.setScheduledStart(getTodaysDate());
    if (testMissingReuqiredField(newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask.setDueDate(getTodaysDatePlus(2));
    if (testMissingReuqiredField(newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask.setPriorityGroup('A');
    newTask.setPriority(1);
    TaskModel_shp newTaskPtr = std::make_shared<TaskModel>(newTask);
    return insertShouldPass(newTaskPtr);
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testTasksFromDataFile()
{
    TestDBInterfaceCore::TestStatus allTestsPassed = TESTPASSED;
    TaskList userTaskTestData = loadTasksFromDataFile();

    for (auto testTask: userTaskTestData)
    {
        if (insertShouldPass(testTask) == TESTPASSED)
        {
            for (auto test: positiveTestFuncs)
            {
                if (!test(testTask))
                {
                    allTestsPassed = TESTFAILED;
                }
            }
        }
    }

    userTaskTestData.clear();

    return allTestsPassed;
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testSharedPointerInteraction()
{
    TaskModel_shp newTask = std::make_shared<TaskModel>(userOne);

    if (testMissingReuqiredField(*newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask->setDescription("Test shared pointer interaction in missing required fields");
    if (testMissingReuqiredField(*newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask->setEstimatedEffort(3);
    if (testMissingReuqiredField(*newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask->setCreationDate(getTodaysDateMinus(2));
    if (testMissingReuqiredField(*newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask->setScheduledStart(getTodaysDate());
    if (testMissingReuqiredField(*newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask->setDueDate(getTodaysDatePlus(2));
    if (testMissingReuqiredField(*newTask) != TESTPASSED)
    {
        return TESTFAILED;
    }

    newTask->setPriorityGroup('A');
    newTask->setPriority(1);
    return insertShouldPass(newTask);
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::insertShouldPass(TaskModel_shp newTask)
{
    std::size_t taskID = taskDBInteface.insert(*newTask);
    if (taskID > 0)
    {
        newTask->setTaskID(taskID);
        newTask->clearModified();
        return TESTPASSED;
    }
    else
    {
        std::cerr << taskDBInteface.getAllErrorMessages() << newTask << "\n";
        std::clog << "Primary key for task: " << newTask->getTaskID() << ", " << newTask->getDescription() <<
        " not set!\n";
        if (verboseOutput)
        {
            std::clog << newTask << "\n\n";
        }
        return TESTFAILED;
    }
}
