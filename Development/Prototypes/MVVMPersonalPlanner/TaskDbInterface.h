#ifndef TASKDBINTERFACE_H_
#define TASKDBINTERFACE_H_

#include "CommandLineParser.h"
#include "DBInterface.h"
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

private:
    TaskModel_shp processResult(boost::mysql::results& results);
    TaskList processResults(boost::mysql::results& results);
    void processResultRow(boost::mysql::row_view rv, TaskModel_shp newTask);
    boost::asio::awaitable<boost::mysql::results> coRoInsertTask(TaskModel& task);
    std::optional<boost::mysql::date> optionalDateConversion(std::optional<std::chrono::year_month_day> optDate);
    boost::asio::awaitable<boost::mysql::results> selectTaskById(const std::size_t taskId);
    boost::asio::awaitable<boost::mysql::results> selectTaskDependencies(const std::size_t taskId);
    void addDependencies(TaskModel_shp newTask);
    boost::asio::awaitable<boost::mysql::results> selectTaskByDescriptionAndAssignedUser(std::string_view description, const std::size_t userID);
};

#endif // TASKDBINTERFACE_H_
