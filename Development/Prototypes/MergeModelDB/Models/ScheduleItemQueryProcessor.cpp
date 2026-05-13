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
    requiredColumns =  {"idUserScheduleItem", "UserID", "StartDateTime", "EndDateTime", "Title", "Personal", "Location", "CreatedTS", "LastUpdateTS", "Hidden"};
    for (auto columnName: requiredColumns)
    {
        columnToIndexMap.push_back(columnName);
    }
}

ScheduleItemList ScheduleItemQueryProcessor::getUserDaySchedule(std::chrono::year_month_day scheduleDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In ScheduleItemQueryProcessor::getUserDaySchedule : ");

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatGetUserDaySchedule(scheduleDate, userID));
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
        boost::mysql::results localResult = runQueryAsync(formatSelectSiByContentDateRangeUser(searchTitle, searchStart, searchEnd, userID));
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
        boost::mysql::results localResult = runQueryAsync(formatGetUniqueContentsByUserSortByContent(searchTitle, userID));
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
        firstFormattedQuery = formatGetAllUniqueContentsByUserSortByContent(userID);
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
        firstFormattedQuery = formatGetAllUniqueLocationsByUserSortByContent(userID);
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

std::string ScheduleItemQueryProcessor::formatGetUserDaySchedule(std::chrono::year_month_day scheduleDate, std::size_t userId)
{
    errorMessages.clear();

    const int secondsInHour = 3600;
    const int secondsInDay = secondsInHour * 24;
    std::chrono::seconds dateAdjustor(secondsInDay - 1);

    std::chrono::system_clock::time_point startSearch = getLocalMidnight(scheduleDate);
    std::chrono::system_clock::time_point endSearch = startSearch + dateAdjustor;

    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "SELECT * FROM UserScheduleItem ");
    boost::mysql::format_sql_to(fctx, " WHERE UserID = {}", userId);
    boost::mysql::format_sql_to(fctx, " AND (Hidden IS NULL OR Hidden <> 1)");
    boost::mysql::format_sql_to(fctx, " AND StartDateTime >= {}", stdChronoTimePointToBoostDateTime(startSearch));
    boost::mysql::format_sql_to(fctx, " AND StartDateTime <= {}", stdChronoTimePointToBoostDateTime(endSearch));
    boost::mysql::format_sql_to(fctx, " ORDER BY StartDateTime ASC");

    return std::move(fctx).get().value();
}

std::string ScheduleItemQueryProcessor::formatSelectSiByContentDateRangeUser(
    std::string content,
    std::chrono::year_month_day searchStart,
    std::chrono::year_month_day searchEnd, std::size_t userId
)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "SELECT * FROM UserScheduleItem ");
    boost::mysql::format_sql_to(fctx, " WHERE UserID = {}", userId);
    boost::mysql::format_sql_to(fctx, " AND Title LIKE {}", wrapSearchContentSQLPatternMatch(content));
    boost::mysql::format_sql_to(fctx, " AND (Hidden IS NULL OR Hidden <> 1)");
    boost::mysql::format_sql_to(fctx, " AND StartDateTime >= {}", stdchronoDateToBoostMySQLDate(searchStart));
    boost::mysql::format_sql_to(fctx, " AND StartDateTime <= {}", stdchronoDateToBoostMySQLDate(searchEnd));

    return std::move(fctx).get().value();
}

std::string ScheduleItemQueryProcessor::formatSelectSiByContentAndUserSortByContent(std::string content, std::size_t userId)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "SELECT * FROM UserScheduleItem ");
    boost::mysql::format_sql_to(fctx, " WHERE UserID = {}", userId);
    boost::mysql::format_sql_to(fctx, " AND Title LIKE {}", wrapSearchContentSQLPatternMatch(content));
    boost::mysql::format_sql_to(fctx, " AND (Hidden IS NULL OR Hidden <> 1)");
    boost::mysql::format_sql_to(fctx, " ORDER BY Title");

    return std::move(fctx).get().value();
}

std::string ScheduleItemQueryProcessor::formatGetUniqueContentsByUserSortByContent(std::string content, std::size_t userId)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "CALL ScheduleItemContentSelectionList({}, {})", content, userId);

    return std::move(fctx).get().value();
}

std::string ScheduleItemQueryProcessor::formatGetAllUniqueContentsByUserSortByContent(std::size_t userId)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "CALL EventTitlesForCompleter({})", userId);

    return std::move(fctx).get().value();
}

std::string ScheduleItemQueryProcessor::formatGetAllUniqueLocationsByUserSortByContent(std::size_t userId)
{
    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, "CALL EventLocationsForCompleter({})", userId);

    return std::move(fctx).get().value();
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

