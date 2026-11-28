#ifndef UNIT_TEST_SCHEDULEITEMMODEL_H_
#define UNIT_TEST_SCHEDULEITEMMODEL_H_

// Project Header Files
#include "ScheduleItemModel.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_ScheduleItemModel : public ScheduleItemModel
{
public:
    using ScheduleItemModel::m_primaryKey;

};

TEST_CASE("Unit Test ScheduleItemModel::setNoteId") {
    unit_test_ScheduleItemModel unitTestScheduleItem;

    std::size_t testValue = 1;
    unitTestScheduleItem.setScheduleItemID(testValue);
    CHECK(unitTestScheduleItem.m_primaryKey == testValue);
}

#endif // UNIT_TEST_SCHEDULEITEMMODEL_H_

