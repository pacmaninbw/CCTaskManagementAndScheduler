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

class UserGoalModel : public ModelDBInterface
{
public:

    UserGoalModel();
    UserGoalModel(std::size_t goalId, std::size_t userId, std::string description, unsigned int priority,
        std::size_t parentID, std::chrono::system_clock::time_point creationDate,
        std::chrono::system_clock::time_point lastUpdate, bool hidden
    );
    ~UserGoalModel() = default;

// get access methods
    std::size_t getGoalId() const noexcept { return m_primaryKey; };
    std::size_t getUserId() const noexcept { return m_userID; };
    std::string getDescription() const noexcept { return m_description; };
    unsigned int getPriority() const noexcept { return m_priority.value_or(0); };
    std::size_t getParentId() const noexcept { return m_parentID.value_or(0); };
    std::chrono::system_clock::time_point getCreationTimeStamp() const noexcept { return m_created.value(); };
    std::chrono::system_clock::time_point getLastUpdateTimeStamp() const noexcept { return m_lastUpdate; };

// set access methods
    void setGoalId(std::size_t userGoalId);
    void setUserId(std::size_t userId);
    void setDescription(std::string newDescription);
    void setPriority(unsigned int newPriority);
    void setParentID(std::size_t newParentID);
    void setCreationTimeStamp(std::chrono::system_clock::time_point newCreationTS);

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
    bool operator==(std::shared_ptr<UserGoalModel> other)
    {
        return diffGoal(*other);
    }


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
            if (goal.m_created.has_value())
            {
                os << std::format(outFmtStr, "Creation Timestamp", goal.m_created.value());
            }
            os << std::format(outFmtStr, "Last Update Timestamp", goal.getLastUpdateTimeStamp());
        }

        return os;
    };

protected:
    bool diffGoal(UserGoalModel& other);
//    void processResultRow(boost::mysql::row_view rv) override;
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatDeleteStatement() override;
//    std::string formatSelectStatement() override;
    
    std::size_t m_userID;
    std::string m_description;
    std::optional<unsigned int> m_priority;
    std::optional<std::size_t> m_parentID;
/*
 * The date of creation can be controlled by the user, if it hasn't been set
 * inserting it into the database will set it. The last update is automatically
 * set by insertion or update into the database.
 */
    std::optional<std::chrono::system_clock::time_point> m_created;
    std::chrono::system_clock::time_point m_lastUpdate;
};

using UserGoalModel_shp = std::shared_ptr<UserGoalModel>;

#endif // USERGOALMODEL_H_
