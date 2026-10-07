#ifndef USERGOALMODEL_H_
#define USERGOALMODEL_H_

// Project Header Files
#include "CommandLineParser.h"
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

/*
 * Used to retrieve values from the database by the boost library. The names
 * of the variables in the struct must match the names of the columns in
 * the user goal table in the database because the boost reflection library is 
 * used to retrieve them.
 */
struct GoalDbQueryValues
{
    std::int64_t id_user_goals;
    std::uint64_t user_id;
    std::string description;
    std::optional<std::int64_t> priority;
    std::optional<std::uint64_t> parent_goal;
    boost::mysql::datetime creation_timestamp;
    boost::mysql::datetime last_modified_time_stamp;
    std::int64_t deleted;
    std::uint64_t last_modified_by_user;
};

class UserGoalModel : public ModelDBInterface
{
public:

    UserGoalModel();
    UserGoalModel(const GoalDbQueryValues& databaseValues);
    ~UserGoalModel() = default;

// get access methods
    std::size_t getGoalId() const noexcept { return m_primaryKey; };
    std::size_t getUserId() const noexcept { return m_userID; };
    std::string getDescription() const noexcept { return m_description; };
    unsigned int getPriority() const noexcept { return m_priority.value_or(0); };
    std::size_t getParentId() const noexcept { return m_parentID.value_or(0); };

// set access methods
    void setGoalId(std::size_t userGoalId);
    void setUserId(std::size_t userId);
    void setDescription(std::string newDescription);
    void setPriority(unsigned int newPriority);
    void setParentID(std::size_t newParentID);

/*
 * Required fields.
 */
    bool isMissingUserID()  { return m_userID == 0; };
    bool isMissingDescription() { return (m_description.empty() || m_description.size() < 10); };
    void initRequiredFields() override;


    bool operator==(UserGoalModel& other)
    {
        return diffGoal(other);
    };

    friend std::ostream& operator<<(std::ostream& os, const UserGoalModel& goal)
    {
        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << std::format("Model Name {}\n", goal.m_modelName);
        os << std::format(outFmtStr, "Goal ID", goal.m_primaryKey);
        os << std::format(outFmtStr, "User ID", goal.m_userID);
        os << std::format(outFmtStr, "Description", goal.m_description);
        os << std::format(outFmtStr, "Priority", goal.getPriority());
        os << std::format(outFmtStr, "Parent ID", goal.getParentId());

        if (programOptions.showTimeStamps)
            {
            if (goal.m_createdTimeStamp.has_value())
            {
                os << std::format(outFmtStr, "Creation Timestamp", goal.m_createdTimeStamp.value());
            }
            os << std::format(outFmtStr, "Last Update Timestamp", goal.m_lastUpdateTimeStamp.value());
        }

        os << std::format(outFmtStr, "Last Modified by User ID", goal.m_lastModifiedByUser);

        return os;
    };

protected:
    bool diffGoal(UserGoalModel& other);
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatDeleteStatement() override;
    
    std::size_t m_userID;
    std::string m_description;
    std::optional<unsigned int> m_priority;
    std::optional<std::size_t> m_parentID;
};

using UserGoalModel_shp = std::shared_ptr<UserGoalModel>;

#endif // USERGOALMODEL_H_
