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

    std::size_t m_taskIdIdx = IndexNotSet;
    std::size_t m_creatorIdx = IndexNotSet;
    std::size_t m_assigneeIdx = IndexNotSet;
    std::size_t m_descriptionIdx = IndexNotSet;
    std::size_t m_parentIdx = IndexNotSet;
    std::size_t m_statusIdx = IndexNotSet;
    std::size_t m_createdIdx = IndexNotSet;
    std::size_t m_dueDateIdx = IndexNotSet;
    std::size_t m_planedStartIdx = IndexNotSet;
    std::size_t m_actualStartIdx = IndexNotSet;
    std::size_t m_planedEndIdx = IndexNotSet;
    std::size_t m_completedIdx = IndexNotSet;
    std::size_t m_estimatedEffortIdx = IndexNotSet;
    std::size_t m_efforToDateIdx = IndexNotSet;
    std::size_t m_priorityCategoryIdx = IndexNotSet;
    std::size_t m_priorityIdx = IndexNotSet;
    std::size_t m_personalIdx = IndexNotSet;
    std::size_t m_dependencyCountIdx = IndexNotSet;
    std::size_t m_depenedencyListIdx = IndexNotSet;
    std::size_t m_lastUpdateIdx = IndexNotSet;
    std::size_t m_hiddenIdx = IndexNotSet;
};

#endif // TASKQUERYPROCESSOR_H_

