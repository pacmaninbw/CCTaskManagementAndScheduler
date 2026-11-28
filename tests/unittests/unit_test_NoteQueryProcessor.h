#ifndef UNIT_TEST_NOTEQUERYPROCESSOR_H_
#define UNIT_TEST_NOTEQUERYPROCESSOR_H_

// Project Header Files
#include "NoteQueryProcessor.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_NoteQueryProcessor : public NoteQueryProcessor
{
public:

};

TEST_CASE("Unit Test NoteQueryProcessor::setNoteId") {
    unit_test_NoteQueryProcessor unitTestNote;

//    std::size_t testValue = 1;
//    unitTestNote.setNoteId(testValue);
//    CHECK(unitTestNote.m_primaryKey == testValue);
}

#endif // UNIT_TEST_NOTEQUERYPROCESSOR_H_

