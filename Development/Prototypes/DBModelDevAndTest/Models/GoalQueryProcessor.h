#ifndef GOALQUERYPROCESSOR_H_
#define GOALQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <string>

using UserGoalList = std::vector<UserGoalModel_shp>;

class GoalQueryProcessor : public QueryProcessor<UserGoalModel>
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
    virtual UserGoalModel_shp processResultRow(boost::mysql::row_view& queryRow) override;
    virtual void fillRequiredIndexes() override;
    std::string formatSelectAllChildGoalsWithParentFromUser(std::size_t parentId, std::size_t userId);
    std::string formatSelectAllChildGoalsWithParent(UserGoalModel& parentGoal);

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllGoalsForUser() noexcept;
    TestStatus testExceptionsGetAllChildrenFromParent() noexcept;
    TestStatus testExceptionsFindGoalsWithSimilarDescription() noexcept;
    TestStatus testExceptionGetGoalByGoalID() noexcept;
    TestStatus testExceptionFindGoalByUserIDAndDescription() noexcept;

    std::size_t GoalIdIdx = IndexNotSet;
    std::size_t UserIdIdx = IndexNotSet;
    std::size_t DescriptionIdx = IndexNotSet;
    std::size_t CreationTSIdx = IndexNotSet;
    std::size_t LastUpdateIdx = IndexNotSet;
    std::size_t PriorityIdx = IndexNotSet;
    std::size_t ParentGoalIDIdx = IndexNotSet;
    std::size_t HiddenIdx = IndexNotSet;
};

#endif // GOALQUERYPROCESSOR_H_

