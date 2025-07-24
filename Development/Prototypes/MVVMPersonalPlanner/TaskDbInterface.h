#ifndef TASKDBINTERFACE_H_
#define TASKDBINTERFACE_H_

#include "CommandLineParser.h"
#include "DBInterface.h"
#include <string_view>
#include "TaskModel.h"

class TaskDbInterface : public DBInterface
{
public:
    TaskDbInterface(ProgramOptions& programOptions);
    ~TaskDbInterface() = default;
    std::size_t insert(const TaskModel& task);
    std::size_t insert(TaskModel_shp task) { return insert(*task); };
    TaskModel_shp getTaskByDescriptionAndUserID(std::string_view description, std::size_t userID); 

private:

};

#endif // TASKDBINTERFACE_H_
