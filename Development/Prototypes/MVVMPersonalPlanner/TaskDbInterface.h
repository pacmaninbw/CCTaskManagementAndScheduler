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
    std::size_t insert(const TaskModel& task);
    std::size_t insert(TaskModel_shp task) { return insert(*task); };
    TaskModel_shp getTaskByDescriptionAndAssignedUser(std::string_view description, UserModel& assignedUser); 

private:
    TaskModel_shp processResults(boost::mysql::results& results);
    boost::asio::awaitable<boost::mysql::results> coRoInsertTask(const TaskModel& task);
    std::optional<boost::mysql::date> optionalDateConversion(std::optional<std::chrono::year_month_day> optDate);

};

#endif // TASKDBINTERFACE_H_
