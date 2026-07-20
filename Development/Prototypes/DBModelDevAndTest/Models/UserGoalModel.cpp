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

/*
 * To make the code more maintainable each field / column in a table will have
 * its own line in insert and update statements. 
 */
std::string UserGoalModel::formatInsertStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());

    boost::mysql::format_sql_to(fctx, "INSERT INTO user_goals (");
    boost::mysql::format_sql_to(fctx, "user_id, ");
    boost::mysql::format_sql_to(fctx, "description, ");
    boost::mysql::format_sql_to(fctx, "priority, ");
    boost::mysql::format_sql_to(fctx, "parent_goal, ");
    boost::mysql::format_sql_to(fctx, "deleted");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_userID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_description);
    boost::mysql::format_sql_to(fctx, "{}, ", m_priority);
    boost::mysql::format_sql_to(fctx, "{}, ", m_parentID);
    boost::mysql::format_sql_to(fctx, "{}", m_deleted);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string UserGoalModel::formatUpdateStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());

    boost::mysql::format_sql_to(fctx, "UPDATE user_goals SET ");
    boost::mysql::format_sql_to(fctx, "user_goals.description = {}, ", m_description);
    boost::mysql::format_sql_to(fctx, "user_goals.priority = {}, ", m_priority);
    boost::mysql::format_sql_to(fctx, "user_goals.parent_goal = {} ", m_parentID);
    boost::mysql::format_sql_to(fctx, "WHERE user_goals.user_id = {} ", m_userID);
    boost::mysql::format_sql_to(fctx, "AND user_goals.id_user_goals = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}

std::string UserGoalModel::formatDeleteStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());

    boost::mysql::format_sql_to(fctx, "UPDATE user_goals SET user_goals.deleted = 1 ");
    boost::mysql::format_sql_to(fctx, "WHERE user_goals.user_id = {} ", m_userID);
    boost::mysql::format_sql_to(fctx, "AND user_goals.id_user_goals = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}


