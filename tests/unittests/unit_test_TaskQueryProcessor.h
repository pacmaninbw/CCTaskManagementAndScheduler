#ifndef UNIT_TEST_TASKQUERYPROCESSOR_H_
#define UNIT_TEST_TASKQUERYPROCESSOR_H_

// Project Header Files
#include "TaskQueryProcessor.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_TaskQueryProcessor : public TaskQueryProcessor
{
public:

};

TEST_CASE("Unit Test TaskQueryProcessor::setNoteId") {
    unit_test_TaskQueryProcessor unitTestNote;

//    std::size_t testValue = 1;
//    unitTestNote.setNoteId(testValue);
//    CHECK(unitTestNote.m_primaryKey == testValue);
}

#endif // UNIT_TEST_TASKQUERYPROCESSOR_H_

