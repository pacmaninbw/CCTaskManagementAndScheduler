#include <iostream>
#include <string>
#include <vector>
#include "TaskModel.h"
#include "TaskModelToDBInterface.h"
#include "UserModel.h"
#include "UserTaskTestData.h"

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

/*
Task ID", "Priority Major", "Priority Minor", "Description", "Due Date", "Estimated Effort in Hours", "Actual Effort in Hours", "Parent Task", "Dependencies", "Status", "Related Goals", "Scheduled Start Date", "Actual Start Date", "Created Date", "Requested Completion Date", "Estimated Completion Date
1, 1, 1, "Archive BHHS74Reunion website to external SSD", "2025-05-05", 12, 1.0, 0, "", "Work in Progress", "5", "2025-04-08", "2025-04-08", "2025-04-01", "2025-06-30", "2025-05-05"
2, 1, 2, "Develop Personal Planning Tool", "2025-05-05", 300, 40.0, 0, "", "", "3,  6, 7, 16", "2025-03-20", "2025-03-20", "2025-04-01", "2025-04-30", "2025-05-15"
3, 1, 1, "Check with GoDaddy about providing service to archive website to external SSD", "2025-04-15", 2, 1.0, 1, "", "Work in Progress", "5", "", "", "", "", "
4, 2, 2, "Install a WordPress Archive Plugin", "2025-05-01", 1, 0.0, 1, "3", "Not Started", "5", "2025-05-01", "", "2025-04-10", "2025-06-30", "2025-05-01"
5, 2, 3, "Have GoDaddy install PHPMyAdmin", "2025-05-02", 1, 0.0, 1, "3", "Not Started", "5", "2025-05-02", "", "2025-04-10", "2025-06-30", "2025-05-03"
6, 2, 4, "Run Archive Plugin", "2025-05-03", 2, 0.0, 1, "3", "Not Started", "5", "2025-05-03", "", "2025-04-10", "2025-06-30", "2025-05-03"
7, 2, 5, "Log into PHPMyAdmin and save Database to disk", "2025-05-04", 1, 0.0, 1, "3", "Not Started", "5", "2025-05-04", "", "2025-04-10", "2025-06-30", "2025-05-04"
8, 2, 6, "FTP all website files to computer", "2025-05-05", 3, 0.0, 1, "3", "Not Started", "5", "2025-05-05", "", "2025-04-10", "2025-06-30", "2025-05-05"
9, 2, 7, "Store all website files on external SSD", "2025-05-05", 2, 0.0, 1, "3, 4, 5, 6, 7, 8", "Not Started", "5", "2025-05-05", "", "2025-04-10", "2025-06-30", "2025-05-05"
10, 1, 0, "Design Database for Personal Planner", "2025-04-08", 24, 16.0, 2, "", "Work in Progress", "3,  6, 7, 16", "2025-03-20", "2025-03-20", "2025-04-10", "2025-04-08", "2025-05-01"
11, 1, 0, "Design UML Diagram for Personal Planner", "2025-04-12", 16, 5.25, 2, "10", "Work in Progress", "3,  6, 7, 16", "2025-04-08", "2025-04-08", "2025-04-10", "2025-04-15", "2025-04-15"
12, 3, 1, "Grocery Shopping", "2025-04-12", 2, 2, 0, "", "Not Started", "", "2025-04-11", "", "2025-04-11", "2025-04-11", ""
13, 3, 2, "Daily 30 minute walk", "2025-04-11", 1, 0.0, 0, "", "Not Started", "", "2025-04-11", "", "2025-04-11", "2056-07-07", "2056-07-07"
14, 3, 3, "Clean apartment", "2025-04-12", 2, 0.0, 0, "", "Not Started", "", "2025-04-12", "", "2025-04-11", "2025-04-12", ""
15, 1, 2, "Create Test Data For personal planner database", "2025-04-13", 4.0, 0, "2", "", "Not Started", "", "2025-04-12", "", "2025-04-12", "2025-04-13", "2025-04-13"
16, 2, 1, "Write Stored Procedures to access personal planner database", "2025-04-15", 8, 9.25, 2, "", "Work in Progress", "", "2025-04-13", "", "2025-04-12", "2025-04-15", "2025-04-25"
17, 3, 3, "Get Laser Printer", "2025-04-15", 2, 0.0, 0, "", "Not Started", "", "2025-04-15", "", "2025-04-12", "2025-04-15", "2025-04-15"
18, 2, 2, "Write C++ database access prototype", "2025-04-17", 30, 8.25, 2, "", "Not Started", "", "2025-04-14", "", "2025-04-12", "2025-04-17", "2025-04-18"
19, 1, 2, "Moderate the stackexchange code review site", "2030-05-01", 365, 30, 0, "", "Work in Progress", "", "2024-04-01", "", "2025-04-13", "2030-05-01", "2030-05-01"

 */

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
}
 ;

void loadUserTaskestDataIntoDatabase()
{
    TaskModelToDBInterface TaskDBInterface;

    UserModel testUser("Chernick", "Paul", "A", "paul.chernick@chernicksw.com");
    testUser.dbSetUserId(1);

    for (auto taskTestData: userTaskTestData)
    {
        TaskModel testTask(testUser, taskTestData.description, taskTestData.estimatedEffortHours,
            taskTestData.dueDate, taskTestData.scheduledStartDate);

        if (!TaskDBInterface.addToDatabaseTable(&testTask))
        {
            std::cerr << testTask << "\n";
        }
    }
}


