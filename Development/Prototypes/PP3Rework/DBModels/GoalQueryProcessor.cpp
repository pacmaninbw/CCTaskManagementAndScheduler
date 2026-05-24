// Project Header Files
#include "QueryProcessor.h"
#include "GoalQueryProcessor.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>

GoalQueryProcessor::GoalQueryProcessor()
: QueryProcessor<UserGoalModel>("UserGoalModel",{"idUserGoals", "UserID", "Description", "CreationTS", "LastUpdateTS", "Priority", "ParentGoal", "Hidden"} )
{
}

UserGoalModel_shp GoalQueryProcessor::getGoalById(std::size_t goalId) noexcept
{
    errorMessages.clear();
    UserGoalModel_shp found = nullptr;

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetGoalById({})", goalId);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        found = getOneResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{}({}) : {}", __func__, goalId, e.what()));
    }

    return found;
}

UserGoalModel_shp GoalQueryProcessor::findGoalByUserIdAndExactDescription(std::size_t userId, std::string fullDescription) noexcept
{
    errorMessages.clear();
    UserGoalModel_shp found = nullptr;

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL FindGoalByUserIdAndExactDescription({}, {})", userId, fullDescription);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        found = getOneResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserQueryProcessor::{}({}, {}) : {}", __func__, userId, fullDescription, e.what()));
    }

    return found;
}

UserGoalList GoalQueryProcessor::getAllGoalsForUser(std::size_t userID) noexcept
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetAllGoalsForUser({})", userID);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In GoalQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return UserGoalList();
}

UserGoalList GoalQueryProcessor::getAllChildrenFromParent(UserGoalModel parentGoal) noexcept
{
    errorMessages.clear();

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatSelectAllChildGoalsWithParent(parentGoal));
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In GoalQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return UserGoalList();
}

UserGoalList GoalQueryProcessor::findGoalsByUserIdAndSimilarDescription(std::size_t userID, std::string searchString) noexcept
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL FindGoalsByUserIdAndSimilarDescription({}, {})", userID, searchString);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In GoalQueryProcessor::{} : {}", __func__, e.what()));
    }
    
    return UserGoalList();
}

UserGoalModel_shp GoalQueryProcessor::processResultRow(boost::mysql::row_view& queryRow)
{
    std::size_t goalId = queryRow.at(GoalIdIdx).as_uint64();
    std::size_t userID = queryRow.at(UserIdIdx).as_uint64();
    std::chrono::system_clock::time_point creationDate = boostMysqlDateTimeToChronoTimePoint(queryRow.at(CreationTSIdx).as_datetime());
    std::string description = queryRow.at(DescriptionIdx).as_string();
    std::chrono::system_clock::time_point lastUpdate = boostMysqlDateTimeToChronoTimePoint(queryRow.at(LastUpdateIdx).as_datetime());
    unsigned int priority = 0;
    std::size_t parentID = 0;
    bool deleted = false;

    // Optional fields.
    if (!queryRow.at(PriorityIdx).is_null())
    {
        priority = queryRow.at(PriorityIdx).as_int64();
    }

    if (!queryRow.at(ParentGoalIDIdx).is_null())
    {
        parentID = queryRow.at(ParentGoalIDIdx).as_uint64();
    }

    if (!queryRow.at(HiddenIdx).is_null())
    {
        deleted = queryRow.at(HiddenIdx).as_int64() == 1? true : false;
    }

    return std::make_shared<UserGoalModel>(goalId, userID, description, priority, parentID, creationDate, lastUpdate, deleted);
}

void GoalQueryProcessor::fillRequiredIndexes()
{
    assignValueToIndex("idUserGoals", GoalIdIdx);
    assignValueToIndex("UserID", UserIdIdx);
    assignValueToIndex("Description", DescriptionIdx);
    assignValueToIndex("CreationTS", CreationTSIdx);
    assignValueToIndex("LastUpdateTS", LastUpdateIdx);
    assignValueToIndex("Priority",PriorityIdx);
    assignValueToIndex("ParentGoal",ParentGoalIDIdx);
    assignValueToIndex("Hidden",HiddenIdx);
}

std::string GoalQueryProcessor::formatSelectAllChildGoalsWithParentFromUser(std::size_t parentId, std::size_t userId)
{
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetAllChildGoalsFromParent({}, {})", userId, parentId);

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


