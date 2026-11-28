#ifndef UNIT_TEST_SCHEDULEITEMLQUERYPROCESSOR_H_
#define UNIT_TEST_SCHEDULEITEMLQUERYPROCESSOR_H_

// Project Header Files
#include "ScheduleItemQueryProcessor.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_ScheduleItemQueryProcessor : public ScheduleItemQueryProcessor
{
public:

};

TEST_CASE("Unit Test ScheduleItemQueryProcessor::setNoteId") {
    unit_test_ScheduleItemQueryProcessor unitTestNote(1);

//    std::size_t testValue = 1;
//    unitTestNote.setNoteId(testValue);
//    CHECK(unitTestNote.m_primaryKey == testValue);
}

#endif // UNIT_TEST_SCHEDULEITEMLQUERYPROCESSOR_H_

