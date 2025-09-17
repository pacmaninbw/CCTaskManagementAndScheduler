#ifndef USERLIST_H_
#define USERLIST_H_

#include <format>
#include <iostream>
#include "ListDBInterface.h"
#include "UserModel.h"

using UserListValues = std::vector<UserModel_shp>;

class UserList : public ListDBInterface<UserModel>
{
public:
    UserList();
    virtual ~UserList() = default;
    UserListValues getAllUsers();

private:

};

#endif // USERLIST_H_

