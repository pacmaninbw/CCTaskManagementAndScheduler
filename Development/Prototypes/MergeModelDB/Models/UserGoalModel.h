#ifndef USERGOALMODEL_H_
#define USERGOALMODEL_H_

#include <chrono>
//#include "commonUtilities.h"
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include "ModelDBInterface.h"
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
    std::chrono::system_clock::time_point getCreationTimeStamp() const noexcept { return creationDate; };
    std::chrono::system_clock::time_point getLastUpdateTimeStamp() const noexcept { return lastUpdate; };

// set access methods
    void setGoalId(std::size_t userGoalId);
    void setUserId(std::size_t userId);
    void setDescription(std::string newDescription);
    void setPriority(unsigned int newPriority);
    void setParentID(std::size_t newParentID);
    void setCreationTimeStamp(std::chrono::system_clock::time_point newCreationTS);
    void setLastUpdateTimeStamp(std::chrono::system_clock::time_point newLastUpdateTS);
/*
 * Select with arguments
 */
    bool selectByGoalID(std::size_t goalID);
    bool selectByUserIDAndDescription(std::size_t userID, std::string targetDesciption);

/*
 * Required fields.
 */
    bool isMissingUserID()  { return userID == 0; };
    bool isMissingDescription() { return (description.empty() || description.size() < 10); };
    bool isMissingCreationDate() { return creationDate.time_since_epoch() == std::chrono::system_clock::duration::zero(); };
    bool isMissingLastUpdate() { return lastUpdate.time_since_epoch() == std::chrono::system_clock::duration::zero(); };
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
        os << std::format(outFmtStr, "Creation Timestamp", goal.creationDate);
        os << std::format(outFmtStr, "Last Update Timestamp", goal.lastUpdate);

        return os;
    };

private:
    bool diffGoal(UserGoalModel& other);
    void processResultRow(NSBM::row_view rv) override;
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatSelectStatement() override;

    
    std::size_t userID;
    std::string description;
    std::optional<unsigned int> priority;
    std::optional<std::size_t> parentID;
    std::chrono::system_clock::time_point creationDate;
    std::chrono::system_clock::time_point lastUpdate;


private:
/*
 * The indexes below are based on the following select statement, maintain this order.
 */
    NSBM::constant_string_view baseQuery = 
        "SELECT idUserGoals, UserID, Description, CreationTS, LastUpdateTS, Priority, ParentGoal FROM UserGoals ";

    const std::size_t GoalIdIdx = 0;
    const std::size_t UserIdIdx = 1;
    const std::size_t DescriptionIdx = 2;
    const std::size_t CreationTSIdx = 3;
    const std::size_t LastUpdateIdx = 4;
    const std::size_t PriorityIdx = 5;
    const std::size_t ParentGoalIDIdx = 6;
};

using UserGoalModel_shp = std::shared_ptr<UserGoalModel>;

#endif // USERGOALMODEL_H_
