#include <chrono>
#include <format>
#include <iostream>
#include "ListDBInterface.h"
#include "UserGoalList.h"
#include "UserGoalModel.h"

UserGoalList::UserGoalList()
: ListDBInterface<UserGoalModel>()
{

}

UserGoalListValues UserGoalList::getAllGoalsForUser(std::size_t userID)
{
    prepareForRunQueryAsync();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In UserGoalList::getActiveGoalsForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator.formatSelectAllByUserId(userID);
        return runQueryFillUserGoalList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return UserGoalListValues();
}

UserGoalListValues UserGoalList::getAllChildrenFromParent(UserGoalModel &parentGoal)
{
    prepareForRunQueryAsync();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In UserGoalList::getActiveGoalsForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator.formatSelectAllChildGoalsWithParent(parentGoal);
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
            queryGenerator.getAllErrorMessages()));
        return UserGoalListValues();
    }
    if (runFirstQuery())
    {
        return fillUserGoalList();
    }

    return UserGoalListValues();
}

