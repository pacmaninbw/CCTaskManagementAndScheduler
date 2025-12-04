// Project Header Files
#include "ListDBInterface.h"
#include "NoteList.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <string>

NoteList::NoteList()
: ListDBInterface<NoteModel>()
{

}

NoteListValues NoteList::getAllNotesForUser(std::size_t userId) noexcept
{
    errorMessages.clear();
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage("In NoteList::getAllNotesForUser : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectByUserId(userId);
        return runQueryFillNoteList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteListValues();
}

NoteListValues NoteList::getNotesForUserSimlarToContent(std::size_t userId, std::string likeContent) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In NoteList::getNotesForUserSimlarToContent : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectByUserIdAndSimilarContent(userId, likeContent);
        return runQueryFillNoteList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteListValues();
}

NoteListValues NoteList::getAllNotesForUserCreatedInDatgeRange(
    std::size_t userId, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In NoteList::getNotesForUserSimlarToContent : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectByUserIdAndCreationDateRange(userId, startDate, endDate);
        return runQueryFillNoteList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteListValues();
}

NoteListValues NoteList::getAllNotesForUserEditedInDatgeRange(
    std::size_t userId, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In NoteList::getNotesForUserSimlarToContent : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectByUserIdAndUpdateDateRange(userId, startDate, endDate);
        return runQueryFillNoteList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return NoteListValues();
}

NoteListValues NoteList::runQueryFillNoteList()
{
    if (firstFormattedQuery.empty())
    {
        appendErrorMessage(std::format("Formatting select multiple notes query string failed {}",
            queryGenerator->getAllErrorMessages()));
        return NoteListValues();
    }
    
    if (runFirstQuery())
    {
        return fillNoteList();
    }

    return NoteListValues();
}

NoteListValues NoteList::fillNoteList()
{
    NoteListValues NoteList;

    for (auto NoteID: primaryKeyResults)
    {
        NoteModel_shp newNote = std::make_shared<NoteModel>(NoteModel());
        newNote->selectByNoteID(NoteID);
        NoteList.push_back(newNote);
    }

    return NoteList;
}

std::vector<ListExceptionTestElement> NoteList::initListExceptionTests() noexcept 
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&NoteList::testExceptionsGetAllNotes, this), "getAllNotesForUser"});
    exceptionTests.push_back({std::bind(&NoteList::testExceptionsGetNotesForUserLikeContent, this), "getNotesForUserSimlarToContent"});
    exceptionTests.push_back({std::bind(&NoteList::testExceptionsGetNotesForUserCreatedDateRange, this), "getAllNotesForUserCreatedInDatgeRange"});
    exceptionTests.push_back({std::bind(&NoteList::testExceptionsGetNotesForUserEditedDateRange, this), "getAllNotesForUserEditedInDatgeRange"});

    return exceptionTests;
}

TestStatus NoteList::testExceptionsGetAllNotes() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("NoteList::testExceptionsGetAllNotes()",
         std::bind(&NoteList::getAllNotesForUser, this, std::placeholders::_1), 1);
}

TestStatus NoteList::testExceptionsGetNotesForUserLikeContent() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::string testContent("Test Content");

    return testListExceptionAndSuccessNArgs("NoteList::testExceptionsGetNotesForUserLikeContent()",
         std::bind(&NoteList::getNotesForUserSimlarToContent, this, std::placeholders::_1, std::placeholders::_2), 
        testUserId, testContent);
}

TestStatus NoteList::testExceptionsGetNotesForUserCreatedDateRange() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::chrono::year_month_day startDate = getTodaysDateMinus(OneWeek);
    std::chrono::year_month_day endDate = getTodaysDate();

    return testListExceptionAndSuccessNArgs("NoteList::testExceptionsGetNotesForUserCreatedDateRange()",
        std::bind(&NoteList::getAllNotesForUserCreatedInDatgeRange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
        testUserId, startDate, endDate);
  }

TestStatus NoteList::testExceptionsGetNotesForUserEditedDateRange() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;
    std::chrono::year_month_day startDate = getTodaysDateMinus(OneWeek);
    std::chrono::year_month_day endDate = getTodaysDate();

    return testListExceptionAndSuccessNArgs("NoteList::testExceptionsGetNotesForUserEditedDateRange()",
        std::bind(&NoteList::getAllNotesForUserEditedInDatgeRange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
        testUserId, startDate, endDate);
}
