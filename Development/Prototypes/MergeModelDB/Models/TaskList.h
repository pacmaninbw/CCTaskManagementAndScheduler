#ifndef TASKLIST_H_
#define TASKLIST_H_

// Project Header Files
#include "ListDBInterface.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>

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
    TaskListValues getDefaultDashboardTaskList(std::size_t assignedUserID,
        std::chrono::year_month_day searchStartDate) noexcept;

private:
    TaskListValues fillTaskList();
    TaskListValues runQueryFillTaskList();

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetActiveTasksForAssignedUser() noexcept;
    TestStatus testExceptionGetUnstartedDueForStartForAssignedUser() noexcept;
    TestStatus testExceptionGetTasksCompletedByAssignedAfterDate() noexcept;
    TestStatus testExceptionGetTasksByAssignedIDandParentID() noexcept;
    TestStatus testExceptionGetDefaultDashboardTaskList() noexcept;
};

#endif // TASKLIST_H_

