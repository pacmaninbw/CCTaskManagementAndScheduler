// Project Header Files
#include "QueryProcessor.h"
#include "ScheduleItemQueryProcessor.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

ScheduleItemQueryProcessor::ScheduleItemQueryProcessor(std::size_t userId)
: QueryProcessor<ScheduleItemModel>()
{
    userID = userId;
}

ScheduleItemQueryProcessorValues ScheduleItemQueryProcessor::getUserDaySchedule(std::chrono::year_month_day scheduleDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In ScheduleItemQueryProcessor::getUserDaySchedule : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectScheduleItemsByDateAndUser(scheduleDate, userID);
        return runQueryFillScheduleItemQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return ScheduleItemQueryProcessorValues();
}

ScheduleItemQueryProcessorValues ScheduleItemQueryProcessor::findUserScheduleItemsByContentAndDateRange(
    std::string searchTitle,
    std::chrono::year_month_day searchStart,
    std::chrono::year_month_day searchEnd
) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In ScheduleItemQueryProcessor::findUserScheduleItemsByContentAndDateRange : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectSiByContentDateRangeUser(searchTitle, searchStart, searchEnd, userID);
        return runQueryFillScheduleItemQueryProcessor();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return ScheduleItemQueryProcessorValues();
}

std::vector<std::string> ScheduleItemQueryProcessor::findEventSToRepeat(std::string searchTitle) noexcept
{
    errorMessages.clear();
    std::vector<std::string> matchingEvents;
    appendErrorMessage("In ScheduleItemQueryProcessor::findEventSToRepeat : ");

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

std::vector<std::string> ScheduleItemQueryProcessor::findEventsForRepeatCompletion() noexcept
{
    errorMessages.clear();
    std::vector<std::string> matchingEvents;
    appendErrorMessage("In ScheduleItemQueryProcessor::findEventsForRepeatCompletion : ");

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

std::vector<std::string> ScheduleItemQueryProcessor::findLocationsForRepeatCompletion() noexcept
{
    errorMessages.clear();
    std::vector<std::string> matchingLocations;
    appendErrorMessage("In ScheduleItemQueryProcessor::findLocationsForRepeatCompletion : ");

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

ScheduleItemQueryProcessorValues ScheduleItemQueryProcessor::fillScheduleItemQueryProcessor()
{
    ScheduleItemQueryProcessorValues ScheduleItemQueryProcessor;

    for (auto scheduleItemID: primaryKeyResults)
    {
        ScheduleItemModel_shp newScheduleItem = std::make_shared<ScheduleItemModel>(ScheduleItemModel());
        newScheduleItem->setScheduleItemID(scheduleItemID);
        newScheduleItem->setUserID(userID);
        newScheduleItem->retrieve();
        ScheduleItemQueryProcessor.push_back(newScheduleItem);
    }

    return ScheduleItemQueryProcessor;
}

ScheduleItemQueryProcessorValues ScheduleItemQueryProcessor::runQueryFillScheduleItemQueryProcessor()
{
    if (firstFormattedQuery.empty())
    {
        appendErrorMessage(std::format("Formatting select multiple schedule items query string failed {}",
            queryGenerator->getAllErrorMessages()));
        return ScheduleItemQueryProcessorValues();
    }
    
    if (runFirstQuery())
    {
        return fillScheduleItemQueryProcessor();
    }

    return ScheduleItemQueryProcessorValues();
}

std::vector<ListExceptionTestElement> ScheduleItemQueryProcessor::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&ScheduleItemQueryProcessor::testExceptionGetUserDaySchedule, this), "getUserDaySchedule"});
    exceptionTests.push_back({std::bind(&ScheduleItemQueryProcessor::testExceptionFindUserScheduleItemsByContentAndDateRange, this),
        "findUserScheduleItemsByContentAndDateRange"});
    exceptionTests.push_back({std::bind(&ScheduleItemQueryProcessor::testExceptionFindEventSToRepeat, this), "findEventSToRepeat"});
    exceptionTests.push_back({std::bind(&ScheduleItemQueryProcessor::testExceptionFindEventsForRepeatCompletion, this), "findEventsToRepeatCompletion"});
    exceptionTests.push_back({std::bind(&ScheduleItemQueryProcessor::testExceptionFindLocationsForRepeatCompletion, this),
        "findLocationsForRepeatCompletion"});

    return exceptionTests;
}

TestStatus ScheduleItemQueryProcessor::testExceptionGetUserDaySchedule() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("ScheduleItemQueryProcessor::testExceptionGgetUserDaySchedule()",
         std::bind(&ScheduleItemQueryProcessor::getUserDaySchedule, this, std::placeholders::_1), commonTestDateValue);
}

TestStatus ScheduleItemQueryProcessor::testExceptionFindUserScheduleItemsByContentAndDateRange() noexcept
{
    selfTestResetAllValues();

    std::string titleSearch("Title search");
    std::chrono::year_month_day testStart(commonTestDateRangeStartValue);
    std::chrono::year_month_day testEnd(commonTestDateRangeEndValue);

    return testListExceptionAndSuccessNArgs("ScheduleItemQueryProcessor::testExceptionFindUserScheduleItemsByContentAndDateRange()",
         std::bind(&ScheduleItemQueryProcessor::findUserScheduleItemsByContentAndDateRange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
        titleSearch, testStart, testEnd);
}

TestStatus ScheduleItemQueryProcessor::testExceptionFindEventSToRepeat() noexcept
{
    selfTestResetAllValues();

    std::string titleSearch("Title search");

    return testListExceptionAndSuccessNArgs("ScheduleItemQueryProcessor::testExceptionFindEventSToRepeat()",
         std::bind(&ScheduleItemQueryProcessor::findEventSToRepeat, this, std::placeholders::_1), titleSearch);
}

TestStatus ScheduleItemQueryProcessor::testExceptionFindEventsForRepeatCompletion() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("ScheduleItemQueryProcessor::testExceptionFindEventsForRepeatCompletion()",
         std::bind(&ScheduleItemQueryProcessor::findEventsForRepeatCompletion, this));
}

TestStatus ScheduleItemQueryProcessor::testExceptionFindLocationsForRepeatCompletion() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("ScheduleItemQueryProcessor::testExceptionFindLocationsForRepeatCompletion()",
         std::bind(&ScheduleItemQueryProcessor::findLocationsForRepeatCompletion, this));
}
