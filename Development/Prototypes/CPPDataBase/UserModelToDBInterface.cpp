#include <iostream>
#include "UserModelToDBInterface.h"

UserModelToDBInterface::UserModelToDBInterface()
: DBInterface("UserProfile", "addNewUser")
{

}

UserModelToDBInterface::~UserModelToDBInterface()
{

}

bool UserModelToDBInterface::ModelObjectHasAllRequiredFields(ModelBase* modelObject)
{
    UserModel* user = dynamic_cast<UserModel*>(modelObject);
    if (!user)
    {
        appendErrorMessage("PROGRAMMER ERROR: In UserModelToDBInterface::addDataToSqlStatement model object is not user model!\n");
        return false;
    }

    bool isValid = user->allRequiredFieldsHaveData();

    if (!isValid)
    {
        appendErrorMessage(user->reportMissingRequiredFields());
    }

    return isValid;
}

bool UserModelToDBInterface::addDataToSqlStatement(ModelBase *modelObject)
{
    UserModel* user = dynamic_cast<UserModel*>(modelObject);
    if (!user)
    {
        appendErrorMessage(
            "PROGRAMMER ERROR: In UserModelToDBInterface::addDataToSqlStatement model object is not user model!");
        return false;
    }

    appendArgToSqlStmt(user->getLastName(), true);
    appendArgToSqlStmt(user->getFirstName(), true);
    appendArgToSqlStmt(user->getMiddleInitial(), true);
    appendArgToSqlStmt(user->getLoginName(), true);
    appendArgToSqlStmt(user->getPassword());

    return true;
}


