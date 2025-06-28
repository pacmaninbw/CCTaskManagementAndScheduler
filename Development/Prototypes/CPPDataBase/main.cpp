#include "DBInterface.h"
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "TaskModel.h"
#include "UserModel.h"

static void testGetUserByLoginName(DBInterface& userDBInterface, UserModel_shp user)
{
    UserModel_shp testInDB = userDBInterface.getUserByLogin(user->getLoginName());
    if (testInDB)
    {
        if (*testInDB == *user)
        {
            std::cout << "User:" << user->getLastName() << ", " << user->getFirstName() <<
                "Successfully iserted and retrieved from database\n";
        }
        else
        {
            std::cout << "Insertion and retrieval of User Failed\nInserted User:\n" <<
            *user << "\n" "Retreived User:\n" << *testInDB << "\n";
        }
    }
    else
    {
        std::cerr << "userDBInterface.getUserByLogin(user->getLoginName()) FAILED!\n";
        std::cerr << userDBInterface.getAllErrorMessages() << "\n";
    }
}

static void testGetUserByFullName(DBInterface& userDBInterface, UserModel_shp user)
{
    UserModel_shp testInDB = userDBInterface.getUserByFullName(user->getLastName(), user->getFirstName(), user->getMiddleInitial());
    if (testInDB)
    {
        if (*testInDB == *user)
        {
            std::cout << "User:" << user->getLastName() << ", " << user->getFirstName() <<
                "Successfully iserted and retrieved by full name from database\n";
        }
        else
        {
            std::cout << "Insertion and retrieval of User by full name Failed\nInserted User:\n" <<
            *user << "\n" "Retreived User:\n" << *testInDB << "\n";
        }
    }
    else
    {
        std::cerr << "userDBInterface.getUserByFullName() FAILED!\n";
        std::cerr << userDBInterface.getAllErrorMessages() << "\n";
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

    for (auto user: userProfileTestData)
    {
        if (!userDBInterface.insertIntoDataBase(*user))
        {
            std::cerr << userDBInterface.getAllErrorMessages() << "\n" << *user << "\n";
        }
        else
        {
            if (user->isInDataBase())
            {
                testGetUserByLoginName(userDBInterface, user);
                testGetUserByFullName(userDBInterface, user);
            }
            else
            {
                std::cout << "Primary key for user: " << user->getLastName() << ", " << user->getFirstName() <<
                " not set!\n";
            }
        }
    }

    return userProfileTestData;
}
struct UserTaskTestData
{
    unsigned int priorityInAllTasks;
    char majorPriority;
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

static void loadUserTaskestDataIntoDatabase(UserModel_shp userOne)
{
    std::vector<UserTaskTestData> userTaskTestData = 
    {
        {1, 1, 1, "Archive BHHS74Reunion website to external SSD", "2025-05-05", 12, 1.0, 0, "", "Work in Progress", "5", "2025-04-08", "2025-04-08", "2025-04-01", "2025-06-30", "2025-05-05"},
        {2, 1, 2, "Develop Personal Planning Tool", "2025-05-05", 300, 40.0, 0, "", "", "3,  6, 7, 16", "2025-03-20", "2025-03-20", "2025-04-01", "2025-04-30", "2025-05-15"},
        {3, 1, 1, "Check with GoDaddy about providing service to archive website to external SSD", "2025-04-15", 2, 1.0, 1, "", "Work in Progress", "5", "", "", "", "", ""},
        {4, 2, 2, "Install a WordPress Archive Plugin", "2025-05-01", 1, 0.0, 1, "3", "Not Started", "5", "2025-05-01", "", "2025-04-10", "2025-06-30", "2025-05-01"},
        {5, 2, 3, "Have GoDaddy install PHPMyAdmin", "2025-05-02", 1, 0.0, 1, "3", "Not Started", "5", "2025-05-02", "", "2025-04-10", "2025-06-30", "2025-05-03"},
        {6, 2, 4, "Run Archive Plugin", "2025-05-03", 2, 0.0, 1, "3", "Not Started", "5", "2025-05-03", "", "2025-04-10", "2025-06-30", "2025-05-03"},
        {7, 2, 5, "Log into PHPMyAdmin and save Database to disk", "2025-05-04", 1, 0.0, 1, "3", "Not Started", "5", "2025-05-04", "", "2025-04-10", "2025-06-30", "2025-05-04"},
        {8, 2, 6, "FTP all website files to computer", "2025-05-05", 3, 0.0, 1, "3", "Not Started", "5", "2025-05-05", "", "2025-04-10", "2025-06-30", "2025-05-05"},
        {9, 2, 7, "Store all website files on external SSD", "2025-05-05", 2, 0.0, 1, "3, 4, 5, 6, 7, 8", "Not Started", "5", "2025-05-05", "", "2025-04-10", "2025-06-30", "2025-05-05"},
        {10, 1, 0, "Design Database for Personal Planner", "2025-04-08", 24, 16.0, 2, "", "Work in Progress", "3,  6, 7, 16", "2025-03-20", "2025-03-20", "2025-04-10", "2025-04-08", "2025-05-01"},
        {11, 1, 0, "Design UML Diagram for Personal Planner", "2025-04-12", 16, 5.25, 2, "10", "Work in Progress", "3,  6, 7, 16", "2025-04-08", "2025-04-08", "2025-04-10", "2025-04-15", "2025-04-15"},
        {12, 3, 1, "Grocery Shopping", "2025-04-12", 2, 2, 0, "", "Not Started", "", "2025-04-11", "", "2025-04-11", "2025-04-11", ""},
        {13, 3, 2, "Daily 30 minute walk", "2025-04-11", 1, 0.0, 0, "", "Not Started", "", "2025-04-11", "", "2025-04-11", "2056-07-07", "2056-07-07"},
        {14, 3, 3, "Clean apartment", "2025-04-12", 2, 0.0, 0, "", "Not Started", "", "2025-04-12", "", "2025-04-11", "2025-04-12", ""}
    };

    DBInterface TaskDBInterface;

    for (auto taskTestData: userTaskTestData)
    {
        std::cout << "Creating task from \"" << taskTestData.description << "\" " << taskTestData.estimatedEffortHours << "\n";
        TaskModel testTask(userOne, taskTestData.description, taskTestData.estimatedEffortHours,
            taskTestData.dueDate, taskTestData.scheduledStartDate);


        if (!TaskDBInterface.insertIntoDataBase(testTask))
        {
            std::cerr << TaskDBInterface.getAllErrorMessages() << testTask << "\n";
        }
    }
}

int main()
{

    try {
        UserList userList = loadUserProfileTestDataIntoDatabase();
        loadUserTaskestDataIntoDatabase(userList[1]);
    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

