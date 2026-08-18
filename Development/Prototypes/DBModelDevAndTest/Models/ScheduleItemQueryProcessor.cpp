// Project Header Files
#include "commonUtilities.h"
#include "QueryProcessor.h"
#include "ScheduleItemQueryProcessor.h"
#include "ScheduleItemModel.h"
#include "chronoBoostConversions.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

ScheduleItemQueryProcessor::ScheduleItemQueryProcessor(std::size_t userId)
: QueryProcessor<ScheduleItemModel, ScheduleItemDbQueryValues>("ScheduleItem")
{
    m_userID = userId;
}

ScheduleItemModel_shp ScheduleItemQueryProcessor::getScheduleItemById(std::size_t eventId) noexcept
{
    clearErrorMessages();
    ScheduleItemModel_shp found = nullptr;

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_schedule_item ");
        boost::mysql::format_sql_to(fctx, "WHERE user_schedule_item.id_user_schedule_item = {} ", eventId);
        boost::mysql::format_sql_to(fctx, "AND user_schedule_item.deleted <> 1");
        
        StaticQueryEvent localResult = staticRunQueryAsync<ScheduleItemDbQueryValues>(std::move(fctx).get().value());
        found = getOneStaticResult(localResult);
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

        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_schedule_item ");
        boost::mysql::format_sql_to(fctx, "WHERE user_schedule_item.user_id = {} ", m_userID);
        boost::mysql::format_sql_to(fctx, "AND user_schedule_item.deleted <> 1 ");
        boost::mysql::format_sql_to(fctx, "AND user_schedule_item.start_date_time >= {} ",
            common::toBoostDateTime(startSearch));
        boost::mysql::format_sql_to(fctx, "AND user_schedule_item.start_date_time <= {} ",
            common::toBoostDateTime(endSearch));
        boost::mysql::format_sql_to(fctx, "ORDER BY user_schedule_item.start_date_time ASC");

        StaticQueryEvent localResult = staticRunQueryAsync<ScheduleItemDbQueryValues>(std::move(fctx).get().value());
        return processStaticResults(localResult);
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
        std::string patternMatcher = wrapSearchContentSQLPatternMatch(searchTitle);
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM user_schedule_item ");
        boost::mysql::format_sql_to(fctx, "WHERE user_id = {} ", m_userID);
        boost::mysql::format_sql_to(fctx, "AND title LIKE {} ", patternMatcher);
        boost::mysql::format_sql_to(fctx, "AND deleted <> 1 ");
        boost::mysql::format_sql_to(fctx, "AND start_date_time >= {} ", common::toBoostMySQLDate(searchStart));
        boost::mysql::format_sql_to(fctx, "AND start_date_time <= {}", common::toBoostMySQLDate(searchEnd));

        StaticQueryEvent localResult = staticRunQueryAsync<ScheduleItemDbQueryValues>(std::move(fctx).get().value());
        return processStaticResults(localResult);
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
        std::string contentPattern = wrapSearchContentSQLPatternMatch(searchTitle);

        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT DISTINCT user_schedule_item.title FROM user_schedule_item ");
        boost::mysql::format_sql_to(fctx, "WHERE user_schedule_item.user_id = {} ", m_userID);
        boost::mysql::format_sql_to(fctx, "AND user_schedule_item.title LIKE {} ", contentPattern);

        boost::mysql::format_sql_to(fctx, "ORDER BY user_schedule_item.title ASC");
        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

        if (!m_selfTest)
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
            if (!m_forceException)
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
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT DISTINCT user_schedule_item.title FROM user_schedule_item ");
        boost::mysql::format_sql_to(fctx, "WHERE user_schedule_item.user_id = {} ", m_userID);
        boost::mysql::format_sql_to(fctx, "ORDER BY user_schedule_item.title ASC");

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
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT DISTINCT user_schedule_item.location FROM user_schedule_item ");
        boost::mysql::format_sql_to(fctx, "WHERE user_schedule_item.user_id = {} ", m_userID);
        boost::mysql::format_sql_to(fctx, "ORDER BY user_schedule_item.location ASC");

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
