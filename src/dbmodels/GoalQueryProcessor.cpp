// Project Header Files
#include "QueryProcessor.h"
#include "GoalQueryProcessor.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <format>

GoalQueryProcessor::GoalQueryProcessor()
: QueryProcessor<UserGoalModel, GoalDbQueryValues>("UserGoalModel")
{
}

UserGoalModel_shp GoalQueryProcessor::getGoalById(std::size_t goalId) noexcept
{
    clearErrorMessages();
    UserGoalModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_goals WHERE user_goals.id_user_goals = {}", goalId);
        StaticQueryGoal staticResults = staticRunQueryAsync<GoalDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(staticResults);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In GoalQueryProcessor::{}({}) : {}", __func__, goalId, e.what()));
    }

    return found;
}

UserGoalModel_shp GoalQueryProcessor::findGoalByUserIdAndExactDescription(std::size_t userId, std::string fullDescription) noexcept
{
    clearErrorMessages();
    UserGoalModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_goals ");
        boost::mysql::format_sql_to(fctx, "WHERE user_goals.user_id = {} ", userId);
        boost::mysql::format_sql_to(fctx, "AND user_goals.description = {} ", fullDescription);
        boost::mysql::format_sql_to(fctx, "AND user_goals.deleted <> 1");

        StaticQueryGoal staticResults = staticRunQueryAsync<GoalDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(staticResults);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In GoalQueryProcessor::{}({}, {}) : {}", __func__, userId, fullDescription, e.what()));
    }

    return found;
}

UserGoalList GoalQueryProcessor::getAllGoalsForUser(std::size_t userID) noexcept
{
    clearErrorMessages();

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_goals WHERE user_goals.user_id = {} ", userID);
        boost::mysql::format_sql_to(fctx, "AND user_goals.deleted <> 1");

        StaticQueryGoal staticResults = staticRunQueryAsync<GoalDbQueryValues>(std::move(fctx).get().value());
        return processStaticResults(staticResults);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In GoalQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return UserGoalList();
}

UserGoalList GoalQueryProcessor::getAllChildrenFromParent(UserGoalModel parentGoal) noexcept
{
    clearErrorMessages();

    try
    {
        StaticQueryGoal staticResults = staticRunQueryAsync<GoalDbQueryValues>(formatSelectAllChildGoalsWithParent(parentGoal));
        return processStaticResults(staticResults);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In GoalQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return UserGoalList();
}

UserGoalList GoalQueryProcessor::findGoalsByUserIdAndSimilarDescription(std::size_t userID, std::string searchString) noexcept
{
    clearErrorMessages();

    try
    {
        std::string searchPattern = wrapSearchContentSQLPatternMatch(searchString);

        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_goals ");
        boost::mysql::format_sql_to(fctx, "WHERE user_goals.user_id = {} ", userID);
        boost::mysql::format_sql_to(fctx, "AND user_goals.description LIKE {} ", searchPattern);
        boost::mysql::format_sql_to(fctx, "AND user_goals.deleted <> 1");

        StaticQueryGoal staticResults = staticRunQueryAsync<GoalDbQueryValues>(std::move(fctx).get().value());
        return processStaticResults(staticResults);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In GoalQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return UserGoalList();
}

std::string GoalQueryProcessor::formatSelectAllChildGoalsWithParentFromUser(std::size_t parentId, std::size_t userId)
{
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_goals ");
        boost::mysql::format_sql_to(fctx, "WHERE user_goals.user_id = {} ", userId);
        boost::mysql::format_sql_to(fctx, "AND user_goals.parent_goal = {} ", parentId);
        boost::mysql::format_sql_to(fctx, "AND user_goals.deleted <> 1");

        return std::move(fctx).get().value();
}

std::string GoalQueryProcessor::formatSelectAllChildGoalsWithParent(UserGoalModel &parentGoal)
{
    return formatSelectAllChildGoalsWithParentFromUser(parentGoal.getGoalId(), parentGoal.getUserId());
}

std::vector<ListExceptionTestElement> GoalQueryProcessor::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&GoalQueryProcessor::testExceptionGetGoalByGoalID, this), "getGoalById"});
    exceptionTests.push_back({std::bind(&GoalQueryProcessor::testExceptionsGetAllGoalsForUser, this), "getAllGoalsForUser"});
    exceptionTests.push_back({std::bind(&GoalQueryProcessor::testExceptionsGetAllChildrenFromParent, this), "selectAllChildGoalsWithParentFromUser"});
    exceptionTests.push_back({std::bind(&GoalQueryProcessor::testExceptionsFindGoalsWithSimilarDescription, this), "findGoalsByUserIdAndSimilarDescription"});
    exceptionTests.push_back({std::bind(&GoalQueryProcessor::testExceptionFindGoalByUserIDAndDescription, this), "findGoalByUserIdAndExactDescription"});

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

TestStatus GoalQueryProcessor::testExceptionGetGoalByGoalID() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("GoalQueryProcessor::getGoalById", std::bind(&GoalQueryProcessor::getGoalById, this, std::placeholders::_1), 1);
}

TestStatus GoalQueryProcessor::testExceptionFindGoalByUserIDAndDescription() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::string testDescription("Test SelectByUserIDAndDescription Exception Handling");

    return testExceptionAndSuccessNArgs("GoalQueryProcessor::findGoalByUserIdAndExactDescription",
        std::bind(&GoalQueryProcessor::findGoalByUserIdAndExactDescription,
        this, std::placeholders::_1, std::placeholders::_2), testUserId, testDescription);
}


