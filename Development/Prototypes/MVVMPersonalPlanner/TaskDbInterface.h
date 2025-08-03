#ifndef TASKDBINTERFACE_H_
#define TASKDBINTERFACE_H_

#include "CommandLineParser.h"
#include "commonUtilities.h"
#include "DBInterface.h"
#include <functional>
#include <optional>
#include <string_view>
#include "TaskModel.h"

class TaskDbInterface : public DBInterface
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
    NSBA::awaitable<NSBM::results> coRoSelectTaskById(const std::size_t taskId);
    NSBA::awaitable<NSBM::results> coRoSelectTaskDependencies(const std::size_t taskId);
    void addDependencies(TaskModel_shp newTask);
    NSBA::awaitable<NSBM::results> coRoSelectTaskByDescriptionAndAssignedUser(std::string_view description, const std::size_t userID);
    NSBA::awaitable<NSBM::results> coRoSelecUnstartedDueForStartForAssignedUsert(std::size_t userID, std::chrono::year_month_day searchStart);

};

#endif // TASKDBINTERFACE_H_
