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
