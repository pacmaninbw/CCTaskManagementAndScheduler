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
        os << std::format("Model Name {}\n", goal.modelName);
        os << std::format(outFmtStr, "Goal ID", goal.primaryKey);
        os << std::format(outFmtStr, "User ID", goal.userID);
        os << std::format(outFmtStr, "Description", goal.description);
        os << std::format(outFmtStr, "Priority", goal.getPriority());
        os << std::format(outFmtStr, "Parent ID", goal.getParentId());

        if (programOptions.showTimeStamps)
            {
            if (goal.creationDate.has_value())
            {
                os << std::format(outFmtStr, "Creation Timestamp", goal.creationDate.value());
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
};

using UserGoalModel_shp = std::shared_ptr<UserGoalModel>;

#endif // USERGOALMODEL_H_
