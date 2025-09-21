#include <exception>
#include <chrono>
//#include "commonUtilities.h"
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include "UserGoalModel.h"
#include <vector>

UserGoalModel::UserGoalModel()
: ModelDBInterface("UserGoalModel")
{
    userID = 0;
    creationDate = std::chrono::system_clock::now();
    lastUpdate = creationDate;
}

void UserGoalModel::setGoalId(std::size_t userGoalId)
{
    modified = true;
    primaryKey = userGoalId;
}

void UserGoalModel::setDescription(std::string newDescription)
{
    modified = true;
    description = newDescription;
}

void UserGoalModel::setPriority(unsigned int newPriority)
{
    modified = true;
    priority = newPriority;
}

void UserGoalModel::setParentID(std::size_t newParentID)
{
    modified = true;
    parentID = newParentID;
}

void UserGoalModel::setCreationTimeStamp(std::chrono::system_clock::time_point newCreationTS)
{
    modified = true;
    creationDate = newCreationTS;
}

void UserGoalModel::setLastUpdateTimeStamp(std::chrono::system_clock::time_point newLastUpdateTS)
{
    modified = true;
    lastUpdate = newLastUpdateTS;
}

bool UserGoalModel::diffGoal(UserGoalModel &other)
{
    return (primaryKey == other.primaryKey && userID == other.userID &&
        description == other.description);
}

void UserGoalModel::initRequiredFields()
{
    missingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingUserID, this), "User ID"});
    missingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingDescription, this), "Description"});
    missingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingCreationDate, this), "Creation Timestamp"});
    missingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingLastUpdate, this), "Last Update"});
}

std::string UserGoalModel::formatInsertStatement()
{
    initFormatOptions();

    std::string insertStatement = NSBM::format_sql(format_opts.value(),
        "INSERT INTO UserGoals (UserID, Description, CreationTS, LastUpdateTS, "
            "Priority, ParentGoal) VALUES ({0}, {1}, {2}, {3}, {4}, {5})",
        userID, description, stdChronoTimePointToBoostDateTime(creationDate),
        stdChronoTimePointToBoostDateTime(lastUpdate), priority, parentID);

    return insertStatement;
}

std::string UserGoalModel::formatUpdateStatement()
{
    initFormatOptions();

    std::string updateStatement = NSBM::format_sql(format_opts.value(),
        "UPDATE UserGoals SET"
            " UserGoals.UserID = {0},"
            " UserGoals.Description = {1},"
            " UserGoals.CreationTS = {2},"
            " UserGoals.LastUpdateTS = {3}" 
            " UserGoals.Priority = {4}" 
            " UserGoals.ParentGoal = {5}" 
        " WHERE UserGoals.idUserGoals = {6}",
        userID, description, stdChronoTimePointToBoostDateTime(creationDate),
        stdChronoTimePointToBoostDateTime(lastUpdate), priority, parentID, primaryKey);
        
    return updateStatement;
}

std::string UserGoalModel::formatSelectStatement()
{
    initFormatOptions();

    NSBM::format_context fctx(format_opts.value());
    NSBM::format_sql_to(fctx, baseQuery);
    NSBM::format_sql_to(fctx, " WHERE idUserGoals = {}", primaryKey);

    return std::move(fctx).get().value();
}


void UserGoalModel::processResultRow(NSBM::row_view rv)
{
    primaryKey = rv.at(GoalIdIdx).as_uint64();
    userID = rv.at(UserIdIdx).as_uint64();
    creationDate = boostMysqlDateTimeToChronoTimePoint(rv.at(CreationTSIdx).as_datetime());
    description = rv.at(DescriptionIdx).as_string();
    lastUpdate = boostMysqlDateTimeToChronoTimePoint(rv.at(LastUpdateIdx).as_datetime());

    // Optional fields.
    if (!rv.at(PriorityIdx).is_null())
    {
        priority = rv.at(PriorityIdx).as_uint64();
    }

    if (!rv.at(ParentGoalIDIdx).is_null())
    {
        parentID = rv.at(ParentGoalIDIdx).as_uint64();
    }
}

bool UserGoalModel::selectByGoalID(std::size_t noteID)
{
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE idUserGoals = {}", noteID);

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In UserGoalModel::selectByGoalID : {}", e.what()));
        return false;
    }
}
