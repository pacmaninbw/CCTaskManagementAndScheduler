// Project Header Files
#include "QueryProcessor.h"
#include "ScheduleItemQueryProcessor.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

ScheduleItemQueryProcessor::ScheduleItemQueryProcessor(std::size_t userId)
: QueryProcessor<ScheduleItemModel>("ScheduleItem", {"idUserScheduleItem", "UserID", "StartDateTime", "EndDateTime", "Title", "Personal", "Location", "CreatedTS", "LastUpdateTS", "Hidden"})
{
    userID = userId;
}

ScheduleItemModel_shp ScheduleItemQueryProcessor::getScheduleItemById(std::size_t eventId) noexcept
{
    errorMessages.clear();
    ScheduleItemModel_shp found = nullptr;

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetScheduleItemById({})", eventId);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        found = getOneResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In ScheduleItemQueryProcessor::{}({}) : {}", __func__, eventId, e.what()));
    }

    return found;
}

ScheduleItemList ScheduleItemQueryProcessor::getUserDaySchedule(std::chrono::year_month_day scheduleDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In ScheduleItemQueryProcessor::getUserDaySchedule : ");

    try
    {
        const int secondsInHour = 3600;
        const int secondsInDay = secondsInHour * 24;
        std::chrono::seconds dateAdjustor(secondsInDay - 1);

        std::chrono::system_clock::time_point startSearch = getLocalMidnight(scheduleDate);
        std::chrono::system_clock::time_point endSearch = startSearch + dateAdjustor;

        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL GetUserDaySchedule({}, {}, {})",
            userID, stdChronoTimePointToBoostDateTime(startSearch), stdChronoTimePointToBoostDateTime(endSearch));
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return ScheduleItemList();
}

ScheduleItemList ScheduleItemQueryProcessor::findUserScheduleItemsByContentAndDateRange(
    std::string searchTitle,
    std::chrono::year_month_day searchStart,
    std::chrono::year_month_day searchEnd
) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In ScheduleItemQueryProcessor::findUserScheduleItemsByContentAndDateRange : ");

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL FindUserScheduleItemsByContentAndDateRange({}, {}, {}, {})",
            userID, searchTitle, stdchronoDateToBoostMySQLDate(searchStart),
            stdchronoDateToBoostMySQLDate(searchEnd));
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());
        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return ScheduleItemList();
}

std::vector<std::string> ScheduleItemQueryProcessor::findEventSToRepeat(std::string searchTitle) noexcept
{
    errorMessages.clear();
    std::vector<std::string> matchingEvents;
    appendErrorMessage("In ScheduleItemQueryProcessor::findEventSToRepeat : ");

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL ScheduleItemContentSelectionList({}, {})", searchTitle, userID);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL EventTitlesForCompleter({})", userID);

        if (runStringOnlyQuery(std::move(fctx).get().value()))
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
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL EventLocationsForCompleter({})", userID);

        if (runStringOnlyQuery(std::move(fctx).get().value()))
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

ScheduleItemModel_shp ScheduleItemQueryProcessor::processResultRow(boost::mysql::row_view &queryRow)
{
    std::size_t scheduleId = queryRow.at(scheduleItemIdIdx).as_uint64();
    std::size_t userID = queryRow.at(userIdIdx).as_uint64();
    std::chrono::system_clock::time_point startTime = boostMysqlDateTimeToChronoTimePoint(queryRow.at(startTimeIdx).as_datetime());
    std::chrono::system_clock::time_point endTime = boostMysqlDateTimeToChronoTimePoint(queryRow.at(endTimeIdx).as_datetime());
    std::string title = queryRow.at(titleIdx).as_string();
    std::string location;
    if (!queryRow.at(locationIdx).is_null())
    {
        location = queryRow.at(locationIdx).as_string();
    }
    std::chrono::system_clock::time_point creationTimeStamp = boostMysqlDateTimeToChronoTimePoint(queryRow.at(createdOnIdx).as_datetime());
    std::chrono::system_clock::time_point lastUpdate = boostMysqlDateTimeToChronoTimePoint(queryRow.at(lastUpdate_Idx).as_datetime());
    bool personal = queryRow.at(personalIdx).as_int64();

    return std::make_shared<ScheduleItemModel>(scheduleId, userID, title, startTime, endTime, location, personal, creationTimeStamp, lastUpdate);

}

void ScheduleItemQueryProcessor::fillRequiredIndexes()
{
    assignValueToIndex("idUserScheduleItem", scheduleItemIdIdx);
    assignValueToIndex("UserID", userIdIdx);
    assignValueToIndex("StartDateTime", startTimeIdx);
    assignValueToIndex("EndDateTime", endTimeIdx);
    assignValueToIndex("Title", titleIdx);
    assignValueToIndex("Personal", personalIdx);
    assignValueToIndex("Location", locationIdx);
    assignValueToIndex("CreatedTS", createdOnIdx);
    assignValueToIndex("LastUpdateTS", lastUpdate_Idx);
    assignValueToIndex("Hidden", hidden_Idx);
}

std::vector<ListExceptionTestElement> ScheduleItemQueryProcessor::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&ScheduleItemQueryProcessor::testExceptionGetUserDaySchedule, this), "getUserDaySchedule"});
    exceptionTests.push_back({std::bind(&ScheduleItemQueryProcessor::testExceptionGetScheduleItemByID, this), "getScheduleItemById"});
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

TestStatus ScheduleItemQueryProcessor::testExceptionGetScheduleItemByID() noexcept
{
    selfTestResetAllValues();

    std::size_t eventId = 1;

    return testExceptionAndSuccessNArgs("ScheduleItemQueryProcessor::testExceptionGetScheduleItemByID()",
         std::bind(&ScheduleItemQueryProcessor::getScheduleItemById, this, std::placeholders::_1), 
        eventId);
}
