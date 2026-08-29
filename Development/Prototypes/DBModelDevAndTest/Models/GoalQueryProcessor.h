#ifndef GOALQUERYPROCESSOR_H_
#define GOALQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "UserGoalModel.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <boost/mysql/pfr.hpp>

// Standard C++ Header Files
#include <string>

using UserGoalList = std::vector<UserGoalModel_shp>;
using StaticQueryGoal = boost::mysql::static_results<boost::mysql::pfr_by_name<GoalDbQueryValues>>;

class GoalQueryProcessor : public QueryProcessor<UserGoalModel, GoalDbQueryValues>
{
public:
    GoalQueryProcessor();
    virtual ~GoalQueryProcessor() = default;

    UserGoalModel_shp getGoalById(std::size_t goalId) noexcept;
    UserGoalModel_shp findGoalByUserIdAndExactDescription(std::size_t userID, std::string searchString) noexcept;
    UserGoalList getAllGoalsForUser(std::size_t userID) noexcept;
    UserGoalList getAllChildrenFromParent(UserGoalModel parentGoal) noexcept;
    UserGoalList findGoalsByUserIdAndSimilarDescription(std::size_t userID, std::string searchString) noexcept;

private:

    std::string formatSelectAllChildGoalsWithParentFromUser(std::size_t parentId, std::size_t userId);
    std::string formatSelectAllChildGoalsWithParent(UserGoalModel& parentGoal);

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllGoalsForUser() noexcept;
    TestStatus testExceptionsGetAllChildrenFromParent() noexcept;
    TestStatus testExceptionsFindGoalsWithSimilarDescription() noexcept;
    TestStatus testExceptionGetGoalByGoalID() noexcept;
    TestStatus testExceptionFindGoalByUserIDAndDescription() noexcept;
};

#endif // GOALQUERYPROCESSOR_H_

