#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
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

static bool testGetUserByLoginName(UserDbInterface& userDBInterface, UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = std::make_shared<UserModel>(UserModel());
    retrievedUser->setLoginName(insertedUser->getLoginName());
    if (retrievedUser->getDatabaseValues())
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
    UserModel_shp retrievedUser = retrievedUser = userDBInterface.getUserByFullName(insertedUser->getLastName(),
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

static UserList loadUserProfileTestDataIntoDatabase()
{
    // Test one case of the alternate constructor.
    UserList userProfileTestData = {{std::make_shared<UserModel>("PacMan", "IN", "BW", "pacmaninbw@gmail.com")}};

    std::ifstream userData(programOptions.userTestDataFile);
    
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
        std::clog << "Insertion and retrieval of users test PASSED\n";
    }
    else
    {
        userProfileTestData.clear();
    }
    return userProfileTestData;
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

static std::chrono::year_month_day getTodaysDate()
{
    std::chrono::time_point<std::chrono::system_clock> today = std::chrono::system_clock::now();
    return std::chrono::floor<std::chrono::days>(today);
}

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
    newTask->setactualEffortToDate(taskData.actualEffortHours);
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

static bool loadUserTaskestDataIntoDatabase(UserModel_shp userOne)
{
    TaskDbInterface taskDBInterface;
    bool allTestsPassed = true;
    std::size_t lCount = 0;
    std::vector<UserTaskTestData> userTaskTestData = loadTasksFromDataFile(programOptions.taskTestDataFile);;

    for (auto taskTestData: userTaskTestData)
    {
        // Try both constructors on an alternating basis.
        TaskModel_shp testTask = (lCount & 0x000001)? creatOddTask(userOne, taskTestData) : creatEvenTask(userOne, taskTestData);
        testTask->setTaskID(taskDBInterface.insert(testTask));

        if (testTask->isInDatabase())
        {
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

    std::clog << "All Task insertions and retrival tests PASSED\n";
    return allTestsPassed;
}

int main(int argc, char* argv[])
{
    try {
		if (const auto progOptions = parseCommandLine(argc, argv); progOptions.has_value())
		{
			ProgramOptions programOptions = *progOptions;
            UtilityTimer stopWatch;
            UserList userList = loadUserProfileTestDataIntoDatabase();
            if (userList.size())
            {
                if (!loadUserTaskestDataIntoDatabase(userList[0]))
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

