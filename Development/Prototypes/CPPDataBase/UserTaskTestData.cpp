#include "DBInterface.h"
#include <iostream>
#include <vector>
#include "UserTaskTestData.h"

void loadUserTaskestDataIntoDatabase(DBInterface& dbInterface)
{
    dbInterface.addUserTaskWithLoginName();
}
