#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include "commonUtilities.h"
#include "CSVReader.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include "TaskDbInterface.h"
#include "TaskModel.h"
#include "TestUserDBInterface.h"
#include "UserDbInterface.h"
#include "UserModel.h"
#include "UtilityTimer.h"
#include <vector>

/*
 * All of the DBInterface classes need access to the programOptions global variable for the
 * MySQL user name and password, as well as the database name and other connection details.
 */
ProgramOptions programOptions;


static bool testTaskUpdate(TaskDbInterface& taskDBInterface, TaskModel& changedTask)
{
    bool testPassed = true;
    std::size_t taskID = changedTask.getTaskID();
    TaskModel_shp original = taskDBInterface.getTaskByTaskID(taskID);

    if (!taskDBInterface.update(changedTask))
    {
        std::cerr << std::format("taskDBInterface.update({}) failed execution!\n: {}\n",
            taskID, taskDBInterface.getAllErrorMessages());
        return false;
    }

    TaskModel_shp shouldBeDifferent = taskDBInterface.getTaskByTaskID(taskID);
    if (*original == *shouldBeDifferent)
    {
        std::clog << std::format("Task update test FAILED for task: {}\n", taskID);
        testPassed = false;
    }

    return testPassed;
}

static bool testAddDepenedcies(TaskDbInterface dbInterface, UserModel& user)
{
    std::string dependentDescription("Install a WordPress Archive Plugin");
    std::string mostDependentTaskDesc("Log into PHPMyAdmin and save Database to disk");
    std::vector<std::string> taskDescriptions = {
        {"Check with GoDaddy about providing service to archive website to external SSD"},
        dependentDescription,
        {"Have GoDaddy install PHPMyAdmin"},
        {"Run Archive Plugin"}
    };

    TaskModel_shp depenedentTask = dbInterface.getTaskByDescriptionAndAssignedUser(taskDescriptions[1], user);
    depenedentTask->addDependency(dbInterface.getTaskByDescriptionAndAssignedUser(taskDescriptions[0], user));
    if (!dbInterface.update(depenedentTask))
    {
        std::clog << std::format("Update to add depenency to '{}' FAILED\n", taskDescriptions[0]);
        return false;
    }

    std::vector<std::size_t> comparison;
    TaskModel_shp mostDepenedentTask = dbInterface.getTaskByDescriptionAndAssignedUser(mostDependentTaskDesc, user);
    for (auto task: taskDescriptions)
    {
        TaskModel_shp dependency = dbInterface.getTaskByDescriptionAndAssignedUser(task, user);
        comparison.push_back(dependency->getTaskID());
        mostDepenedentTask->addDependency(dependency);
    }
    if (!dbInterface.update(mostDepenedentTask))
    {
        std::clog << std::format("Update to add depenency to '{}' FAILED\n", mostDependentTaskDesc);
        return false;
    }

    TaskModel_shp testDepenedenciesInDB = dbInterface.getTaskByTaskID(mostDepenedentTask->getTaskID());
    std::vector<std::size_t> dbValue = testDepenedenciesInDB->getDependencies();
    if (comparison != dbValue)
    {
        std::cerr << "Retrival of task dependencies differ, Test FAILED\n";
        return false;
    }

    return true;
}

static bool testSomeUpdates(TaskDbInterface dbInterface, UserModel& user)
{
    TaskModel_shp firstTaskToChange = dbInterface.getTaskByDescriptionAndAssignedUser("Archive BHHS74Reunion website to external SSD", user);
    firstTaskToChange->addEffortHours(5.0);
    firstTaskToChange->markComplete();
    if (!testTaskUpdate(dbInterface, *firstTaskToChange))
    {
        return false;
    }

    if (!testAddDepenedcies(dbInterface, user))
    {
        return false;
    }

    std::clog << "All update task tests PASSED!\n";

    return true;
}

static bool testGetTaskByDescription(TaskDbInterface& taskDBInterface, TaskModel& task, UserModel& user , bool verboseOutput)
{
    TaskModel_shp testInDB = taskDBInterface.getTaskByDescriptionAndAssignedUser(task.getDescription(), user);
    if (testInDB)
    {
        if (*testInDB == task)
        {
            return true;
        }
        else
        {
            std::clog << "Inserted and retrieved Task are not the same! Test FAILED!\n";
            if (verboseOutput)
            {
                std::clog << "Inserted Task:\n" << task << "\n" "Retreived Task:\n" << *testInDB << "\n";
            }
            return false;
        }
    }
    else
    {
        std::cerr << "taskDBInterface.getTaskByDescription(task.getDescription())) FAILED!\n" 
            << taskDBInterface.getAllErrorMessages() << "\n";
        return false;
    }
}

static bool testGetTaskByID(TaskDbInterface& taskDBInterface, TaskModel& task, bool verboseOutput)
{
    TaskModel_shp testInDB = taskDBInterface.getTaskByTaskID(task.getTaskID());
    if (testInDB)
    {
        if (*testInDB == task)
        {
            return true;
        }
        else
        {
            std::clog << "Inserted and retrieved Task are not the same! Test FAILED!\n";
            if (verboseOutput)
            {
                std::clog << "Inserted Task:\n" << task << "\n" "Retreived Task:\n" << *testInDB << "\n";
            }
            return false;
        }
    }
    else
    {
        std::cerr << "taskDBInterface.getTaskByDescription(task.getTaskByTaskID())) FAILED!\n" 
            << taskDBInterface.getAllErrorMessages() << "\n";
        return false;
    }
}

static bool testGetUnstartedTasks(TaskDbInterface& taskDBInterface, UserModel_shp assigned, bool verboseOutput)
{
    TaskList notStartedList = taskDBInterface.getUnstartedDueForStartForAssignedUser(assigned);
    if (!notStartedList.empty())
    {
        std::clog << std::format("Find unstarted tasks for user({}) PASSED!\n", assigned->getUserID());
        
        if (verboseOutput)
        {
            std::clog << std::format("User {} has {} unstarted tasks\n",
                assigned->getUserID(), notStartedList.size());
            for (auto task: notStartedList)
            {
                std::clog << *task << "\n";
            }
        }
        return true; 
    }

    std::cerr << std::format("taskDBInterface.getUnstartedDueForStartForAssignedUser({}) FAILED!\n", assigned->getUserID()) <<
        taskDBInterface.getAllErrorMessages() << "\n";

    return false;
}

struct UserTaskTestData
{
    char majorPriority;
    unsigned int minorPriority;
    std::string description;
    std::string dueDate;
    unsigned int estimatedEffortHours;
    double actualEffortHours;
    std::size_t parentTask;
    std::string dependencies;
    std::string status;
    std::string scheduledStartDate;
    std::string actualStartDate;
    std::string createdDate;
    std::string dueDate2;
    std::string estimatedCompletionDate;
};

static std::chrono::year_month_day stringToDate(std::string dateString)
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

static std::vector<UserTaskTestData> loadTasksFromDataFile(std::string taskFileName)
{
    std::vector<UserTaskTestData> inputTaskData;

    std::ifstream taskDataFile(taskFileName);
    
    for (auto row: CSVRange(taskDataFile))
    {
        UserTaskTestData currentTask;
        currentTask.majorPriority = row[0][0];
        currentTask.minorPriority = std::stoi(row[1]);
        currentTask.description = row[2];
        currentTask.dueDate = row[3];
        currentTask.estimatedEffortHours = std::stoi(row[4]);
        currentTask.actualEffortHours = std::stod(row[5]);
        currentTask.parentTask = std::stoi(row[6]);
        currentTask.status = row[7];
        currentTask.scheduledStartDate = row[8];
        currentTask.actualStartDate = row[9];
        currentTask.createdDate = row[10];
        currentTask.dueDate2 = row[11];
        if (row.size() > 12)
        {
            currentTask.estimatedCompletionDate = row[12];
        }

        inputTaskData.push_back(currentTask);
    }

    return inputTaskData;
}

static void commonTaskInit(TaskModel_shp newTask, const UserTaskTestData taskData)
{
    // Required fields first.
    newTask->setEstimatedEffort(taskData.estimatedEffortHours);
    newTask->setActualEffortToDate(taskData.actualEffortHours);
    newTask->setDueDate(stringToDate(taskData.dueDate));
    newTask->setScheduledStart(stringToDate(taskData.scheduledStartDate));
    newTask->setStatus(taskData.status);
    newTask->setPriorityGroup(taskData.majorPriority);
    newTask->setPriority(taskData.minorPriority);
    newTask->setPercentageComplete(0.0);

    // Optional fields
    if (taskData.parentTask)
    {
        newTask->setParentTaskID(taskData.parentTask);
    }
    if (!taskData.actualStartDate.empty())
    {
        newTask->setactualStartDate(stringToDate(taskData.actualStartDate));
    }
    if (!taskData.estimatedCompletionDate.empty())
    {
        newTask->setEstimatedCompletion(stringToDate(taskData.estimatedCompletionDate));
    }
    if (!taskData.createdDate.empty())
    {
        // Override the auto date creation with the actual creation date.
        newTask->setCreationDate(stringToDate(taskData.createdDate));
    }
}

static TaskModel_shp creatOddTask(const UserModel_shp userOne, const UserTaskTestData taskData)
{
    TaskModel_shp newTask = std::make_shared<TaskModel>(userOne, taskData.description);
    commonTaskInit(newTask, taskData);

    return newTask;
}

static TaskModel_shp creatEvenTask(const UserModel_shp userOne, const UserTaskTestData taskData)
{
    TaskModel_shp newTask = std::make_shared<TaskModel>(userOne);
    newTask->setDescription(taskData.description);
    commonTaskInit(newTask, taskData);

    return newTask;
}

static bool loadUserTaskestDataIntoDatabase()
{
    UserDbInterface userDbInterface;
    UserModel_shp userOne = userDbInterface.getUserByUserID(1);
    if (!userOne)
    {
        std::cerr << "Failed to retrieve userOne from DataBase!\n";
        return false;
    }

    TaskDbInterface taskDBInterface;
    taskDBInterface.initFormatOptions();
    bool allTestsPassed = true;
    std::size_t lCount = 0;
    std::vector<UserTaskTestData> userTaskTestData = loadTasksFromDataFile(programOptions.taskTestDataFile);

    for (auto taskTestData: userTaskTestData)
    {
        // Try both constructors on an alternating basis.
        TaskModel_shp testTask = (lCount & 0x000001)? creatOddTask(userOne, taskTestData) :
            creatEvenTask(userOne, taskTestData);
        testTask->setTaskID(taskDBInterface.insert(testTask));

        if (testTask->isInDatabase())
        {
            if (!testGetTaskByID(taskDBInterface, *testTask, programOptions.verboseOutput))
            {
                allTestsPassed = false;
            }

            if (!testGetTaskByDescription(taskDBInterface, *testTask, *userOne, programOptions.verboseOutput))
            {
                allTestsPassed = false;
            }
        }
        else
        {
            std::cerr << taskDBInterface.getAllErrorMessages() << *testTask << "\n";
            std::clog << "Primary key for task: " << testTask->getTaskID() << ", " << testTask->getDescription() <<
            " not set!\n";
            if (programOptions.verboseOutput)
            {
                std::clog << *testTask << "\n\n";
            }
            allTestsPassed = false;
        }
        ++lCount;
    }

    if (allTestsPassed)
    {
        allTestsPassed = testGetUnstartedTasks(taskDBInterface, userOne, programOptions.verboseOutput);
    }

    if (allTestsPassed)
    {
        allTestsPassed = testSomeUpdates(taskDBInterface, *userOne);
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

int main(int argc, char* argv[])
{
    try {
		if (const auto progOptions = parseCommandLine(argc, argv); progOptions.has_value())
		{
			programOptions = *progOptions;
            UtilityTimer stopWatch;
            TestUserDBInterface userTests(programOptions.userTestDataFile);

            if (userTests.runAllTests())
            {
                if (!loadUserTaskestDataIntoDatabase())
                {
                    return EXIT_FAILURE;
                }
            }
            else
            {
                return EXIT_FAILURE;
            }
            std::clog << "All tests Passed\n";
			if (programOptions.enableExecutionTime)
			{
                stopWatch.stopTimerAndReport("Testing of Insertion and retrieval of users and tasks in MySQL database\n");
			}
        }
        else
		{
			if (progOptions.error() != CommandLineStatus::HelpRequested)
			{
				return EXIT_FAILURE;
			}
		}
    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

