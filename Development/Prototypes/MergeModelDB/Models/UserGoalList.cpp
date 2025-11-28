// Project Header Files
#include "ListDBInterface.h"
#include "UserGoalList.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>

UserGoalList::UserGoalList()
: ListDBInterface<UserGoalModel>()
{
}

UserGoalListValues UserGoalList::getAllGoalsForUser(std::size_t userID) noexcept
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In UserGoalList::getActiveGoalsForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectAllByUserId(userID);
        return runQueryFillUserGoalList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return UserGoalListValues();
}

UserGoalListValues UserGoalList::getAllChildrenFromParent(UserGoalModel parentGoal) noexcept
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In UserGoalList::getActiveGoalsForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectAllChildGoalsWithParent(parentGoal);
        return runQueryFillUserGoalList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return UserGoalListValues();
}

UserGoalListValues UserGoalList::fillUserGoalList()
{
    UserGoalListValues UserGoalList;

    for (auto GoalID: primaryKeyResults)
    {
        UserGoalModel_shp newGoal = std::make_shared<UserGoalModel>(UserGoalModel());
        newGoal->selectByGoalID(GoalID);
        UserGoalList.push_back(newGoal);
    }

    return UserGoalList;
}

UserGoalListValues UserGoalList::runQueryFillUserGoalList()
{
    if (firstFormattedQuery.empty())
    {
        appendErrorMessage(std::format("Formatting select multiple Goals query string failed {}",
            queryGenerator->getAllErrorMessages()));
        return UserGoalListValues();
    }
    if (runFirstQuery())
    {
        return fillUserGoalList();
    }

    return UserGoalListValues();
}

std::vector<ListExceptionTestElement> UserGoalList::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserGoalList::testExceptionsGetAllGoalsForUser, this), "getAllGoalsForUser"});
    exceptionTests.push_back({std::bind(&UserGoalList::testExceptionsGetAllChildrenFromParent, this), "selectAllChildGoalsWithParentFromUser"});

    return exceptionTests;
}

TestStatus UserGoalList::testExceptionsGetAllGoalsForUser() noexcept
{
   selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("UserGoalList::testExceptionsGetAllGoalsForUser()",
         std::bind(&UserGoalList::getAllGoalsForUser, this, std::placeholders::_1), 1);
}

TestStatus UserGoalList::testExceptionsGetAllChildrenFromParent() noexcept
{
   selfTestResetAllValues();

    UserGoalModel parent;
    parent.setGoalId(1);
    parent.setDescription("Get a Job in Software Engineering");
    parent.setPriority(1);

    return testListExceptionAndSuccessNArgs("UserGoalList::testExceptionsGetAllChildrenFromParent()",
         std::bind(&UserGoalList::getAllChildrenFromParent, this, std::placeholders::_1), parent);
}


