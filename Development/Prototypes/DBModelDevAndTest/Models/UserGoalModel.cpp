// Project Header Files
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <vector>

UserGoalModel::UserGoalModel()
: ModelDBInterface("UserGoalModel", "idUserGoals")
{
    userID = 0;
}

UserGoalModel::UserGoalModel(
    std::size_t goalId,
    std::size_t userId,
    std::string descriptionIn,
    unsigned int priorityIn,
    std::size_t parentIdIn,
    std::chrono::system_clock::time_point creationDateIn,
    std::chrono::system_clock::time_point lastUpdateIn,
    bool hidden
)
: UserGoalModel()
{
    m_PrimaryKey = goalId;
    userID = userId;
    description = descriptionIn;
    priority = priorityIn;
    parentID = parentIdIn;
    creationDate = creationDateIn;
    lastUpdate = lastUpdateIn;
    m_Deleted = hidden;
}

void UserGoalModel::setGoalId(std::size_t userGoalId)
{
    m_Modified = true;
    m_PrimaryKey = userGoalId;
}

void UserGoalModel::setUserId(std::size_t userId)
{
    m_Modified = true;
    userID = userId;
}

void UserGoalModel::setDescription(std::string newDescription)
{
    m_Modified = true;
    description = newDescription;
}

void UserGoalModel::setPriority(unsigned int newPriority)
{
    m_Modified = true;
    priority = newPriority;
}

void UserGoalModel::setParentID(std::size_t newParentID)
{
    m_Modified = true;
    parentID = newParentID;
}

void UserGoalModel::setCreationTimeStamp(std::chrono::system_clock::time_point newCreationTS)
{
    m_Modified = true;
    creationDate = newCreationTS;
}

bool UserGoalModel::diffGoal(UserGoalModel &other)
{
    return (m_PrimaryKey == other.m_PrimaryKey && userID == other.userID &&
        description == other.description);
}

void UserGoalModel::initRequiredFields()
{
    m_MissingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingUserID, this), "User ID"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingDescription, this), "Description"});
}

std::string UserGoalModel::formatInsertStatement()
{
    initFormatOptions();

    std::string insertStatement = boost::mysql::format_sql(format_opts.value(),
        "CALL AddUserGoal({0}, {1}, {2}, {3})", userID, description, priority, parentID);

    return insertStatement;
}

std::string UserGoalModel::formatUpdateStatement()
{
    initFormatOptions();

    std::string updateStatement = boost::mysql::format_sql(format_opts.value(),
        "CALL UpdateUserGoalAllFields({0}, {1}, {2}, {3}, {4})",
        userID, m_PrimaryKey, description, priority, parentID);
        
    return updateStatement;
}

std::string UserGoalModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),"CALL HideGoal({}, {})", userID, m_PrimaryKey);
}


