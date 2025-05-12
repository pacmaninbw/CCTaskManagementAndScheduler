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

protected:
    bool ModelObjectHasAllRequiredFields(ModelBase* modelObject) override;
    bool addDataToSqlStatement(ModelBase* modelObject) override;

private:

};

#endif // TASKMODELTODBINTERFACE_H_


