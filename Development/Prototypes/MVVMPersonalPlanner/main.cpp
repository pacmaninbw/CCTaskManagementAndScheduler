#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include "commonUtilities.h"
#include "CSVReader.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "TaskDbInterface.h"
#include "TaskModel.h"
#include "UserDbInterface.h"
#include "UserModel.h"
#include "UtilityTimer.h"

/*
 * All of the DBInterface classes need access to the programOptions global variable for the
 * MySQL user name and password, as well as the database name and other connection details.
 */
ProgramOptions programOptions;

static bool testGetUserByLoginAndPassword(UserDbInterface& userDBInterface, UserModel_shp insertedUser)
{
    std::string_view testName = insertedUser->getLoginName();
    std::string_view testPassword = insertedUser->getPassword();

    UserModel_shp retrievedUser = userDBInterface.getUserByLoginAndPassword(testName, testPassword);
    if (retrievedUser)
    {
        if (*retrievedUser != *insertedUser)
        {
            std::cerr << "Insertion user and retrieved User are not the same. Test FAILED!\nInserted User:\n" <<
            *insertedUser << "\n" "Retreived User:\n" << *retrievedUser << "\n";
            return false;
        }
    }
    else
    {
        std::cerr << "userDBInterface.getUserByLogin(user->getLoginName()) FAILED!\n" <<
            userDBInterface.getAllErrorMessages() << "\n";
        return false;
    }

    retrievedUser = userDBInterface.getUserByLoginAndPassword(testName, "NotThePassword");
    if (retrievedUser)
    {
        std::cerr << "userDBInterface.getUserByLogin(user->getLoginName()) Found user with fake password!\n";
        return false;
    }

    return true;
}

static bool testGetUserByLoginName(UserDbInterface& userDBInterface, UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = userDBInterface.getUserByLoginName(insertedUser->getLoginName());
    if (retrievedUser)
    {
        if (*retrievedUser == *insertedUser)
        {
            return true;
        }
        else
        {
            std::cerr << "Insertion user and retrieved User are not the same. Test FAILED!\nInserted User:\n" <<
            *insertedUser << "\n" "Retreived User:\n" << *retrievedUser << "\n";
            return false;
        }
    }
    else
    {
        std::cerr << "userDBInterface.getUserByLogin(user->getLoginName()) FAILED!\n" <<
            userDBInterface.getAllErrorMessages() << "\n";
        return false;
    }
}

static bool testGetUserByFullName(UserDbInterface& userDBInterface, UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = userDBInterface.getUserByFullName(insertedUser->getLastName(),
        insertedUser->getFirstName(), insertedUser->getMiddleInitial());
    if (retrievedUser)
    {
        if (*retrievedUser == *insertedUser)
        {
            return true;
        }
        else
        {
            std::cerr << "Insertion user and retrieved User are not the same. Test FAILED!\nInserted User:\n" <<
            *insertedUser << "\n" "Retreived User:\n" << *retrievedUser << "\n";
            return false;
        }
    }
    else
    {
        std::cerr << "userDBInterface.getUserByFullName() FAILED!\n" <<
            userDBInterface.getAllErrorMessages() << "\n";
        return false;
    }
}
static bool testGetAllUsers(UserList userProfileTestData, UserDbInterface& userDbInterface)
{
    UserList allUsers = userDbInterface.getAllUsers();
    bool testPassed = false;

    if ((userProfileTestData.size() == allUsers.size()) &&
        std::equal(userProfileTestData.begin(), userProfileTestData.end(), allUsers.begin(),
            [](const UserModel_shp a, const UserModel_shp b) { return *a == *b; }))
    {
        testPassed = true;
    }
    else
    {
        std::clog << "Get All users FAILED! " << allUsers.size() << "\n";
        if (userProfileTestData.size() != allUsers.size())
        {
            std::clog << std::format("Size differs: userProfileTestData.size({}) != llUsers.size({})",
                userProfileTestData.size(), allUsers.size());
        }
        else
        {
            for (std::size_t userLisetIdx = 0; userLisetIdx < userProfileTestData.size(); ++userLisetIdx)
            {
                if (*userProfileTestData[userLisetIdx] != *allUsers[userLisetIdx])
                {
                    std::clog << std::format("Original Data [{}]", userLisetIdx) << "\n" <<
                        *userProfileTestData[userLisetIdx] << std::format("Database Data [{}]", userLisetIdx) << 
                        "\n" << *allUsers[userLisetIdx] << "\n";
                }
            }
        }
    }

    allUsers.clear();

    return testPassed;
}

static void loadTestUsersFromFile(std::string fileName, UserList& userProfileTestData)
{
    std::ifstream userData(fileName);
    
    for (auto row: CSVRange(userData))
    {
        UserModel_shp userIn = std::make_shared<UserModel>(UserModel());
        userIn->setLastName(row[0]);
        userIn->setFirstName(row[1]);
        userIn->setMiddleInitial(row[2]);
        userIn->setEmail(row[3]);
        userIn->autoGenerateLoginAndPassword();
        userProfileTestData.push_back(userIn);
    }
}

static bool loadUserProfileTestDataIntoDatabase()
{
    // Test one case of the alternate constructor.
    UserList userProfileTestData = {{std::make_shared<UserModel>("PacMan", "IN", "BW", "pacmaninbw@gmail.com")}};
    loadTestUsersFromFile(programOptions.userTestDataFile, userProfileTestData);

    UserDbInterface userDBInterface;
    bool allTestsPassed = true;

    for (auto user: userProfileTestData)
    {
        std::size_t userID = userDBInterface.insert(user);
        user->setUserID(userID);
        if (!userID)
        {
            std::cerr << userDBInterface.getAllErrorMessages() << "\n" << *user << "\n";
            allTestsPassed = false;
        }
        else
        {
            if (user->isInDataBase())
            {
                if (!testGetUserByLoginName(userDBInterface, user))
                {
                    allTestsPassed = false;
                }

                if (!testGetUserByLoginAndPassword(userDBInterface, user))
                {
                    allTestsPassed = false;
                }

                if (!testGetUserByFullName(userDBInterface, user))
                {
                    allTestsPassed = false;
                }
            }
            else
            {
                std::clog << "Primary key for user: " << user->getLastName() << ", " << user->getFirstName() << " not set!\n";
                if (programOptions.verboseOutput)
                {
                    std::clog << *user << "\n\n";
                }
                allTestsPassed = false;
            }
        }
    }

    if (allTestsPassed)
    {
        allTestsPassed = testGetAllUsers(userProfileTestData, userDBInterface);
    }

    userProfileTestData.clear();

    if (allTestsPassed)
    {
        std::clog << "Insertion and retrieval of users test PASSED!\n";
        return true;
    }
    else
    {
        std::cerr << "Some or all insertion and retrieval of users test FAILED!\n";
        return false;
    }
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
        std::cerr << "userDBInterface.getTaskByDescription(task.getDescription())) FAILED!\n" 
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
        std::cerr << "userDBInterface.getTaskByDescription(task.getTaskByTaskID())) FAILED!\n" 
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
            if (loadUserProfileTestDataIntoDatabase())
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

