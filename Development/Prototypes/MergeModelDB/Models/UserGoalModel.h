#ifndef USERGOALMODEL_H_
#define USERGOALMODEL_H_

// Project Header Files
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class UserGoalModel : public ModelDBInterface
{
public:

    UserGoalModel();
    ~UserGoalModel() = default;

// get access methods
    std::size_t getGoalId() const noexcept { return primaryKey; };
    std::size_t getUserId() const noexcept { return userID; };
    std::string getDescription() const noexcept { return description; };
    unsigned int getPriority() const noexcept { return priority.value_or(0); };
    std::size_t getParentId() const noexcept { return parentID.value_or(0); };
    std::chrono::system_clock::time_point getCreationTimeStamp() const noexcept { return creationDate.value(); };
    std::chrono::system_clock::time_point getLastUpdateTimeStamp() const noexcept { return lastUpdate; };

// set access methods
    void setGoalId(std::size_t userGoalId);
    void setUserId(std::size_t userId);
    void setDescription(std::string newDescription);
    void setPriority(unsigned int newPriority);
    void setParentID(std::size_t newParentID);
    void setCreationTimeStamp(std::chrono::system_clock::time_point newCreationTS);
/*
 * Select with arguments
 */
    bool selectByGoalID(std::size_t goalID);
    bool selectByUserIDAndDescription(std::size_t userID, std::string targetDesciption);
    std::string formatSelectAllByUserId(std::size_t userId);
    std::string formatSelectAllChildGoalsWithParentFromUser(std::size_t parentId, std::size_t userId);
    std::string formatSelectAllChildGoalsWithParent(UserGoalModel& parentGoal);
    std::string formatSelectByExactDescription(std::string fullDescription, std::size_t userId);
    std::string formatSelectBySimilarDescription(std::string partialDescription, std::size_t userId);

/*
 * Required fields.
 */
    bool isMissingUserID()  { return userID == 0; };
    bool isMissingDescription() { return (description.empty() || description.size() < 10); };
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
        os << std::format(outFmtStr, "Goal ID", goal.primaryKey);
        os << std::format(outFmtStr, "User ID", goal.userID);
        os << std::format(outFmtStr, "Description", goal.description);
        os << std::format(outFmtStr, "Priority", goal.getPriority());
        os << std::format(outFmtStr, "Parent ID", goal.getParentId());
        if (goal.creationDate.has_value())
        {
            os << std::format(outFmtStr, "Creation Timestamp", goal.creationDate.value());
        }
        os << std::format(outFmtStr, "Last Update Timestamp", goal.getLastUpdateTimeStamp());

        return os;
    };

protected:
    bool diffGoal(UserGoalModel& other);
    void processResultRow(boost::mysql::row_view rv) override;
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatSelectStatement() override;
    
    std::size_t userID;
    std::string description;
    std::optional<unsigned int> priority;
    std::optional<std::size_t> parentID;
/*
 * The date of creation can be controlled by the user, if it hasn't been set
 * inserting it into the database will set it. The last update is automatically
 * set by insertion or update into the database.
 */
    std::optional<std::chrono::system_clock::time_point> creationDate;
    std::chrono::system_clock::time_point lastUpdate;

private:
/*
 * The indexes below are based on the following select statement, maintain this order.
 */
    boost::mysql::constant_string_view baseQuery = 
        "SELECT idUserGoals, UserID, Description, CreationTS, LastUpdateTS, Priority, ParentGoal FROM UserGoals ";

    static const std::size_t GoalIdIdx = 0;
    static const std::size_t UserIdIdx = 1;
    static const std::size_t DescriptionIdx = 2;
    static const std::size_t CreationTSIdx = 3;
    static const std::size_t LastUpdateIdx = 4;
    static const std::size_t PriorityIdx = 5;
    static const std::size_t ParentGoalIDIdx = 6;

    boost::mysql::constant_string_view listQueryBase = "SELECT idUserGoals FROM UserGoals ";
};

using UserGoalModel_shp = std::shared_ptr<UserGoalModel>;

#endif // USERGOALMODEL_H_
