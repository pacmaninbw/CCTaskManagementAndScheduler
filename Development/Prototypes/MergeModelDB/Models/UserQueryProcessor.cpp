// Project Header Files
#include "QueryProcessor.h"
#include "UserQueryProcessor.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>

UserQueryProcessor::UserQueryProcessor()
: QueryProcessor<UserModel>()
{

}

UserQueryProcessorValues UserQueryProcessor::getAllUsers() noexcept
{
    errorMessages.clear();
    
    UserQueryProcessorValues allUsers;

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
        appendErrorMessage(std::format("In UserQueryProcessor::getAllUsers : {}", e.what()));
    }
    
    return allUsers;
}

std::vector<ListExceptionTestElement> UserQueryProcessor::initListExceptionTests() noexcept 
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserQueryProcessor::testExceptionsGetAllUsers, this), "selectByUserID"});

    return exceptionTests;
}

TestStatus UserQueryProcessor::testExceptionsGetAllUsers() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("UserQueryProcessorSelfTest::testExceptionsGetAllUsers()", std::bind(&UserQueryProcessor::getAllUsers, this));
}

