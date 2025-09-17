#include <format>
#include <iostream>
#include "ListDBInterface.h"
#include "UserList.h"
#include "UserModel.h"

UserList::UserList()
: ListDBInterface<UserModel>()
{

}

UserListValues UserList::getAllUsers()
{
    prepareForRunQueryAsync();
    UserListValues allUsers;

    try
    {
        firstFormattedQuery = queryGenerator.formatGetAllUsersQuery();
        if (firstFormattedQuery.empty())
        {
            appendErrorMessage(std::format("Formatting getAllUser query string failed {}",
                queryGenerator.getAllErrorMessages()));
            return allUsers;
        }
        if (runFirstQuery())
        {
            for (auto userID: primaryKeyResults)
            {
                UserModel_shp newUser = std::make_shared<UserModel>(UserModel());
                newUser->selectByUserID(userID);
                allUsers.push_back(newUser);
            }
        }
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserList::getAllUsers : {}", e.what()));
    }
    
    return allUsers;
}
