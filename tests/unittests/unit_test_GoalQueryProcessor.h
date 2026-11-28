#ifndef UNIT_TEST_GOALQUERYPROCESSOR_H_
#define UNIT_TEST_GOALQUERYPROCESSOR_H_

// Project Header Files
#include "GoalQueryProcessor.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_GoalQueryProcessor : public GoalQueryProcessor
{
public:

};

TEST_CASE("Unit Test GoalQueryProcessor::setNoteId") {
    unit_test_GoalQueryProcessor unitTestNote;

//    std::size_t testValue = 1;
//    unitTestNote.setNoteId(testValue);
//    CHECK(unitTestNote.m_primaryKey == testValue);
}

#endif // UNIT_TEST_GOALQUERYPROCESSOR_H_

