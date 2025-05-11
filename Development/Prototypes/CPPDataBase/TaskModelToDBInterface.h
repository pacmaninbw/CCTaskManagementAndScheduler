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

protected:
    bool ModelObjectHasAllRequiredFields(ModelBase* modelObject) override;

private:

};

#endif // TASKMODELTODBINTERFACE_H_