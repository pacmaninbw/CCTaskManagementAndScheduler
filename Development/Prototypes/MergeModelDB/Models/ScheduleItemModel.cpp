// Project Header Files
#include "commonUtilities.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

ScheduleItemModel::ScheduleItemModel()
: ModelDBInterface("ScheduleItem", "idUserScheduleItem")
{
    userID = 0;
    title = "";
    personal = false;
}

ScheduleItemModel::ScheduleItemModel(
    std::size_t newID,
    std::size_t userId,
    std::string titleIn,
    std::chrono::system_clock::time_point startTimeTS,
    std::chrono::system_clock::time_point endTimeTS,
    std::string locationStr,
    bool personalIn,
    std::chrono::system_clock::time_point creationDateTS,
    std::chrono::system_clock::time_point lastUpdateTS
)
: ScheduleItemModel()
{
    primaryKey = newID;
    userID = userId;
    title = titleIn;
    startTime = startTimeTS;
    endTime = endTimeTS;
    personal = personalIn;
    location = locationStr;
    creationTimeStamp = creationDateTS;
    lastUpdate = lastUpdateTS;
    deleted = false;
}


void ScheduleItemModel::setUserID(std::size_t userId)
{
    modified = true;
    userID = userId;
}

void ScheduleItemModel::setTitle(std::string titleIn)
{
    modified = true;
    title = titleIn;
}

void ScheduleItemModel::setStartDateAndTime(std::chrono::system_clock::time_point startTimeTS)
{
    modified = true;
    startTime = startTimeTS;
}

void ScheduleItemModel::setEndDateAndTime(std::chrono::system_clock::time_point endTimeTS)
{
    modified = true;
    endTime = endTimeTS;
}

void ScheduleItemModel::setCreationDate(std::chrono::system_clock::time_point creationDateTS)
{
    modified = true;
    creationTimeStamp = creationDateTS;
}

void ScheduleItemModel::setLastUpdate(std::chrono::system_clock::time_point lastUpdateTS)
{
    modified = true;
    lastUpdate = lastUpdateTS;
}

void ScheduleItemModel::setPersonal(bool personalIn)
{
    modified = true;
    personal = personalIn;
}

void ScheduleItemModel::setLocation(std::string locationStr)
{
    modified = true;
    location = locationStr;
}

void ScheduleItemModel::setScheduleItemID(std::size_t newID)
{
    modified = true;
    primaryKey = newID;
}

bool ScheduleItemModel::diffScheduleItem(ScheduleItemModel &other)
{
    std::chrono::system_clock::time_point localStartTime = startTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point otherStartTime = other.startTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point localEndTime = endTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point otherEndTime = other.endTime.value_or(std::chrono::system_clock::now());

    return (primaryKey == other.primaryKey &&
        userID == other.userID &&
        title == other.title &&
        localStartTime == otherStartTime &&
        localEndTime == otherEndTime &&
        personal == other.personal
    );
}

std::string ScheduleItemModel::formatInsertStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),
        "CALL AddScheduleEvent({0}, {1}, {2}, {3}, {4}, {5})",
            userID,
            optionalDateTimeConversion(startTime.value()),
            optionalDateTimeConversion(endTime.value()),
            title,
            static_cast<unsigned int>(personal?1:0),
            location
    );
}

std::string ScheduleItemModel::formatUpdateStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),
        "CALL UpdateScheduleItemAllFields({0}, {1}, {2}, {3}, {4}, {5}, {6})",
            userID, primaryKey,
            optionalDateTimeConversion(startTime),
            optionalDateTimeConversion(endTime),
            title,
            static_cast<unsigned int>(personal?1:0),
            location
    );
}

std::string ScheduleItemModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(), "CALL HideScheduleItem({}, {})", userID, primaryKey);
}

std::string ScheduleItemModel::formatSelectStatement()
{
    errorMessages.clear();

    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, baseQuery);
    boost::mysql::format_sql_to(fctx, " WHERE idUserScheduleItem = {} AND UserID = {}", primaryKey, userID);
    boost::mysql::format_sql_to(fctx, " AND (Hidden IS NULL OR Hidden <> 1)");

    return std::move(fctx).get().value();
}

void ScheduleItemModel::initRequiredFields()
{
    missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingUserID, this), "User ID"});
    missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingTitle, this), "Title"});
    missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingStartTime, this), "Start Time"});
    missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingEndTime, this), "End Time"});
}

void ScheduleItemModel::processResultRow(boost::mysql::row_view rv)
{
    primaryKey = rv.at(scheduleItemIdIdx).as_uint64();
    userID = rv.at(userIdIdx).as_uint64();
    startTime = boostMysqlDateTimeToChronoTimePoint(rv.at(startTimeIdx).as_datetime());
    endTime = boostMysqlDateTimeToChronoTimePoint(rv.at(endTimeIdx).as_datetime());
    title = rv.at(titleIdx).as_string();
    if (!rv.at(locationIdx).is_null())
    {
        location = rv.at(locationIdx).as_string();
    }
    creationTimeStamp = boostMysqlDateTimeToChronoTimePoint(rv.at(createdOnIdx).as_datetime());
    lastUpdate = boostMysqlDateTimeToChronoTimePoint(rv.at(lastUpdate_Idx).as_datetime());
    personal = rv.at(personalIdx).as_int64();
}

