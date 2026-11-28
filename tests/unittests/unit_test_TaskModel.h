#ifndef UNIT_TEST_TASKMODEL_H_
#define UNIT_TEST_TASKMODEL_H_

// Project Header Files
#include "TaskModel.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_TaskModel : public TaskModel
{
public:
    using TaskModel::m_primaryKey;

};

TEST_CASE("Unit Test TaskModel::setTaskId") {
    unit_test_TaskModel unitTestTask;

    std::size_t testValue = 1;
    unitTestTask.setTaskID(testValue);
    CHECK(unitTestTask.m_primaryKey == testValue);
}

#endif // UNIT_TEST_TASKMODEL_H_

