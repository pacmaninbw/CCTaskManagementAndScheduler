// Project Header Files
#include "ListDBInterface.h"
#include "UserList.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>

UserList::UserList()
: ListDBInterface<UserModel>()
{

}

UserListValues UserList::getAllUsers()
{
    errorMessages.clear();
    
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
