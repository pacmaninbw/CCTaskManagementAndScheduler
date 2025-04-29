#ifndef USERTASKTESTDATA_H_
#define USERTASKTESTDATA_H_

#include <string_view>


struct UserTaskTestData
{
    std::string_view userLoginName;
    unsigned int priorityInAllTasks;
    char majorPriority;
    unsigned int minorPriority;
    std::string_view descrption;
    std::string_view dueDate;
    unsigned int estimatedEffortHours;
    double actualEffortHours;
    std::string_view parentTaskDescription;
};

#endif // USERTASKTESTDATA_H_
