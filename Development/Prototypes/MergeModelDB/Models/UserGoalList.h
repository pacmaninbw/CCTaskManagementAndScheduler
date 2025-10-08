#ifndef USERGOALLIST_H_
#define USERGOALLIST_H_

// Project Header Files
#include "ListDBInterface.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

using UserGoalListValues = std::vector<UserGoalModel_shp>;

class UserGoalList : public ListDBInterface<UserGoalModel>
{
public:
    UserGoalList();
    virtual ~UserGoalList() = default;

    UserGoalListValues getAllGoalsForUser(std::size_t userID);
    UserGoalListValues getAllChildrenFromParent(UserGoalModel& parentGoal);

private:
    UserGoalListValues fillUserGoalList();
    UserGoalListValues runQueryFillUserGoalList();

};

#endif // USERGOALLIST_H_

