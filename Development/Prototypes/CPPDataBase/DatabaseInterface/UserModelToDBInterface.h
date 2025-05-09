#ifndef USERMODELTODBINTERFACE_H_
#define USERMODELTODBINTERFACE_H_

#include "DBInterface.h"
#include "UserModel.h"

class UserModelToDBInterface :
    public DBInterface
{
public:
    UserModelToDBInterface();
    ~UserModelToDBInterface();
    void addUserToDB(UserModel& user);

private:

};

#endif // USERMODELTODBINTERFACE_H_