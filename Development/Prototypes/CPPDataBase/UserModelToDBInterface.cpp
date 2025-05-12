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

    bool isValid = true;

    reportIfError(requiredKeyHasValue(user->getUserID()), "User ID alreay has a value, the user is in the database\n", isValid);
    reportIfError(!requiredFieldHaseData(user->getLastName()), "Missing required last name value.\n", isValid);
    reportIfError(!requiredFieldHaseData(user->getFirstName()), "Missing required first name value.\n", isValid);
    reportIfError(!requiredFieldHaseData(user->getLoginName()), "Missing required login name value.\n", isValid);
    reportIfError(!requiredFieldHaseData(user->getPassword()), "Missing required password value.\n", isValid);

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


