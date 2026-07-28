#ifndef NOTEQUERYPROCESSOR_H_
#define NOTEQUERYPROCESSOR_H_

// Project Header Files
#include "NoteModel.h"
#include "QueryProcessor.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>


// Standard C++ Header Files
#include <chrono>
#include <string>
#include <vector>

using NoteList = std::vector<NoteModel_shp>;
using StaticQueryNote = boost::mysql::static_results<boost::mysql::pfr_by_name<NoteDbQueryValues>>;

class NoteQueryProcessor : public QueryProcessor<NoteModel, NoteDbQueryValues>
{
public:
    NoteQueryProcessor();
    virtual ~NoteQueryProcessor() = default;
    NoteModel_shp getNoteById(std::size_t noteId) noexcept;
    NoteList getAllNotesForUser(std::size_t userId) noexcept;
    NoteList getNotesForUserSimlarToContent(std::size_t userId, std::string likeContent) noexcept;
    NoteList getAllNotesForUserCreatedInDatgeRange(std::size_t userId,
        std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept;
    NoteList getAllNotesForUserEditedInDatgeRange(std::size_t userId,
        std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept;
    NoteList getDashboardNoteTable(std::size_t userId, std::chrono::year_month_day searchDate) noexcept;

private:
    std::string formatGetNotesFromUserForDate(std::size_t userId, std::chrono::year_month_day searchDate);

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetNoteByID() noexcept;
    TestStatus testExceptionsGetAllNotes() noexcept;
    TestStatus testExceptionsGetNotesForUserLikeContent() noexcept;
    TestStatus testExceptionsGetNotesForUserCreatedDateRange() noexcept;
    TestStatus testExceptionsGetNotesForUserEditedDateRange() noexcept;
    TestStatus testExceptionsGetDashboardNoteTable() noexcept;

    std::size_t m_noteIdx = IndexNotSet;
    std::size_t m_userIdx = IndexNotSet;
    std::size_t m_contentIdx = IndexNotSet;
    std::size_t m_hiddenIdx = IndexNotSet;
    std::size_t m_createdIdx = IndexNotSet;
    std::size_t m_lastmodIdx = IndexNotSet;
};

#endif // NOTEQUERYPROCESSOR_H_

