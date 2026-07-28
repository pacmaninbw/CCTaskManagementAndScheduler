// Project Header Files
#include "commonUtilities.h"
#include "NoteQueryProcessor.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

NoteQueryProcessor::NoteQueryProcessor()
: QueryProcessor<NoteModel, NoteDbQueryValues>("NoteModel")
{
}

NoteModel_shp NoteQueryProcessor::getNoteById(std::size_t noteId) noexcept
{
    clearErrorMessages();

    NoteModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_notes  WHERE user_notes.id_user_notes = {}", noteId);
        StaticQueryNote localResult = staticRunQueryAsync<NoteDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In NoteQueryProcessor::{}({}) : {}", __func__, noteId, e.what()));
    }

    return found;
}

NoteList NoteQueryProcessor::getAllNotesForUser(std::size_t userId) noexcept
{
    clearErrorMessages();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUser : ");

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_notes ");
        boost::mysql::format_sql_to(fctx, "WHERE user_notes.user_id = {} ", userId);
        boost::mysql::format_sql_to(fctx, "AND user_notes.deleted <> 1");

        StaticQueryNote localResult = staticRunQueryAsync<NoteDbQueryValues>(std::move(fctx).get().value());

        return processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteList();
}

NoteList NoteQueryProcessor::getNotesForUserSimlarToContent(std::size_t userId, std::string likeContent) noexcept
{
    clearErrorMessages();
    appendErrorMessage("In NoteQueryProcessor::getNotesForUserSimlarToContent : ");

    try
    {
        std::string contentPattern = wrapSearchContentSQLPatternMatch(likeContent);

        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_notes ");
        boost::mysql::format_sql_to(fctx, "WHERE user_notes.user_id = {} ", userId);
        boost::mysql::format_sql_to(fctx, "AND user_notes.content LIKE {} ", contentPattern);
        boost::mysql::format_sql_to(fctx, "AND user_notes.deleted <> 1");

        StaticQueryNote localResult = staticRunQueryAsync<NoteDbQueryValues>(std::move(fctx).get().value());

        return processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteList();
}

NoteList NoteQueryProcessor::getAllNotesForUserCreatedInDatgeRange(
    std::size_t userId, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept
{
    clearErrorMessages();
    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUserCreatedInDatgeRange : ");

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_notes ");
        boost::mysql::format_sql_to(fctx, "WHERE user_notes.user_id = {} ", userId);
        boost::mysql::format_sql_to(fctx, "AND user_notes.note_creation >= {} ", stdchronoDateToBoostMySQLDate(startDate));
        boost::mysql::format_sql_to(fctx, "AND user_notes.note_creation <= {} ", stdchronoDateToBoostMySQLDate(endDate));
        boost::mysql::format_sql_to(fctx, "AND user_notes.deleted <> 1");

        StaticQueryNote localResult = staticRunQueryAsync<NoteDbQueryValues>(std::move(fctx).get().value());

        return processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteList();
}

NoteList NoteQueryProcessor::getAllNotesForUserEditedInDatgeRange(
    std::size_t userId, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept
{
    clearErrorMessages();
    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUserEditedInDatgeRange : ");

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_notes ");
        boost::mysql::format_sql_to(fctx, "WHERE user_notes.user_id = {} ", userId);
        boost::mysql::format_sql_to(fctx, "AND user_notes.last_modifed >= {} ", stdchronoDateToBoostMySQLDate(startDate));
        boost::mysql::format_sql_to(fctx, "AND user_notes.last_modifed <= {} ", stdchronoDateToBoostMySQLDate(endDate));
        boost::mysql::format_sql_to(fctx, "AND user_notes.deleted <> 1");

        StaticQueryNote localResult = staticRunQueryAsync<NoteDbQueryValues>(std::move(fctx).get().value());

        return processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteList();
}

NoteList NoteQueryProcessor::getDashboardNoteTable(std::size_t userId, std::chrono::year_month_day searchDate) noexcept
{
    clearErrorMessages();
    appendErrorMessage("In NoteQueryProcessor::getDashboardNoteTable : ");

    try
    {
        StaticQueryNote localResult = staticRunQueryAsync<NoteDbQueryValues>(formatGetNotesFromUserForDate(userId, searchDate));
        return processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteList();
}

/*
 * Get the notes for the date requested. Adjust for local time to GMT.
 */
std::string NoteQueryProcessor::formatGetNotesFromUserForDate(std::size_t userId, std::chrono::year_month_day searchDate)
{
    std::chrono::system_clock::time_point startDay(getLocalMidnight(searchDate));
    std::chrono::hours mostHoursInDay{23};
    std::chrono::minutes minutesInLastHour{59};
    std::chrono::system_clock::duration oneDayOffset = mostHoursInDay + minutesInLastHour;
    std::chrono::system_clock::time_point endDay(startDay + oneDayOffset);

    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "SELECT * FROM user_notes ");
    boost::mysql::format_sql_to(fctx, "WHERE user_notes.user_id = {} ", userId);
    boost::mysql::format_sql_to(fctx, "AND user_notes.note_creation >= {} ", stdChronoTimePointToBoostDateTime(startDay));
    boost::mysql::format_sql_to(fctx, "AND user_notes.note_creation <= {} ", stdChronoTimePointToBoostDateTime(endDay));
    boost::mysql::format_sql_to(fctx, "AND user_notes.deleted <> 1 ");
    boost::mysql::format_sql_to(fctx, "ORDER BY user_notes.note_creation ASC");

    return std::move(fctx).get().value();
}

std::vector<ListExceptionTestElement> NoteQueryProcessor::initListExceptionTests() noexcept 
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionGetNoteByID, this), "getNoteById"});
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetAllNotes, this), "getAllNotesForUser"});
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetNotesForUserLikeContent, this), "getNotesForUserSimlarToContent"});
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetNotesForUserCreatedDateRange, this), "getAllNotesForUserCreatedInDatgeRange"});
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetNotesForUserEditedDateRange, this), "getAllNotesForUserEditedInDatgeRange"});
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetDashboardNoteTable, this), "testExceptionsGetDashboardNoteTable"});

    return exceptionTests;
}

TestStatus NoteQueryProcessor::testExceptionGetNoteByID() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("NoteQueryProcessor::getNoteById",
         std::bind(&NoteQueryProcessor::getNoteById, this, std::placeholders::_1), 1);
}

TestStatus NoteQueryProcessor::testExceptionsGetAllNotes() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("NoteQueryProcessor::testExceptionsGetAllNotes()",
         std::bind(&NoteQueryProcessor::getAllNotesForUser, this, std::placeholders::_1), 1);
}

TestStatus NoteQueryProcessor::testExceptionsGetNotesForUserLikeContent() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::string testContent("Test Content");

    return testListExceptionAndSuccessNArgs("NoteQueryProcessor::testExceptionsGetNotesForUserLikeContent()",
         std::bind(&NoteQueryProcessor::getNotesForUserSimlarToContent, this, std::placeholders::_1, std::placeholders::_2), 
        testUserId, testContent);
}

TestStatus NoteQueryProcessor::testExceptionsGetNotesForUserCreatedDateRange() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::chrono::year_month_day startDate = commonTestDateRangeStartValue;
    std::chrono::year_month_day endDate = commonTestDateValue;

    return testListExceptionAndSuccessNArgs("NoteQueryProcessor::testExceptionsGetNotesForUserCreatedDateRange()",
        std::bind(&NoteQueryProcessor::getAllNotesForUserCreatedInDatgeRange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
        testUserId, startDate, endDate);
  }

TestStatus NoteQueryProcessor::testExceptionsGetNotesForUserEditedDateRange() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::chrono::year_month_day startDate = commonTestDateRangeStartValue;
    std::chrono::year_month_day endDate = commonTestDateValue;

    return testListExceptionAndSuccessNArgs("NoteQueryProcessor::testExceptionsGetNotesForUserEditedDateRange()",
        std::bind(&NoteQueryProcessor::getAllNotesForUserEditedInDatgeRange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
        testUserId, startDate, endDate);
}

TestStatus NoteQueryProcessor::testExceptionsGetDashboardNoteTable() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::chrono::year_month_day searchDate = commonTestDateValue;

    return testListExceptionAndSuccessNArgs("NoteQueryProcessor::testExceptionsGetDashboardNoteTable()",
        std::bind(&NoteQueryProcessor::getDashboardNoteTable, this, std::placeholders::_1, std::placeholders::_2), 
        testUserId, searchDate);
}

