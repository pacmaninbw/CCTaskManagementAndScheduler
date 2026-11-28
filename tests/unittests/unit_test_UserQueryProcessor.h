#ifndef UNIT_TEST_USERQUERYPROCESSOR_H_
#define UNIT_TEST_USERQUERYPROCESSOR_H_

// Project Header Files
#include "UserQueryProcessor.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_UserQueryProcessor : public UserQueryProcessor
{
public:

};

TEST_CASE("Unit Test UserQueryProcessor::setNoteId") {
    unit_test_UserQueryProcessor unitTestNote;

//    std::size_t testValue = 1;
//    unitTestNote.setNoteId(testValue);
//    CHECK(unitTestNote.m_primaryKey == testValue);
}

#endif // UNIT_TEST_USERQUERYPROCESSOR_H_

