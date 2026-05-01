#ifndef GOALQUERYPROCESSOR_H_
#define GOALQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <iostream>
#include <string>

using GoalQueryProcessorValues = std::vector<UserGoalModel_shp>;

class GoalQueryProcessor : public QueryProcessor<UserGoalModel>
{
public:
    GoalQueryProcessor();
    virtual ~GoalQueryProcessor() = default;

    GoalQueryProcessorValues getAllGoalsForUser(std::size_t userID) noexcept;
    GoalQueryProcessorValues getAllChildrenFromParent(UserGoalModel parentGoal) noexcept;
    GoalQueryProcessorValues findGoalsByUserIdAndSimilarDescription(std::size_t userID, std::string searchString) noexcept;

private:
    GoalQueryProcessorValues fillGoalQueryProcessor();
    GoalQueryProcessorValues runQueryFillGoalQueryProcessor();

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllGoalsForUser() noexcept;
    TestStatus testExceptionsGetAllChildrenFromParent() noexcept;
    TestStatus testExceptionsFindGoalsWithSimilarDescription() noexcept;
};

#endif // GOALQUERYPROCESSOR_H_

