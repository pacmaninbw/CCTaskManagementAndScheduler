#ifndef TASKLIST_H_
#define TASKLIST_H_

#include <chrono>
#include <format>
#include <iostream>
#include "ListDBInterface.h"
#include "TaskModel.h"

using TaskListValues = std::vector<TaskModel_shp>;

class TaskList : public ListDBInterface<TaskModel>
{
public:
    TaskList();
    virtual ~TaskList() = default;

    TaskListValues getActiveTasksForAssignedUser(std::size_t assignedUserID);
    TaskListValues getUnstartedDueForStartForAssignedUser(std::size_t assignedUserID);
    TaskListValues getTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
        std::chrono::year_month_day& searchStartDate);
    TaskListValues getTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID);

private:
    TaskListValues fillTaskList();
    TaskListValues runQueryFillTaskList();

};

#endif // TASKLIST_H_

