#ifndef USERGOALLIST_H_
#define USERGOALLIST_H_

#include <chrono>
#include <format>
#include <iostream>
#include "ListDBInterface.h"
#include "UserGoalModel.h"

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

