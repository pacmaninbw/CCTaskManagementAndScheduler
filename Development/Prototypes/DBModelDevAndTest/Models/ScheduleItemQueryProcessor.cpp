// Project Header Files
#include "commonUtilities.h"
#include "QueryProcessor.h"
#include "ScheduleItemQueryProcessor.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

ScheduleItemQueryProcessor::ScheduleItemQueryProcessor(std::size_t userId)
: QueryProcessor<ScheduleItemModel>("ScheduleItem", 
        {
            "idUserScheduleItem", "UserID", "StartDateTime", "EndDateTime", "Title",
            "Personal", "Location", "CreatedTS", "LastUpdateTS", "Hidden"
        }
    )
{
    m_UserID = userId;
}

ScheduleItemModel_shp ScheduleItemQueryProcessor::getScheduleItemById(std::size_t eventId) noexcept
{
    clearErrorMessages();
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
    clearErrorMessages();
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
            m_UserID, stdChronoTimePointToBoostDateTime(startSearch), stdChronoTimePointToBoostDateTime(endSearch));
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
    clearErrorMessages();
    appendErrorMessage("In ScheduleItemQueryProcessor::findUserScheduleItemsByContentAndDateRange : ");

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL FindUserScheduleItemsByContentAndDateRange({}, {}, {}, {})",
            m_UserID, searchTitle, stdchronoDateToBoostMySQLDate(searchStart),
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
    clearErrorMessages();
    std::vector<std::string> matchingEvents;
    appendErrorMessage("In ScheduleItemQueryProcessor::findEventSToRepeat : ");

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL ScheduleItemContentSelectionList({}, {})", searchTitle, m_UserID);
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

        if (!m_SelfTest)
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
            if (!m_ForceException)
            {
                matchingEvents.push_back("Test String");
            }
        }

        clearErrorMessages();
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
    clearErrorMessages();
    std::vector<std::string> matchingEvents;
    appendErrorMessage("In ScheduleItemQueryProcessor::findEventsForRepeatCompletion : ");

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL EventTitlesForCompleter({})", m_UserID);

        if (runStringOnlyQuery(std::move(fctx).get().value()))
        {
            matchingEvents = stringOnlyResults;
        }

        clearErrorMessages();
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
    clearErrorMessages();
    std::vector<std::string> matchingLocations;
    appendErrorMessage("In ScheduleItemQueryProcessor::findLocationsForRepeatCompletion : ");

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, "CALL EventLocationsForCompleter({})", m_UserID);

        if (runStringOnlyQuery(std::move(fctx).get().value()))
        {
            matchingLocations = stringOnlyResults;
        }

        clearErrorMessages();
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
    std::size_t scheduleId = queryRow.at(m_ScheduleItemIdIdx).as_uint64();
    std::size_t userID = queryRow.at(m_UserIdIdx).as_uint64();
    std::chrono::system_clock::time_point startTime = boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_StartTimeIdx).as_datetime());
    std::chrono::system_clock::time_point endTime = boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_EndTimeIdx).as_datetime());
    std::string title = queryRow.at(m_TitleIdx).as_string();
    std::string location;
    if (!queryRow.at(m_LocationIdx).is_null())
    {
        location = queryRow.at(m_LocationIdx).as_string();
    }
    std::chrono::system_clock::time_point creationTimeStamp = boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_CreatedOnIdx).as_datetime());
    std::chrono::system_clock::time_point lastUpdate = boostMysqlDateTimeToChronoTimePoint(queryRow.at(m_LastUpdateIdx).as_datetime());
    bool personal = queryRow.at(m_PersonalIdx).as_int64();

    return std::make_shared<ScheduleItemModel>(scheduleId, userID, title, startTime, endTime, location, personal, creationTimeStamp, lastUpdate);

}

void ScheduleItemQueryProcessor::fillRequiredIndexes()
{
    assignValueToIndex("idUserScheduleItem", m_ScheduleItemIdIdx);
    assignValueToIndex("UserID", m_UserIdIdx);
    assignValueToIndex("StartDateTime", m_StartTimeIdx);
    assignValueToIndex("EndDateTime", m_EndTimeIdx);
    assignValueToIndex("Title", m_TitleIdx);
    assignValueToIndex("Personal", m_PersonalIdx);
    assignValueToIndex("Location", m_LocationIdx);
    assignValueToIndex("CreatedTS", m_CreatedOnIdx);
    assignValueToIndex("LastUpdateTS", m_LastUpdateIdx);
    assignValueToIndex("Hidden", m_HiddenIdx);
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
         std::bind(&ScheduleItemQueryProcessor::findUserScheduleItemsByContentAndDateRange, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3), 
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
