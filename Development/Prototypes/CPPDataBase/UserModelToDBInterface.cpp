#include <exception>
#include <stdexcept>
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

void UserModelToDBInterface::addDataToSqlStatement(ModelBase *modelObject)
{
    UserModel* user = dynamic_cast<UserModel*>(modelObject);
    if (!user)
    {
        std::runtime_error badObject(
            "PROGRAMMER ERROR: In UserModelToDBInterface::addDataToSqlStatement model object is not user model!");
        throw badObject;
    }

    appendArgToSqlStmt(user->getLastName(), true);
    appendArgToSqlStmt(user->getFirstName(), true);
    appendArgToSqlStmt(user->getMiddleInitial(), true);
    appendArgToSqlStmt(user->getLoginName(), true);
    appendArgToSqlStmt(user->getPassword());
}
