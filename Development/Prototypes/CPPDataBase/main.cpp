#include "DBInterface.h"
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "TaskModel.h"
#include "UserModel.h"

static bool testGetUserByLoginName(DBInterface& userDBInterface, UserModel_shp insertedUser)
{
    UserModel_shp retrievedUser = userDBInterface.getUserByLogin(insertedUser->getLoginName());
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

static bool testGetUserByFullName(DBInterface& userDBInterface, UserModel_shp insertedUser)
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

static UserList loadUserProfileTestDataIntoDatabase()
{

    UserList userProfileTestData = 
    {
        {std::make_shared<UserModel>("Chernick", "Paul", "A", "paul.chernick@chernicksw.com")},
        {std::make_shared<UserModel>("Chernick", "Nina", "L", "ChernickNinaL@gmail.com")},
        {std::make_shared<UserModel>("Chernick", "Dina", "B", "ChernickDinaB@gmail.com")},
        {std::make_shared<UserModel>("Shiminovics", "Eitan", "I", "ShimonvicsEitanI@gmail.com")},
        {std::make_shared<UserModel>("PacMan", "IN", "BW", "pacmaninbw@gmail.com")},
        {std::make_shared<UserModel>("Black", "Patrick", "A", "BlackPatrickA@gmail.com")}
    };

    DBInterface userDBInterface;
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
                std::cout << "Primary key for user: " << user->getLastName() << ", " << user->getFirstName() <<
                " not set!\n";
                allTestsPassed = false;
            }
        }
    }

    if (allTestsPassed)
    {
        std::cout << "Insertion and retrieval of users test PASSED\n";
    }
    else
    {
        userProfileTestData.clear();
    }
    return userProfileTestData;
}

static bool testGetTaskByDescription(DBInterface& taskDBInterface, TaskModel& task)
{
    TaskModel_shp testInDB = taskDBInterface.getTaskByDescription(task.getDescription());
    if (testInDB)
    {
        if (*testInDB == task)
        {
            return true;
        }
        else
        {
            std::cout << "Inserted and retrieved Task are not the same! Test FAILED!\nInserted Task:\n" <<
            task << "\n" "Retreived Task:\n" << *testInDB << "\n";
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
    const char majorPriority;
    unsigned int minorPriority;
    std::string description;
    std::string dueDate;
    unsigned int estimatedEffortHours;
    double actualEffortHours;
    std::size_t parentTask;
    std::string dependencies;
    std::string status;
    std::string relatedGoals;
    std::string scheduledStartDate;
    std::string actualStartDate;
    std::string createdDate;
    std::string dueDate2;
    std::string estimatedCompletionDate;
};

static std::vector<UserTaskTestData> userTaskTestData = 
{
    {'A', 1, "Archive BHHS74Reunion website to external SSD", "2025-05-05", 12, 1.0, 0, "", "Work in Progress", "5", "2025-04-08", "2025-04-08", "2025-04-01", "2025-06-30", "2025-05-05"},
    {'A', 2, "Develop Personal Planning Tool", "2025-05-05", 300, 40.0, 0, "", "Work in Progress", "3,  6, 7, 16", "2025-03-20", "2025-03-20", "2025-04-01", "2025-04-30", "2025-05-15"},
    {'A', 1, "Check with GoDaddy about providing service to archive website to external SSD", "2025-04-15", 2, 1.0, 1, "", "Work in Progress", "5", "", "", "", "", ""},
    {'B', 2, "Install a WordPress Archive Plugin", "2025-05-01", 1, 0.0, 1, "3", "Not Started", "5", "2025-05-01", "", "2025-04-10", "2025-06-30", "2025-05-01"},
    {'B', 3, "Have GoDaddy install PHPMyAdmin", "2025-05-02", 1, 0.0, 1, "3", "Not Started", "5", "2025-05-02", "", "2025-04-10", "2025-06-30", "2025-05-03"},
    {'B', 4, "Run Archive Plugin", "2025-05-03", 2, 0.0, 1, "3", "Not Started", "5", "2025-05-03", "", "2025-04-10", "2025-06-30", "2025-05-03"},
    {'B', 5, "Log into PHPMyAdmin and save Database to disk", "2025-05-04", 1, 0.0, 1, "3", "Not Started", "5", "2025-05-04", "", "2025-04-10", "2025-06-30", "2025-05-04"},
    {'B', 6, "FTP all website files to computer", "2025-05-05", 3, 0.0, 1, "3", "Not Started", "5", "2025-05-05", "", "2025-04-10", "2025-06-30", "2025-05-05"},
    {'B', 7, "Store all website files on external SSD", "2025-05-05", 2, 0.0, 1, "3, 4, 5, 6, 7, 8", "Not Started", "5", "2025-05-05", "", "2025-04-10", "2025-06-30", "2025-05-05"},
    {'A', 0, "Design Database for Personal Planner", "2025-04-08", 24, 16.0, 2, "", "Work in Progress", "3,  6, 7, 16", "2025-03-20", "2025-03-20", "2025-04-10", "2025-04-08", "2025-05-01"},
    {'A', 0, "Design UML Diagram for Personal Planner", "2025-04-12", 16, 5.25, 2, "10", "Work in Progress", "3,  6, 7, 16", "2025-04-08", "2025-04-08", "2025-04-10", "2025-04-15", "2025-04-15"},
    {'C', 1, "Grocery Shopping", "2025-04-12", 2, 2, 0, "", "Not Started", "", "2025-04-11", "", "2025-04-11", "2025-04-11", ""},
    {'C', 2, "Daily 30 minute walk", "2025-04-11", 1, 0.0, 0, "", "Not Started", "", "2025-04-11", "", "2025-04-11", "2056-07-07", "2056-07-07"},
    {'C', 3, "Clean apartment", "2025-04-12", 2, 0.0, 0, "", "Not Started", "", "2025-04-12", "", "2025-04-11", "2025-04-12", ""}
};

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

static bool loadUserTaskestDataIntoDatabase(UserModel_shp userOne)
{
    DBInterface TaskDBInterface;
    bool allTestsPassed = true;
    std::size_t lCount = 0;

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
                if (!testGetTaskByDescription(TaskDBInterface, *testTask))
                {
                    allTestsPassed = false;
                }
            }
            else
            {
                std::cout << "Primary key for task: " << testTask->getPrimaryKey() << ", " << testTask->getDescription() <<
                " not set!\n";
                allTestsPassed = false;
            }
        }
        ++lCount;
    }

    std::cout << "All Task insertions and retrival tests PASSED\n";
    return allTestsPassed;
}

int main()
{
    try {
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
        std::cout << "All tests Passed\n";
    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

