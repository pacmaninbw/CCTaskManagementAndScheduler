#ifndef TASKMODELTODBINTERFACE_H_
#define TASKMODELTODBINTERFACE_H_

#include "DBInterface.h"
#include "TaskModel.h"

class TaskModelToDBInterface :
    public DBInterface
{
public:
    TaskModelToDBInterface();
    ~TaskModelToDBInterface();
    void addTaskToDatabase(TaskModel& task);

private:
    bool isValidDataForAddToDatabase(TaskModel& task);

};

#endif // TASKMODELTODBINTERFACE_H_