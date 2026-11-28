#ifndef UNIT_TEST_NOTEMODEL_H_
#define UNIT_TEST_NOTEMODEL_H_

// Project Header Files
#include "NoteModel.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_NoteModel : public NoteModel
{
public:
    using NoteModel::m_primaryKey;

};

TEST_CASE("Unit Test NoteModel::setNoteId") {
    unit_test_NoteModel unitTestNote;

    std::size_t testValue = 1;
    unitTestNote.setNoteId(testValue);
    CHECK(unitTestNote.m_primaryKey == testValue);
}

#endif // UNIT_TEST_NOTEMODEL_H_
