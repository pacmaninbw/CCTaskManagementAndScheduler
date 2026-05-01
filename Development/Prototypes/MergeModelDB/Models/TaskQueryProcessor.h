#ifndef TASKQUERYPROCESSOR_H_
#define TASKQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>

using TaskQueryProcessorValues = std::vector<TaskModel_shp>;

class TaskQueryProcessor : public QueryProcessor<TaskModel>
{
public:
    TaskQueryProcessor();
    virtual ~TaskQueryProcessor() = default;

    TaskQueryProcessorValues getActiveTasksForAssignedUser(std::size_t assignedUserID) noexcept;
    TaskQueryProcessorValues getUnstartedDueForStartForAssignedUser(std::size_t assignedUserID) noexcept;
    TaskQueryProcessorValues getTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
        std::chrono::year_month_day searchStartDate) noexcept;
    TaskQueryProcessorValues getTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID) noexcept;
    TaskQueryProcessorValues getDefaultDashboardTaskQueryProcessor(std::size_t assignedUserID,
        std::chrono::year_month_day searchStartDate) noexcept;

private:
    TaskQueryProcessorValues fillTaskQueryProcessor();
    TaskQueryProcessorValues runQueryFillTaskQueryProcessor();

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetActiveTasksForAssignedUser() noexcept;
    TestStatus testExceptionGetUnstartedDueForStartForAssignedUser() noexcept;
    TestStatus testExceptionGetTasksCompletedByAssignedAfterDate() noexcept;
    TestStatus testExceptionGetTasksByAssignedIDandParentID() noexcept;
    TestStatus testExceptionGetDefaultDashboardTaskQueryProcessor() noexcept;
};

#endif // TASKQUERYPROCESSOR_H_

