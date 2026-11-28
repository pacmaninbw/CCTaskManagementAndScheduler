#ifndef UNIT_TEST_USERGOALMODEL_H_
#define UNIT_TEST_USERGOALMODEL_H_

// Project Header Files
#include "UserGoalModel.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_UserGoalModel : public UserGoalModel
{
public:
    using UserGoalModel::m_primaryKey;

};

TEST_CASE("Unit Test UserGoalModel::setNoteId") {
    unit_test_UserGoalModel unitTestGoal;

    std::size_t testValue = 1;
    unitTestGoal.setGoalId(testValue);
    CHECK(unitTestGoal.m_primaryKey == testValue);
}

#endif // UNIT_TEST_USERGOALMODEL_H_

