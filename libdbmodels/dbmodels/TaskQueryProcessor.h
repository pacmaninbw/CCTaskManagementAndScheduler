#ifndef TASKQUERYPROCESSOR_H_
#define TASKQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>

using TaskList = std::vector<TaskModel_shp>;
using StaticQueryTask = boost::mysql::static_results<boost::mysql::pfr_by_name<TaskDbQueryValues>>;

class TaskQueryProcessor : public QueryProcessor<TaskModel, TaskDbQueryValues>
{
public:
    TaskQueryProcessor();
    virtual ~TaskQueryProcessor() = default;

    TaskModel_shp getTaskByTaskID(std::size_t taskId) noexcept;
    TaskList getTaskByDescriptionAndAssignedUser(std::string_view description, std::size_t assignedUserID) noexcept;
    TaskList getActiveTasksForAssignedUser(std::size_t assignedUserID) noexcept;
    TaskList getUnstartedDueForStartForAssignedUser(std::size_t assignedUserID) noexcept;
    TaskList getTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
        std::chrono::year_month_day searchStartDate) noexcept;
    TaskList getTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID) noexcept;
    TaskList getDefaultDashboardTaskList(std::size_t assignedUserID,
        std::chrono::year_month_day searchStartDate) noexcept;

private:
    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetByTaskID() noexcept;
    TestStatus testExceptionGetByDescriptionAndAssignedUser() noexcept;
    TestStatus testExceptionGetActiveTasksForAssignedUser() noexcept;
    TestStatus testExceptionGetUnstartedDueForStartForAssignedUser() noexcept;
    TestStatus testExceptionGetTasksCompletedByAssignedAfterDate() noexcept;
    TestStatus testExceptionGetTasksByAssignedIDandParentID() noexcept;
    TestStatus testExceptionGetDefaultDashboardTaskList() noexcept;

};

#endif // TASKQUERYPROCESSOR_H_

