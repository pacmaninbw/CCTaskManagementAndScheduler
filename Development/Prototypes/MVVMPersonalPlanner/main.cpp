#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include "DBInterface.h"
#include "CSVReader.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "TaskModel.h"
#include "UserModel.h"
#include "UtilityTimer.h"

static bool testGetUserByLoginName(DBInterface& userDBInterface, UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = std::make_shared<UserModel>(UserModel());
    if (userDBInterface.getUniqueModelFromDB(retrievedUser, {{"LoginName", PTS_DataField(insertedUser->getLoginName())}}))
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

static bool testGetUserByFullName(DBInterface& userDBInterface, UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = std::make_shared<UserModel>(UserModel());
    if (userDBInterface.getUniqueModelFromDB(retrievedUser, {{"LastName", PTS_DataField(insertedUser->getLastName())},
        {"FirstName", PTS_DataField(insertedUser->getFirstName())}, {"MiddleInitial", PTS_DataField(insertedUser->getMiddleInitial())}}))
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

static UserList loadUserProfileTestDataIntoDatabase(ProgramOptions &programOptions)
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

    DBInterface userDBInterface(programOptions);
    bool allTestsPassed = true;

    for (auto user: userProfileTestData)
    {
        if (!userDBInterface.insertIntoDataBase(*user))
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

static bool testGetTaskByDescription(DBInterface& taskDBInterface, TaskModel& task, bool verboseOutput)
{
    TaskModel_shp testInDB = std::make_shared<TaskModel>(TaskModel());
    if (taskDBInterface.getUniqueModelFromDB(testInDB, {{"Description", PTS_DataField(task.getDescription())}}))
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
    newTask->setDueDate(newTask->stringToDate(taskData.dueDate));
    newTask->setScheduledStart(newTask->stringToDate(taskData.scheduledStartDate));
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
        newTask->setactualStartDate(newTask->stringToDate(taskData.actualStartDate));
    }
    if (!taskData.estimatedCompletionDate.empty())
    {
        newTask->setEstimatedCompletion(newTask->stringToDate(taskData.estimatedCompletionDate));
    }
    if (!taskData.createdDate.empty())
    {
        // Override the auto date creation with the actual creation date.
        newTask->setCreationDate(newTask->stringToDate(taskData.createdDate));
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

static bool loadUserTaskestDataIntoDatabase(UserModel_shp userOne, ProgramOptions& programOptions)
{
    DBInterface TaskDBInterface(programOptions);
    bool allTestsPassed = true;
    std::size_t lCount = 0;
    std::vector<UserTaskTestData> userTaskTestData = loadTasksFromDataFile(programOptions.taskTestDataFile);;

    for (auto taskTestData: userTaskTestData)
    {
        // Try both constructors on an alternating basis.
        TaskModel_shp testTask = (lCount & 0x000001)? creatOddTask(userOne, taskTestData) : creatEvenTask(userOne, taskTestData);

        if (!TaskDBInterface.insertIntoDataBase(*testTask))
        {
            std::cerr << TaskDBInterface.getAllErrorMessages() << *testTask << "\n";
            allTestsPassed = false;
        }
        else
        {
            if (testTask->isInDataBase())
            {
                if (!testGetTaskByDescription(TaskDBInterface, *testTask, programOptions.verboseOutput))
                {
                    allTestsPassed = false;
                }
            }
            else
            {
                std::clog << "Primary key for task: " << testTask->getPrimaryKey() << ", " << testTask->getDescription() <<
                " not set!\n";
                if (programOptions.verboseOutput)
                {
                    std::clog << *testTask << "\n\n";
                }
                allTestsPassed = false;
            }
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
            UserList userList = loadUserProfileTestDataIntoDatabase(programOptions);
            if (userList.size())
            {
                if (!loadUserTaskestDataIntoDatabase(userList[0], programOptions))
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

