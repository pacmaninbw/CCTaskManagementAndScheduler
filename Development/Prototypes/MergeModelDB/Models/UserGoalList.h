#ifndef USERGOALLIST_H_
#define USERGOALLIST_H_

// Project Header Files
#include "ListDBInterface.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <iostream>

using UserGoalListValues = std::vector<UserGoalModel_shp>;

class UserGoalList : public ListDBInterface<UserGoalModel>
{
public:
    UserGoalList();
    virtual ~UserGoalList() = default;

    UserGoalListValues getAllGoalsForUser(std::size_t userID) noexcept;
    UserGoalListValues getAllChildrenFromParent(UserGoalModel parentGoal) noexcept;

private:
    UserGoalListValues fillUserGoalList();
    UserGoalListValues runQueryFillUserGoalList();

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllGoalsForUser() noexcept;
    TestStatus testExceptionsGetAllChildrenFromParent() noexcept;
};

#endif // USERGOALLIST_H_

