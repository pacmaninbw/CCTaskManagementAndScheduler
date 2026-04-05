// Project Header Files
#include "ListDBInterface.h"
#include "ScheduleItemList.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

ScheduleItemList::ScheduleItemList(std::size_t userId)
: ListDBInterface<ScheduleItemModel>()
{
    userID = userId;
}

ScheduleItemListValues ScheduleItemList::getUserDaySchedule(std::chrono::year_month_day scheduleDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In ScheduleItemList::getUserDaySchedule : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectScheduleItemsByDateAndUser(scheduleDate, userID);
        return runQueryFillScheduleItemList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return ScheduleItemListValues();
}

ScheduleItemListValues ScheduleItemList::findUserScheduleItemsByContentAndDateRange(
    std::string searchTitle,
    std::chrono::year_month_day searchStart,
    std::chrono::year_month_day searchEnd
) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In ScheduleItemList::findUserScheduleItemsByContentAndDateRange : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectSiByContentDateRangeUser(searchTitle, searchStart, searchEnd, userID);
        return runQueryFillScheduleItemList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return ScheduleItemListValues();
}

std::vector<std::string> ScheduleItemList::findEventSToRepeat(std::string searchTitle) noexcept
{
    errorMessages.clear();
    std::vector<std::string> matchingEvents;
    appendErrorMessage("In ScheduleItemList::findEventSToRepeat : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatGetUniqueContentsByUserSortByContent(searchTitle, userID);
        if (firstFormattedQuery.empty())
        {
            appendErrorMessage(std::format("Formatting stored procedure query string failed {}",
                queryGenerator->getAllErrorMessages()));

            return matchingEvents;
        }

        boost::mysql::results localResult = runQueryAsync(firstFormattedQuery);
        if (!inSelfTest)
        {
            // localresult will abort with an assert in self test
            if (!localResult.rows().empty())
            {
                for (auto row: localResult.rows())
                {
                    matchingEvents.push_back(row.at(0).as_string());    
                }
            }
        }
        else
        {
            if (!forceException)
            {
                matchingEvents.push_back("Test String");
            }
        }

        errorMessages.clear();
        return matchingEvents;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return matchingEvents;
}

std::vector<std::string> ScheduleItemList::findEventsForRepeatCompletion() noexcept
{
    errorMessages.clear();
    std::vector<std::string> matchingEvents;
    appendErrorMessage("In ScheduleItemList::findEventsForRepeatCompletion : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatGetAllUniqueContentsByUserSortByContent(userID);
        if (firstFormattedQuery.empty())
        {
            appendErrorMessage(std::format("Formatting stored procedure query string failed {}",
                queryGenerator->getAllErrorMessages()));

            return matchingEvents;
        }

        if (runStringOnlyQuery())
        {
            matchingEvents = stringOnlyResults;
        }

        errorMessages.clear();
        return matchingEvents;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return matchingEvents;
}

std::vector<std::string> ScheduleItemList::findLocationsForRepeatCompletion() noexcept
{
    errorMessages.clear();
    std::vector<std::string> matchingLocations;
    appendErrorMessage("In ScheduleItemList::findLocationsForRepeatCompletion : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatGetAllUniqueLocationsByUserSortByContent(userID);
        if (firstFormattedQuery.empty())
        {
            appendErrorMessage(std::format("Formatting stored procedure query string failed {}",
                queryGenerator->getAllErrorMessages()));

            return matchingLocations;
        }

        if (runStringOnlyQuery())
        {
            matchingLocations = stringOnlyResults;
        }

        errorMessages.clear();
        return matchingLocations;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return matchingLocations;
}

ScheduleItemListValues ScheduleItemList::fillScheduleItemList()
{
    ScheduleItemListValues scheduleItemList;

    for (auto scheduleItemID: primaryKeyResults)
    {
        ScheduleItemModel_shp newScheduleItem = std::make_shared<ScheduleItemModel>(ScheduleItemModel());
        newScheduleItem->setScheduleItemID(scheduleItemID);
        newScheduleItem->setUserID(userID);
        newScheduleItem->retrieve();
        scheduleItemList.push_back(newScheduleItem);
    }

    return scheduleItemList;
}

ScheduleItemListValues ScheduleItemList::runQueryFillScheduleItemList()
{
    if (firstFormattedQuery.empty())
    {
        appendErrorMessage(std::format("Formatting select multiple schedule items query string failed {}",
            queryGenerator->getAllErrorMessages()));
        return ScheduleItemListValues();
    }
    
    if (runFirstQuery())
    {
        return fillScheduleItemList();
    }

    return ScheduleItemListValues();
}

std::vector<ListExceptionTestElement> ScheduleItemList::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&ScheduleItemList::testExceptionGetUserDaySchedule, this), "getUserDaySchedule"});
    exceptionTests.push_back({std::bind(&ScheduleItemList::testExceptionFindUserScheduleItemsByContentAndDateRange, this),
        "findUserScheduleItemsByContentAndDateRange"});
    exceptionTests.push_back({std::bind(&ScheduleItemList::testExceptionFindEventSToRepeat, this), "findEventSToRepeat"});
    exceptionTests.push_back({std::bind(&ScheduleItemList::testExceptionFindEventsForRepeatCompletion, this), "findEventsToRepeatCompletion"});
    exceptionTests.push_back({std::bind(&ScheduleItemList::testExceptionFindLocationsForRepeatCompletion, this),
        "findLocationsForRepeatCompletion"});

    return exceptionTests;
}

TestStatus ScheduleItemList::testExceptionGetUserDaySchedule() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("ScheduleItemList::testExceptionGgetUserDaySchedule()",
         std::bind(&ScheduleItemList::getUserDaySchedule, this, std::placeholders::_1), commonTestDateValue);
}

TestStatus ScheduleItemList::testExceptionFindUserScheduleItemsByContentAndDateRange() noexcept
{
    selfTestResetAllValues();

    std::string titleSearch("Title search");
    std::chrono::year_month_day testStart(commonTestDateRangeStartValue);
    std::chrono::year_month_day testEnd(commonTestDateRangeEndValue);

    return testListExceptionAndSuccessNArgs("ScheduleItemList::testExceptionFindUserScheduleItemsByContentAndDateRange()",
         std::bind(&ScheduleItemList::findUserScheduleItemsByContentAndDateRange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
        titleSearch, testStart, testEnd);
}

TestStatus ScheduleItemList::testExceptionFindEventSToRepeat() noexcept
{
    selfTestResetAllValues();

    std::string titleSearch("Title search");

    return testListExceptionAndSuccessNArgs("ScheduleItemList::testExceptionFindEventSToRepeat()",
         std::bind(&ScheduleItemList::findEventSToRepeat, this, std::placeholders::_1), titleSearch);
}

TestStatus ScheduleItemList::testExceptionFindEventsForRepeatCompletion() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("ScheduleItemList::testExceptionFindEventsForRepeatCompletion()",
         std::bind(&ScheduleItemList::findEventsForRepeatCompletion, this));
}

TestStatus ScheduleItemList::testExceptionFindLocationsForRepeatCompletion() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("ScheduleItemList::testExceptionFindLocationsForRepeatCompletion()",
         std::bind(&ScheduleItemList::findLocationsForRepeatCompletion, this));
}
