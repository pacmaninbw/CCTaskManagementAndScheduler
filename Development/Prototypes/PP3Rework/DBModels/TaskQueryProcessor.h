#ifndef TASKQUERYPROCESSOR_H_
#define TASKQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>

using TaskList = std::vector<TaskModel_shp>;

class TaskQueryProcessor : public QueryProcessor<TaskModel>
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
    virtual TaskModel_shp processResultRow(boost::mysql::row_view& queryRow) override;
    virtual void fillRequiredIndexes() override;

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetByTaskID() noexcept;
    TestStatus testExceptionGetByDescriptionAndAssignedUser() noexcept;
    TestStatus testExceptionGetActiveTasksForAssignedUser() noexcept;
    TestStatus testExceptionGetUnstartedDueForStartForAssignedUser() noexcept;
    TestStatus testExceptionGetTasksCompletedByAssignedAfterDate() noexcept;
    TestStatus testExceptionGetTasksByAssignedIDandParentID() noexcept;
    TestStatus testExceptionGetDefaultDashboardTaskList() noexcept;

    std::size_t taskIdIdx = IndexNotSet;
    std::size_t createdByIdx = IndexNotSet;
    std::size_t assignedToIdx = IndexNotSet;
    std::size_t descriptionIdx = IndexNotSet;
    std::size_t parentTaskIdx = IndexNotSet;
    std::size_t statusIdx = IndexNotSet;
    std::size_t createdOnIdx = IndexNotSet;
    std::size_t requiredDeliveryIdx = IndexNotSet;
    std::size_t scheduledStartIdx = IndexNotSet;
    std::size_t actualStartIdx = IndexNotSet;
    std::size_t estimatedCompletionIdx = IndexNotSet;
    std::size_t completedIdx = IndexNotSet;
    std::size_t estimatedEffortHoursIdx = IndexNotSet;
    std::size_t actualEffortHoursIdx = IndexNotSet;
    std::size_t schedulePriorityGroupIdx = IndexNotSet;
    std::size_t priorityInGroupIdx = IndexNotSet;
    std::size_t personalIdx = IndexNotSet;
    std::size_t dependencyCountIdx = IndexNotSet;
    std::size_t depenedenciesTextIdx = IndexNotSet;
    std::size_t lastUpdate_Idx = IndexNotSet;
    std::size_t hidden_Idx = IndexNotSet;
};

#endif // TASKQUERYPROCESSOR_H_

