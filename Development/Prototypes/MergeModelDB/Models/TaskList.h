#ifndef TASKLIST_H_
#define TASKLIST_H_

// Project Header Files
#include "ListDBInterface.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

using TaskListValues = std::vector<TaskModel_shp>;

class TaskList : public ListDBInterface<TaskModel>
{
public:
    TaskList();
    virtual ~TaskList() = default;

    TaskListValues getActiveTasksForAssignedUser(std::size_t assignedUserID) noexcept;
    TaskListValues getUnstartedDueForStartForAssignedUser(std::size_t assignedUserID) noexcept;
    TaskListValues getTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
        std::chrono::year_month_day searchStartDate) noexcept;
    TaskListValues getTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID) noexcept;

private:
    TaskListValues fillTaskList();
    TaskListValues runQueryFillTaskList();

};

#endif // TASKLIST_H_

