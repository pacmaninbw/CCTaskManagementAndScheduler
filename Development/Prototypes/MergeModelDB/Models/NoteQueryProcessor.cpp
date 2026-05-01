// Project Header Files
#include "QueryProcessor.h"
#include "NoteQueryProcessor.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <string>

NoteQueryProcessor::NoteQueryProcessor()
: QueryProcessor<NoteModel>()
{

}

NoteQueryProcessorValues NoteQueryProcessor::getAllNotesForUser(std::size_t userId) noexcept
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectByUserId(userId);
        return runQueryFillNoteQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteQueryProcessorValues();
}

NoteQueryProcessorValues NoteQueryProcessor::getNotesForUserSimlarToContent(std::size_t userId, std::string likeContent) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In NoteQueryProcessor::getNotesForUserSimlarToContent : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectByUserIdAndSimilarContent(userId, likeContent);
        return runQueryFillNoteQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteQueryProcessorValues();
}

NoteQueryProcessorValues NoteQueryProcessor::getAllNotesForUserCreatedInDatgeRange(
    std::size_t userId, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUserCreatedInDatgeRange : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectByUserIdAndCreationDateRange(userId, startDate, endDate);
        return runQueryFillNoteQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteQueryProcessorValues();
}

NoteQueryProcessorValues NoteQueryProcessor::getAllNotesForUserEditedInDatgeRange(
    std::size_t userId, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In NoteQueryProcessor::getAllNotesForUserEditedInDatgeRange : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectByUserIdAndUpdateDateRange(userId, startDate, endDate);
        return runQueryFillNoteQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteQueryProcessorValues();
}

NoteQueryProcessorValues NoteQueryProcessor::getDashboardNoteTable(std::size_t userId, std::chrono::year_month_day searchDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In NoteQueryProcessor::getDashboardNoteTable : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatGetNotesFromUserForDate(userId, searchDate);
        return runQueryFillNoteQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteQueryProcessorValues();
}

NoteQueryProcessorValues NoteQueryProcessor::runQueryFillNoteQueryProcessor()
{
    if (firstFormattedQuery.empty())
    {
        appendErrorMessage(std::format("Formatting select multiple notes query string failed {}",
            queryGenerator->getAllErrorMessages()));
        return NoteQueryProcessorValues();
    }
    
    if (runFirstQuery())
    {
        return fillNoteQueryProcessor();
    }

    return NoteQueryProcessorValues();
}

NoteQueryProcessorValues NoteQueryProcessor::fillNoteQueryProcessor()
{
    NoteQueryProcessorValues NoteQueryProcessor;

    for (auto NoteID: primaryKeyResults)
    {
        NoteModel_shp newNote = std::make_shared<NoteModel>(NoteModel());
        newNote->selectByNoteID(NoteID);
        NoteQueryProcessor.push_back(newNote);
    }

    return NoteQueryProcessor;
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
