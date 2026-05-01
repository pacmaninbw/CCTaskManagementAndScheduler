#ifndef NOTEQUERYPROCESSOR_H_
#define NOTEQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <chrono>
#include <iostream>
#include <string>

using NoteQueryProcessorValues = std::vector<NoteModel_shp>;

class NoteQueryProcessor : public QueryProcessor<NoteModel>
{
public:
    NoteQueryProcessor();
    virtual ~NoteQueryProcessor() = default;
    NoteQueryProcessorValues getAllNotesForUser(std::size_t userId) noexcept;
    NoteQueryProcessorValues getNotesForUserSimlarToContent(std::size_t userId, std::string likeContent) noexcept;
    NoteQueryProcessorValues getAllNotesForUserCreatedInDatgeRange(std::size_t userId,
        std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept;
    NoteQueryProcessorValues getAllNotesForUserEditedInDatgeRange(std::size_t userId,
        std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept;
    NoteQueryProcessorValues getDashboardNoteTable(std::size_t userId, std::chrono::year_month_day searchDate) noexcept;

private:
    NoteQueryProcessorValues runQueryFillNoteQueryProcessor();
    NoteQueryProcessorValues fillNoteQueryProcessor();

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllNotes() noexcept;
    TestStatus testExceptionsGetNotesForUserLikeContent() noexcept;
    TestStatus testExceptionsGetNotesForUserCreatedDateRange() noexcept;
    TestStatus testExceptionsGetNotesForUserEditedDateRange() noexcept;
    TestStatus testExceptionsGetDashboardNoteTable() noexcept;
};

#endif // NOTEQUERYPROCESSOR_H_

