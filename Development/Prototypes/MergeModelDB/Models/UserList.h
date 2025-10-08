#ifndef USERLIST_H_
#define USERLIST_H_

// Project Header Files
#include "ListDBInterface.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>

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

