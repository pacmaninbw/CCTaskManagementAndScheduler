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
    m_userID = 0;
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
    m_primaryKey = goalId;
    m_userID = userId;
    m_description = descriptionIn;
    m_priority = priorityIn;
    m_parentID = parentIdIn;
    m_created = creationDateIn;
    m_lastUpdate = lastUpdateIn;
    m_deleted = hidden;
}

void UserGoalModel::setGoalId(std::size_t userGoalId)
{
    m_modified = true;
    m_primaryKey = userGoalId;
}

void UserGoalModel::setUserId(std::size_t userId)
{
    m_modified = true;
    m_userID = userId;
}

void UserGoalModel::setDescription(std::string newDescription)
{
    m_modified = true;
    m_description = newDescription;
}

void UserGoalModel::setPriority(unsigned int newPriority)
{
    m_modified = true;
    m_priority = newPriority;
}

void UserGoalModel::setParentID(std::size_t newParentID)
{
    m_modified = true;
    m_parentID = newParentID;
}

void UserGoalModel::setCreationTimeStamp(std::chrono::system_clock::time_point newCreationTS)
{
    m_modified = true;
    m_created = newCreationTS;
}

bool UserGoalModel::diffGoal(UserGoalModel &other)
{
    return (m_primaryKey == other.m_primaryKey && m_userID == other.m_userID &&
        m_description == other.m_description);
}

void UserGoalModel::initRequiredFields()
{
    m_missingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingUserID, this), "User ID"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingDescription, this), "Description"});
}

std::string UserGoalModel::formatInsertStatement()
{
    initFormatOptions();

    std::string insertStatement = boost::mysql::format_sql(m_formatOpts.value(),
        "CALL AddUserGoal({0}, {1}, {2}, {3})", m_userID, m_description, m_priority, m_parentID);

    return insertStatement;
}

std::string UserGoalModel::formatUpdateStatement()
{
    initFormatOptions();

    std::string updateStatement = boost::mysql::format_sql(m_formatOpts.value(),
        "CALL UpdateUserGoalAllFields({0}, {1}, {2}, {3}, {4})",
        m_userID, m_primaryKey, m_description, m_priority, m_parentID);
        
    return updateStatement;
}

std::string UserGoalModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(m_formatOpts.value(),"CALL HideGoal({}, {})", m_userID, m_primaryKey);
}


