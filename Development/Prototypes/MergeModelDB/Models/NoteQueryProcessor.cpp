// Project Header Files
#include "QueryProcessor.h"
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
: CoreDBInterface()
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
        boost::mysql::results localResult = runQueryAsync(formatSelectByUserId(userId));
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
        boost::mysql::results localResult = runQueryAsync(formatSelectByUserIdAndSimilarContent(userId, likeContent));
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
        boost::mysql::results localResult = runQueryAsync(formatSelectByUserIdAndCreationDateRange(userId, startDate, endDate));
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
        boost::mysql::results localResult = runQueryAsync(formatSelectByUserIdAndUpdateDateRange(userId, startDate, endDate));
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

std::string NoteQueryProcessor::formatSelectByUserId(std::size_t userId)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "CALL GetAllUndeletedNotesForUser({})", userId);

    return std::move(fctx).get().value();
}

std::string NoteQueryProcessor::formatSelectByUserIdAndSimilarContent(std::size_t userId, std::string similarContent)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "CALL GetNotesForUserSimlarToContent({}, {})", userId, similarContent);

    return std::move(fctx).get().value();
}

std::string NoteQueryProcessor::formatSelectByUserIdAndCreationDateRange(
    std::size_t userId, std::chrono::year_month_day startDay, std::chrono::year_month_day endDay)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "CALL GetAllNotesForUserCreatedInDatgeRange({}, {}, {})", userId,
        stdchronoDateToBoostMySQLDate(startDay), stdchronoDateToBoostMySQLDate(endDay));

    return std::move(fctx).get().value();
}

std::string NoteQueryProcessor::formatSelectByUserIdAndUpdateDateRange(
    std::size_t userId, std::chrono::year_month_day startDay, std::chrono::year_month_day endDay)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "CALL GetAllNotesForUserEditedInDatgeRange({}, {}, {})", userId,
        stdchronoDateToBoostMySQLDate(startDay), stdchronoDateToBoostMySQLDate(endDay));

    return std::move(fctx).get().value();
}

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

/*
 * Map the column names to the indexes for the columns, the order of the columns
 * in the result set is not guarenteed. Ensure that the columns necessary to fill
 * the model are present. Currently there is no error if there are too many fields.
 */
void NoteQueryProcessor::mapColumnNameToIndex(boost::mysql::resultset_view &noteQueryresultSet)
{
    std::vector<std::string> columnNames;
    bool hasAllRequiredColumns = true;

    for (auto metaIter: noteQueryresultSet.meta())
    {
        columnNames.push_back(metaIter.column_name());
    }

    for (std::size_t i = 0; i < columnToIndexMap.size(); ++i)
    {
        std::string nameToFind = columnToIndexMap[i].columnName;
        auto iterToIndex = std::find(columnNames.begin(), columnNames.end(), nameToFind);
        if (iterToIndex != columnNames.end())
        {
            columnToIndexMap[i].columnIndex = std::distance(columnNames.begin(), iterToIndex);
        }
        else
        {
            appendErrorMessage(std::format("Required field {} not found in results", nameToFind));
            hasAllRequiredColumns = false;
        }
    }

    if (!hasAllRequiredColumns)
    {
        throw std::out_of_range("Results missing required fields");
    }

    fillRequiredIndexes();
}

void NoteQueryProcessor::assignValueToIndex(std::string columnName, std::size_t &columnIndex)
{
        auto iterToIndex = std::find_if(columnToIndexMap.begin(), columnToIndexMap.end(),
            [columnName](const ColumnNameToIndexmapping& ctim){ return ctim.columnName == columnName; });
        if (iterToIndex != columnToIndexMap.end())
        {
            if (iterToIndex->columnIndex.has_value())
            {
                columnIndex = iterToIndex->columnIndex.value();
            }
            else
            {
                appendErrorMessage(std::format("NULL Value to index for {}", columnName));
            }
        }
        else
        {
            appendErrorMessage(std::format("Column Name: {} NOT FOUND in assignValueToIndex()", columnName));
        }
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

/*
 * The boost::mysql::results class should contain all the results from the current
 * query, the results may span multiple result sets. The processResults method
 * should return all the results found in a single list.
 */
NoteList NoteQueryProcessor::processResults(boost::mysql::results &noteQueryResults)
{
    NoteList queryResultValues;

    for (std::size_t i = 0; i < noteQueryResults.size(); ++i)
    {
        boost::mysql::resultset_view resultview = noteQueryResults[i];
        NoteList intermediateResults = processResultSet(resultview);
        queryResultValues.insert(queryResultValues.end(), intermediateResults.begin(), intermediateResults.end());
    }

    return queryResultValues;
}

/*
 * Each boost::mysql result set contain a list of the column names in the result set.
 * Given the current implementation these columns should be in the same order for 
 * every result set, but we can't depend on that so we need to map the column names
 * to the indexes for every result set. For performance reasons we want to map the
 * indexes once per result set rather than doing a string search for each row of 
 * data. Create an object of the proper model for each row of data in the result set.
 */
NoteList NoteQueryProcessor::processResultSet(boost::mysql::resultset_view &noteQueryresultSet)
{
    NoteList intermediateResults;

    if (noteQueryresultSet.rows().num_columns() > 0)
    {
        mapColumnNameToIndex(noteQueryresultSet);
        for (auto rv: noteQueryresultSet.rows())
        {
            intermediateResults.push_back(processResultRow(rv));
        }
    }

    return intermediateResults;
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

#if 0
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
#endif

