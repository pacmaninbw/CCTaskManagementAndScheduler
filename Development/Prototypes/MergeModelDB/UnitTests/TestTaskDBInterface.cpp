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
#include "TaskModel.h"
#include "UserModel.h"
#include <vector>

TestTaskDBInterface::TestTaskDBInterface(std::string taskFileName)
: TestDBInterfaceCore(programOptions.verboseOutput, "task")
{
    dataFileName = taskFileName;
    positiveTestFuncs.push_back(std::bind(&TestTaskDBInterface::testGetTaskByID, this, std::placeholders::_1));
    positiveTestFuncs.push_back(std::bind(&TestTaskDBInterface::testGetTaskByDescription, this, std::placeholders::_1));

    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testTasksFromDataFile, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testGetUnstartedTasks, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testTaskUpdates, this));
    positiviePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testGetActiveTasks, this));

    negativePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testNegativePathAlreadyInDataBase, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testnegativePathNotModified, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testNegativePathMissingRequiredFields, this));
    negativePathTestFuncsNoArgs.push_back(std::bind(&TestTaskDBInterface::testSharedPointerInteraction, this));
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::runAllTests()
{
    userOne = std::make_shared<UserModel>();
    userOne->setUserID(1);
    userOne->retrieve();
    if (!userOne->isInDataBase())
    {
        std::cerr << "Failed to retrieve userOne from DataBase!\n";
        return TESTFAILED;
    }

    TestDBInterfaceCore::TestStatus positivePathPassed = runPositivePathTests();
    TestDBInterfaceCore::TestStatus negativePathPassed = runNegativePathTests();

    TestDBInterfaceCore::TestStatus allTestsStatus =
        (positivePathPassed == TESTPASSED && negativePathPassed == TESTPASSED) ? TESTPASSED : TESTFAILED;

    reportTestStatus(allTestsStatus, "");

    return allTestsStatus;
}

bool TestTaskDBInterface::testGetTaskByDescription(TaskModel_shp insertedTask)
{
    TaskModel_shp retrievedTask = std::make_shared<TaskModel>();
    if (retrievedTask->selectByDescriptionAndAssignedUser(insertedTask->getDescription(), userOne->getUserID()))
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
            << retrievedTask->getAllErrorMessages() << "\n";
        return false;
    }
}

bool TestTaskDBInterface::testGetTaskByID(TaskModel_shp insertedTask)
{
    TaskModel_shp retrievedTask = std::make_shared<TaskModel>();
    retrievedTask->setTaskID(insertedTask->getTaskID());
    if (retrievedTask->retrieve())
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
            << retrievedTask->getAllErrorMessages() << "\n";
        return false;
    }
}

TaskListValues TestTaskDBInterface::loadTasksFromDataFile()
{
    std::size_t lCount = 0;
    TaskListValues inputTaskData;

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
    TaskModel_shp newTask = std::make_shared<TaskModel>(userOne->getUserID(), taskData[CSV_DescriptionColIdx]);
    commonTaskInit(newTask, taskData);

    return newTask;
}

TaskModel_shp TestTaskDBInterface::creatEvenTask(CSVRow taskData)
{
    TaskModel_shp newTask = std::make_shared<TaskModel>(userOne->getUserID());
    newTask->setDescription(taskData[CSV_DescriptionColIdx]);
    commonTaskInit(newTask, taskData);

    return newTask;
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testGetUnstartedTasks()
{
    TaskList taskDBInteface;
    TaskListValues notStartedList = taskDBInteface.getUnstartedDueForStartForAssignedUser(userOne->getUserID());
    if (!notStartedList.empty())
    {    
        if (verboseOutput)
        {
            std::clog << std::format("Find unstarted tasks for user({}) PASSED!\n", userOne->getUserID());
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

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testGetActiveTasks()
{
    TaskList taskDBInteface;
    TaskListValues activeTasks = taskDBInteface.getActiveTasksForAssignedUser(userOne->getUserID());
    if (!activeTasks.empty())
    {    
        if (verboseOutput)
        {
            std::clog << std::format("Find active tasks for user({}) PASSED!\n", userOne->getUserID());
            std::clog << std::format("User {} has {} unstarted tasks\n",
                userOne->getUserID(), activeTasks.size());
            for (auto task: activeTasks)
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
    TaskModel_shp firstTaskToChange = std::make_shared<TaskModel>();
    firstTaskToChange->selectByDescriptionAndAssignedUser("Archive BHHS74Reunion website to external SSD", userOne->getUserID());
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

    if (!testGetCompletedList())
    {
        return TESTFAILED;
    }

    return TESTPASSED;
}

bool TestTaskDBInterface::testTaskUpdate(TaskModel_shp changedTask)
{
    bool testPassed = true;
    std::size_t taskID = changedTask->getTaskID();
    TaskModel_shp original = std::make_shared<TaskModel>();
    original->setTaskID(taskID);
    original->retrieve();

    if (!changedTask->update())
    {
        std::cerr << std::format("taskDBInteface.update({}) failed execution!\n: {}\n",
            taskID, changedTask->getAllErrorMessages());
        return false;
    }

    TaskModel_shp shouldBeDifferent = std::make_shared<TaskModel>();
    shouldBeDifferent->setTaskID(taskID);
    shouldBeDifferent->retrieve();
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
    std::size_t user1ID = userOne->getUserID();
    TaskModel_shp depenedentTask = std::make_shared<TaskModel>();
    TaskModel_shp depenedsOn = std::make_shared<TaskModel>();
    depenedsOn->selectByDescriptionAndAssignedUser(taskDescriptions[0], user1ID);
    depenedentTask->selectByDescriptionAndAssignedUser(taskDescriptions[1], user1ID);
    depenedentTask->addDependency(depenedsOn);
    if (!depenedentTask->update())
    {
        std::clog << std::format("Update to add depenency to '{}' FAILED\n", taskDescriptions[0]);
        return false;
    }

    std::vector<std::size_t> comparison;
    TaskModel_shp mostDepenedentTask = std::make_shared<TaskModel>();
    mostDepenedentTask->selectByDescriptionAndAssignedUser(mostDependentTaskDesc, user1ID);
    for (auto task: taskDescriptions)
    {
        TaskModel_shp dependency = std::make_shared<TaskModel>();
        dependency->selectByDescriptionAndAssignedUser(task, user1ID);
        comparison.push_back(dependency->getTaskID());
        mostDepenedentTask->addDependency(dependency);
    }
    if (!mostDepenedentTask->update())
    {
        std::clog << std::format("Update to add depenency to '{}' FAILED\n", mostDependentTaskDesc);
        return false;
    }

    TaskModel_shp testDepenedenciesInDB = std::make_shared<TaskModel>();
    testDepenedenciesInDB->setTaskID(mostDepenedentTask->getTaskID());
    testDepenedenciesInDB->retrieve();
    std::vector<std::size_t> dbValue = testDepenedenciesInDB->getDependencies();
    if (comparison != dbValue)
    {
        std::cerr << "Retrival of task dependencies differ, Test FAILED\n";
        return false;
    }

    return true;
}

bool TestTaskDBInterface::testGetCompletedList()
{
    std::size_t user1ID = userOne->getUserID();

    TaskModel_shp parentTask = std::make_shared<TaskModel>();
    parentTask->selectByDescriptionAndAssignedUser("Archive BHHS74Reunion website to external SSD", user1ID);
    TaskModel::TaskStatus newStatus = TaskModel::TaskStatus::Complete;

    std::chrono::year_month_day completedDate = parentTask->getCompletionDate();

    if (!completedDate.ok())
    {
        std::cerr << "Parent Completion Date Not set\n" << *parentTask << "\n" << "completedDate " << completedDate << "\n";
        return false;
    }

    TaskList taskSearch;

    TaskListValues tasksToMarkComplete = taskSearch.getTasksByAssignedIDandParentID(user1ID, parentTask->getTaskID());
    for (auto task: tasksToMarkComplete)
    {
        task->setCompletionDate(completedDate);
        task->setStatus(newStatus);
        if (!task->update())
        {
            std::cerr << std::format("In testGetCompletedList Task Update Failed: \n{}\n", task->getAllErrorMessages());
            return false;
        }
    }

    std::chrono::year_month_day searchAfter = stringToDate("2025-5-11");
    TaskListValues completedTasks = taskSearch.getTasksCompletedByAssignedAfterDate(user1ID, searchAfter);

    if (completedTasks.size() != (tasksToMarkComplete.size() + 1))
    {
        std::clog << std::format("Test FAILED: completedTasks.size() {} != expected value {}\n",
            completedTasks.size(), tasksToMarkComplete.size() + 1);
        return false;
    }

    return true;;
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
    TaskModel_shp taskNotModified = std::make_shared<TaskModel>();
    taskNotModified->setTaskID(1);
    if (!taskNotModified->retrieve())
    {
        std::cerr << "Task 1 not found in database!!\n";
        return TESTFAILED;
    }

    taskNotModified->setTaskID(0); // Force it to check modified rather than Already in DB.
    taskNotModified->clearModified();
    std::vector<std::string> expectedErrors = {"not modified!"};
    return testInsertionFailureMessages(taskNotModified, expectedErrors);
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testNegativePathAlreadyInDataBase()
{
    TaskModel_shp taskAlreadyInDB = std::make_shared<TaskModel>();
    taskAlreadyInDB->setTaskID(1);
    if (!taskAlreadyInDB->retrieve())
    {
        std::cerr << "Task 1 not found in database!!\n";
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(taskAlreadyInDB, expectedErrors);
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testMissingReuqiredField(TaskModel taskMissingFields)
{
    std::vector<std::string> expectedErrors = {"missing required values!"};
    return testInsertionFailureMessages(&taskMissingFields, expectedErrors);
}

TestDBInterfaceCore::TestStatus TestTaskDBInterface::testNegativePathMissingRequiredFields()
{
    TaskModel newTask(userOne->getUserID());
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
    TaskListValues userTaskTestData = loadTasksFromDataFile();

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
    TaskModel_shp newTask = std::make_shared<TaskModel>(userOne->getUserID());

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
    if (newTask->insert())
    {
        return TESTPASSED;
    }
    else
    {
        std::cerr << newTask->getAllErrorMessages() << newTask << "\n";
        std::clog << "Primary key for task: " << newTask->getTaskID() << ", " << newTask->getDescription() <<
        " not set!\n";
        if (verboseOutput)
        {
            std::clog << newTask << "\n\n";
        }
        return TESTFAILED;
    }
}

