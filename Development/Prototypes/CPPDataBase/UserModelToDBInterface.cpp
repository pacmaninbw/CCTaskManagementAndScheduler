#include "UserModelToDBInterface.h"

UserModelToDBInterface::UserModelToDBInterface()
: DBInterface("UserProfile", "addNewUser")
{

}

UserModelToDBInterface::~UserModelToDBInterface()
{

}

void UserModelToDBInterface::addUserToDB(UserModel &user)
{
    startAddStmt();

    appendArgToSqlStmt(user.getLastName(), true);
    appendArgToSqlStmt(user.getFirstName(), true);
    appendArgToSqlStmt(user.getMiddleInitial(), true);
    appendArgToSqlStmt(user.getLoginName(), true);
    appendArgToSqlStmt(user.getPassword());
    
    sqlStatement += ")";

    asyncExecutionSqlStatment(sqlStatement);
}

bool UserModelToDBInterface::ModelObjectHasAllRequiredFields(ModelBase* modelObject)
{
    UserModel* user = dynamic_cast<UserModel*>(modelObject);
    if (!user)
    {
        return false;
    }

    bool isValid = true;

    if (requiredKeyHasValue(user->getUserID()))
    {
        isValid = false;    // Can't add the data because it is alreay in the database
    }

    if (!requiredFieldHaseData(user->getLastName()))
    {
        isValid = false;
    }

    if (!requiredFieldHaseData(user->getFirstName()))
    {
        isValid = false;
    }

    if (!requiredFieldHaseData(user->getLoginName()))
    {
        isValid = false;
    }

    if (!requiredFieldHaseData(user->getPassword()))
    {
        isValid = false;
    }

    return isValid;
}
