// Project Header Files
#include "NoteQueryProcessor.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <algorithm>
#include <chrono>
#include <exception>
#include <format>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

NoteQueryProcessor::NoteQueryProcessor()
: QueryProcessor<NoteModel>("NoteModel")
{
    requiredColumns =  {"idUserNotes", "UserID", "Content", "Hidden", "NotationDateTime",  "LastUpdate"};
    for (auto columnName: requiredColumns)
    {
        columnToIndexMap.push_back(columnName);
    }
}

NoteList NoteQueryProcessor::getAllNotesForUser(std::size_t userId) noexcept
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUser : ");

    try
    {
        initFormatOptions();
        boost::mysql::results localResult = runQueryAsync(boost::mysql::format_sql(
            format_opts.value(), "CALL GetAllUndeletedNotesForUser({})", userId));
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteList();
}

NoteList NoteQueryProcessor::getNotesForUserSimlarToContent(std::size_t userId, std::string likeContent) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In NoteQueryProcessor::getNotesForUserSimlarToContent : ");

    try
    {
        initFormatOptions();
        boost::mysql::results localResult = runQueryAsync(
            boost::mysql::format_sql(format_opts.value(), 
                "CALL GetNotesForUserSimlarToContent({}, {})", userId, likeContent
        ));
        return processResults(localResult);
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
    errorMessages.clear();
    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUserCreatedInDatgeRange : ");

    try
    {
        initFormatOptions();
        boost::mysql::results localResult = runQueryAsync(
            boost::mysql::format_sql(format_opts.value(),
                "CALL GetAllNotesForUserCreatedInDatgeRange({}, {}, {})", userId,
                stdchronoDateToBoostMySQLDate(startDate), stdchronoDateToBoostMySQLDate(endDate)
        ));
        return processResults(localResult);
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
    errorMessages.clear();
    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUserEditedInDatgeRange : ");

    try
    {
        initFormatOptions();
        boost::mysql::results localResult = runQueryAsync(
            boost::mysql::format_sql(format_opts.value(),
                "CALL GetAllNotesForUserEditedInDatgeRange({}, {}, {})", userId,
                stdchronoDateToBoostMySQLDate(startDate), stdchronoDateToBoostMySQLDate(endDate))
        );
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteList();
}

NoteList NoteQueryProcessor::getDashboardNoteTable(std::size_t userId, std::chrono::year_month_day searchDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In NoteQueryProcessor::getDashboardNoteTable : ");

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatGetNotesFromUserForDate(userId, searchDate));
        return processResults(localResult);
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

    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "CALL GetDashboardNoteTable({}, {}, {})", userId,
        stdChronoTimePointToBoostDateTime(startDay), stdChronoTimePointToBoostDateTime(endDay));

    return std::move(fctx).get().value();
}

void NoteQueryProcessor::fillRequiredIndexes()
{
    assignValueToIndex("idUserNotes", noteIDX);
    assignValueToIndex("UserID", userIDX);
    assignValueToIndex("Content", contentIDX);
    assignValueToIndex("Hidden", hiddenIDX);
    assignValueToIndex("NotationDateTime", createdIDX);
    assignValueToIndex("LastUpdate", lastmodIDX);
}

NoteModel_shp NoteQueryProcessor::processResultRow(boost::mysql::row_view &noteQueryRow)
{
    std::size_t noteId = noteQueryRow.at(noteIDX).as_uint64();
    std::size_t userID = noteQueryRow.at(userIDX).as_uint64();
    std::string content = noteQueryRow.at(contentIDX).as_string();
    bool deleted = false;
    if (!noteQueryRow.at(hiddenIDX).is_null())
    {
        deleted = noteQueryRow.at(hiddenIDX).as_int64() == 1? true : false;
    }

    std::chrono::system_clock::time_point creationDate = boostMysqlDateTimeToChronoTimePoint(noteQueryRow.at(createdIDX).as_datetime());
    std::chrono::system_clock::time_point lastUpdate = boostMysqlDateTimeToChronoTimePoint(noteQueryRow.at(lastmodIDX).as_datetime());

    NoteModel_shp noteListMember = std::make_shared<NoteModel>(noteId, userID, content, creationDate, lastUpdate, deleted);

    return noteListMember;
}

std::vector<ListExceptionTestElement> NoteQueryProcessor::initListExceptionTests() noexcept 
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetAllNotes, this), "getAllNotesForUser"});
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetNotesForUserLikeContent, this), "getNotesForUserSimlarToContent"});
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetNotesForUserCreatedDateRange, this), "getAllNotesForUserCreatedInDatgeRange"});
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetNotesForUserEditedDateRange, this), "getAllNotesForUserEditedInDatgeRange"});
    exceptionTests.push_back({std::bind(&NoteQueryProcessor::testExceptionsGetDashboardNoteTable, this), "testExceptionsGetDashboardNoteTable"});

    return exceptionTests;
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

