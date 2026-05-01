// Project Header Files
#include "QueryProcessor.h"
#include "GoalQueryProcessor.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>

GoalQueryProcessor::GoalQueryProcessor()
: QueryProcessor<UserGoalModel>()
{
}

GoalQueryProcessorValues GoalQueryProcessor::getAllGoalsForUser(std::size_t userID) noexcept
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In GoalQueryProcessor::getActiveGoalsForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectAllByUserId(userID);
        return runQueryFillGoalQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return GoalQueryProcessorValues();
}

GoalQueryProcessorValues GoalQueryProcessor::getAllChildrenFromParent(UserGoalModel parentGoal) noexcept
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In GoalQueryProcessor::getActiveGoalsForAssignedUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectAllChildGoalsWithParent(parentGoal);
        return runQueryFillGoalQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return GoalQueryProcessorValues();
}

GoalQueryProcessorValues GoalQueryProcessor::findGoalsByUserIdAndSimilarDescription(std::size_t userID, std::string searchString) noexcept
{
    errorMessages.clear();

    appendErrorMessage("In GoalQueryProcessor::findGoalsByUserIdAndSimilarDescription : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectBySimilarDescription(searchString, userID);
        return runQueryFillGoalQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return GoalQueryProcessorValues();
}

GoalQueryProcessorValues GoalQueryProcessor::fillGoalQueryProcessor()
{
    GoalQueryProcessorValues GoalQueryProcessor;

    for (auto GoalID: primaryKeyResults)
    {
        UserGoalModel_shp newGoal = std::make_shared<UserGoalModel>(UserGoalModel());
        newGoal->selectByGoalID(GoalID);
        GoalQueryProcessor.push_back(newGoal);
    }

    return GoalQueryProcessor;
}

GoalQueryProcessorValues GoalQueryProcessor::runQueryFillGoalQueryProcessor()
{
    if (firstFormattedQuery.empty())
    {
        appendErrorMessage(std::format("Formatting select multiple Goals query string failed {}",
            queryGenerator->getAllErrorMessages()));
        return GoalQueryProcessorValues();
    }
    if (runFirstQuery())
    {
        return fillGoalQueryProcessor();
    }

    return GoalQueryProcessorValues();
}

std::vector<ListExceptionTestElement> GoalQueryProcessor::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&GoalQueryProcessor::testExceptionsGetAllGoalsForUser, this), "getAllGoalsForUser"});
    exceptionTests.push_back({std::bind(&GoalQueryProcessor::testExceptionsGetAllChildrenFromParent, this), "selectAllChildGoalsWithParentFromUser"});
    exceptionTests.push_back({std::bind(&GoalQueryProcessor::testExceptionsFindGoalsWithSimilarDescription, this), "findGoalsByUserIdAndSimilarDescription"});

    return exceptionTests;
}

TestStatus GoalQueryProcessor::testExceptionsGetAllGoalsForUser() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("GoalQueryProcessor::testExceptionsGetAllGoalsForUser()",
         std::bind(&GoalQueryProcessor::getAllGoalsForUser, this, std::placeholders::_1), 1);
}

TestStatus GoalQueryProcessor::testExceptionsGetAllChildrenFromParent() noexcept
{
    selfTestResetAllValues();

    UserGoalModel parent;
    parent.setGoalId(1);
    parent.setDescription("Get a Job in Software Engineering");
    parent.setPriority(1);

    return testListExceptionAndSuccessNArgs("GoalQueryProcessor::testExceptionsGetAllChildrenFromParent()",
         std::bind(&GoalQueryProcessor::getAllChildrenFromParent, this, std::placeholders::_1), parent);
}

TestStatus GoalQueryProcessor::testExceptionsFindGoalsWithSimilarDescription() noexcept
{
    selfTestResetAllValues();
    std::string searchString("Maintain");
    std::size_t userId = 1;

    return testListExceptionAndSuccessNArgs("GoalQueryProcessor::findGoalsByUserIdAndSimilarDescription()",
         std::bind(&GoalQueryProcessor::findGoalsByUserIdAndSimilarDescription, this, std::placeholders::_1, std::placeholders::_2),
         userId, searchString);
}
