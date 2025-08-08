#ifndef TASKDBINTERFACE_H_
#define TASKDBINTERFACE_H_

#include "CommandLineParser.h"
#include "commonUtilities.h"
#include "BoostDBInterfaceCore.h"
#include <functional>
#include <optional>
#include <string_view>
#include "TaskModel.h"

class TaskDbInterface : public BoostDBInterfaceCore
{
public:
    TaskDbInterface();
    ~TaskDbInterface() = default;
    std::size_t insert(TaskModel& task);
    std::size_t insert(TaskModel_shp task) { return insert(*task); };
    TaskModel_shp getTaskByTaskID(std::size_t taskId);
    TaskModel_shp getTaskByDescriptionAndAssignedUser(std::string_view description, UserModel& assignedUser);
    TaskModel_shp getParentTask(TaskModel& task);
    TaskModel_shp getParentTask(TaskModel_shp task) { return getParentTask(*task); };
    TaskList getActiveTasksForAssignedUser(UserModel& assignedUser);
    TaskList getActiveTasksForAssignedUser(UserModel_shp assignedUser)
        { return getActiveTasksForAssignedUser(*assignedUser); };
    TaskList getUnstartedDueForStartForAssignedUser(UserModel& assignedUser);
    TaskList getUnstartedDueForStartForAssignedUser(UserModel_shp assignedUser)
        { return getUnstartedDueForStartForAssignedUser(*assignedUser); };
    TaskList getTasksCompletedByAssignedAfterDate(UserModel& assignedUser,
        std::chrono::year_month_day searchStartDate);
    TaskList getTasksCompletedByAssignedAfterDate(UserModel_shp assignedUser,
        std::chrono::year_month_day searchStartDate)
        { return getTasksCompletedByAssignedAfterDate(*assignedUser, searchStartDate); };

private:
    TaskModel_shp processResult(NSBM::results& results);
    TaskList processResults(NSBM::results& results);
    void processResultRow(NSBM::row_view rv, TaskModel_shp newTask);
    NSBA::awaitable<NSBM::results> coRoInsertTask(TaskModel& task);
    std::optional<NSBM::date> optionalDateConversion(std::optional<std::chrono::year_month_day> optDate);
    NSBA::awaitable<NSBM::results> coRoSelectTaskById();
    NSBA::awaitable<NSBM::results> coRoSelectTaskDependencies(const std::size_t taskId);
    void addDependencies(TaskModel_shp newTask);
    NSBA::awaitable<NSBM::results> coRoSelectTaskByDescriptionAndAssignedUser();
    NSBA::awaitable<NSBM::results> coRoSelectUnstartedDueForStartForAssignedUser();
    NSBA::awaitable<NSBM::results> coRoSelectTasksWithStatusForAssignedUserBefore();

private:
/*
 * The indexes below are based on the following select statement, maintain this order
 * for any new select statements, add any new field indexes at the end.
 *       "SELECT TaskID, CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
 *           "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
 *           "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup FROM Tasks WHERE TaskID = {0}",
 */
    const std::size_t taskIdIdx = 0;
    const std::size_t createdByIdx = 1;
    const std::size_t assignedToIdx = 2;
    const std::size_t descriptionIdx = 3;
    const std::size_t parentTaskIdx = 4;
    const std::size_t statusIdx = 5;
    const std::size_t percentageCompleteIdx = 6;
    const std::size_t createdOnIdx = 7;
    const std::size_t requiredDeliveryIdx = 8;
    const std::size_t scheduledStartIdx = 9;
    const std::size_t actualStartIdx = 10;
    const std::size_t estimatedCompletionIdx = 11;
    const std::size_t completedIdx = 12;
    const std::size_t estimatedEffortHoursIdx = 13;
    const std::size_t actualEffortHoursIdx = 14;
    const std::size_t schedulePriorityGroupIdx = 15;
    const std::size_t priorityInGroupIdx = 16;
    // Added fields
    const std::size_t personalIdx = 17;
    const std::size_t dependencyCountIdx = 18;
};

#endif // TASKDBINTERFACE_H_
