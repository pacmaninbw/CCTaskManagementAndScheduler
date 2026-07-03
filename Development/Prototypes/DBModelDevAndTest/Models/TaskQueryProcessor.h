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

    std::size_t m_TaskIdIdx = IndexNotSet;
    std::size_t m_CreatedByIdx = IndexNotSet;
    std::size_t m_AssignedToIdx = IndexNotSet;
    std::size_t m_DescriptionIdx = IndexNotSet;
    std::size_t m_ParentIdx = IndexNotSet;
    std::size_t m_StatusIdx = IndexNotSet;
    std::size_t m_CreatedOnIdx = IndexNotSet;
    std::size_t m_DueDateIdx = IndexNotSet;
    std::size_t m_PlanedStartIdx = IndexNotSet;
    std::size_t m_ActualStartIdx = IndexNotSet;
    std::size_t m_PlanedEndIdx = IndexNotSet;
    std::size_t m_CompletedIdx = IndexNotSet;
    std::size_t m_EstEffortHoursIdx = IndexNotSet;
    std::size_t m_EfforToDateIdx = IndexNotSet;
    std::size_t m_PriorityCategoryIdx = IndexNotSet;
    std::size_t m_PriorityIdx = IndexNotSet;
    std::size_t m_PersonalIdx = IndexNotSet;
    std::size_t m_DependencyCountIdx = IndexNotSet;
    std::size_t m_DepenedencyListIdx = IndexNotSet;
    std::size_t m_LastUpdateIdx = IndexNotSet;
    std::size_t m_HiddenIdx = IndexNotSet;
};

#endif // TASKQUERYPROCESSOR_H_

