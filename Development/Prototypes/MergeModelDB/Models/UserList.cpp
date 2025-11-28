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

UserListValues UserList::getAllUsers() noexcept
{
    errorMessages.clear();
    
    UserListValues allUsers;

    try
    {
        firstFormattedQuery = queryGenerator->formatGetAllUsersQuery();
        if (firstFormattedQuery.empty())
        {
            appendErrorMessage(std::format("Formatting getAllUser query string failed {}",
                queryGenerator->getAllErrorMessages()));
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

std::vector<ListExceptionTestElement> UserList::initListExceptionTests() noexcept 
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserList::testExceptionsGetAllUsers, this), "selectByUserID"});

    return exceptionTests;
}

TestStatus UserList::testExceptionsGetAllUsers() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("UserListSelfTest::testExceptionsGetAllUsers()", std::bind(&UserList::getAllUsers, this));
}

