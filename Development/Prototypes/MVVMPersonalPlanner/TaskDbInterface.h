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
    TaskList getAllCurrentActiveTasksForAssignedUser(UserModel& assignedUser);
    TaskList getAllCurrentActiveTasksForAssignedUser(UserModel_shp assignedUser)
        { return getAllCurrentActiveTasksForAssignedUser(*assignedUser); };
    TaskList getUnstartedDueForStartForAssignedUser(UserModel& assignedUser);
    TaskList getUnstartedDueForStartForAssignedUser(UserModel_shp assignedUser)
        { return getUnstartedDueForStartForAssignedUser(*assignedUser); };
    TaskList getAllRecentCompletedTasksForAssignedUser(UserModel& assignedUser,
        std::chrono::year_month_day searchStartDate);
    TaskList getAllRecentCompletedTasksForAssignedUser(UserModel_shp assignedUser,
        std::chrono::year_month_day searchStartDate)
        { return getAllRecentCompletedTasksForAssignedUser(*assignedUser, searchStartDate); };

private:
    TaskModel_shp processResult(bMysql::results& results);
    TaskList processResults(bMysql::results& results);
    void processResultRow(bMysql::row_view rv, TaskModel_shp newTask);
    bAsio::awaitable<bMysql::results> coRoInsertTask(TaskModel& task);
    std::optional<bMysql::date> optionalDateConversion(std::optional<std::chrono::year_month_day> optDate);
    bAsio::awaitable<bMysql::results> coRoSelectTaskById(const std::size_t taskId);
    bAsio::awaitable<bMysql::results> coRoSelectTaskDependencies(const std::size_t taskId);
    void addDependencies(TaskModel_shp newTask);
    bAsio::awaitable<bMysql::results> coRoSelectTaskByDescriptionAndAssignedUser(std::string_view description, const std::size_t userID);
    bAsio::awaitable<bMysql::results> coRoSelecUnstartedDueForStartForAssignedUsert(std::size_t userID, std::chrono::year_month_day searchStart);

};

#endif // TASKDBINTERFACE_H_
