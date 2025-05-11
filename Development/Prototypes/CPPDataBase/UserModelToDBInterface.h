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

protected:
    bool ModelObjectHasAllRequiredFields(ModelBase* modelObject) override;
    void addDataToSqlStatement(ModelBase* modelObject) override;

private:

};

#endif // USERMODELTODBINTERFACE_H_


