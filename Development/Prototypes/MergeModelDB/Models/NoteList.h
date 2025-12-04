#ifndef NOTELIST_H_
#define NOTELIST_H_

// Project Header Files
#include "ListDBInterface.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <chrono>
#include <iostream>
#include <string>

using NoteListValues = std::vector<NoteModel_shp>;

class NoteList : public ListDBInterface<NoteModel>
{
public:
    NoteList();
    virtual ~NoteList() = default;
    NoteListValues getAllNotesForUser(std::size_t userId) noexcept;
    NoteListValues getNotesForUserSimlarToContent(std::size_t userId, std::string likeContent) noexcept;
    NoteListValues getAllNotesForUserCreatedInDatgeRange(std::size_t userId,
        std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept;
    NoteListValues getAllNotesForUserEditedInDatgeRange(std::size_t userId,
        std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept;

private:
    NoteListValues runQueryFillNoteList();
    NoteListValues fillNoteList();

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllNotes() noexcept;
    TestStatus testExceptionsGetNotesForUserLikeContent() noexcept;
    TestStatus testExceptionsGetNotesForUserCreatedDateRange() noexcept;
    TestStatus testExceptionsGetNotesForUserEditedDateRange() noexcept;
};

#endif // NOTELIST_H_

