// Project Header Files
#include "commonUtilities.h"
#include "GenericDictionary.h"
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

static const ScheduleItemModel::ScheduleItemType UnknowScheduleItem = static_cast<ScheduleItemModel::ScheduleItemType>(-1);

static std::vector<GenericDictionary<ScheduleItemModel::ScheduleItemType, std::string>::DictType> schedlueTypeConversionsDefs = {
    {ScheduleItemModel::ScheduleItemType::Meeting, "Meeting"},
    {ScheduleItemModel::ScheduleItemType::Phone_Call, "Phone Call"},
    {ScheduleItemModel::ScheduleItemType::Task_Execution, "Task Execution"},
    {ScheduleItemModel::ScheduleItemType::Personal_Appointment, "Personal Appointment"},
    {ScheduleItemModel::ScheduleItemType::Personal_Other, "Personal Other"}
};

static GenericDictionary<ScheduleItemModel::ScheduleItemType, std::string> scheduleItemTypeConversionTable(schedlueTypeConversionsDefs);

ScheduleItemModel::ScheduleItemModel()
: ModelDBInterface("ScheduleItem")
{
    userID = 0;
    title = "";
    personal = false;
}

ScheduleItemModel::ScheduleItemModel(std::size_t userID)
: ScheduleItemModel()
{
    setUserID(userID);
}

std::string ScheduleItemModel::getTypeStringVal() const
{
    ScheduleItemModel::ScheduleItemType status = getType();
    auto itemTypeName = scheduleItemTypeConversionTable.lookupName(status);
    return itemTypeName.has_value()? *itemTypeName : "Unknown Schedule Item Type Value";
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

void ScheduleItemModel::setType(ScheduleItemModel::ScheduleItemType sItemType)
{
    modified = true;
    scheduleItemType = sItemType;
    personal = (sItemType == ScheduleItemModel::ScheduleItemType::Personal_Appointment ||
        sItemType == ScheduleItemModel::ScheduleItemType::Personal_Other) ? true : false;
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

std::string ScheduleItemModel::scheduleItemTypeString(ScheduleItemModel::ScheduleItemType inVal) const
{
    auto scheduleItemName = scheduleItemTypeConversionTable.lookupName(inVal);
    return scheduleItemName.has_value()? *scheduleItemName : "Unknown ScheduleItemType Name";
}

ScheduleItemModel::ScheduleItemType ScheduleItemModel::stringToType(std::string sItemTypeName) const
{
    auto scheduleItem = scheduleItemTypeConversionTable.lookupID(sItemTypeName);
    return scheduleItem.has_value()? *scheduleItem : UnknowScheduleItem;
}

std::string ScheduleItemModel::formatSelectScheduleItemsByDateAndUser(std::chrono::year_month_day scheduleDate, std::size_t userId) noexcept
{
    errorMessages.clear();

    std::chrono::sys_days searchAdjustor(scheduleDate);
    searchAdjustor -= std::chrono::days{1};
    std::chrono::year_month_day startSearch(searchAdjustor);

    searchAdjustor = scheduleDate;
    searchAdjustor += std::chrono::days{1};
    std::chrono::year_month_day endSearch(searchAdjustor);

    try {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, listQueryBase);
        boost::mysql::format_sql_to(fctx, " WHERE UserID = {}", userId);
        boost::mysql::format_sql_to(fctx, " AND StartDateTime >= {}", stdchronoDateToBoostMySQLDate(startSearch));
        boost::mysql::format_sql_to(fctx, " AND StartDateTime <= {}", stdchronoDateToBoostMySQLDate(endSearch));

        return std::move(fctx).get().value();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In ScheduleItemModel::formatSelectScheduleItemsByDateAndUser({}) : {}", userId, e.what()));
    }

    return std::string();
}

std::string ScheduleItemModel::formatSelectSiByContentDateRangeUser(
    std::string content,
    std::chrono::year_month_day searchStart,
    std::chrono::year_month_day searchEnd,
    std::size_t userId
) noexcept
{
    errorMessages.clear();

    try {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, listQueryBase);
        boost::mysql::format_sql_to(fctx, " WHERE UserID = {}", userId);
        boost::mysql::format_sql_to(fctx, " AND Title LIKE {}", content);
        boost::mysql::format_sql_to(fctx, " AND StartDateTime >= {}", stdchronoDateToBoostMySQLDate(searchStart));
        boost::mysql::format_sql_to(fctx, " AND StartDateTime <= {}", stdchronoDateToBoostMySQLDate(searchEnd));

        return std::move(fctx).get().value();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In ScheduleItemModel::formatSelectSiByContentDateRangeUser({}) : {}", userId, e.what()));
    }

    return std::string();
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
    if (isMissingCreationDate())
    {
        creationTimeStamp = std::chrono::system_clock::now();
    }

    lastUpdate = std::chrono::system_clock::now();

    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),
        "INSERT INTO ScheduleItems (UserID, StartDateTime, EndDateTime, ItemType, Title, Location, CreatedTS, LastUpdateTS) "
            " VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7})",
            userID,
            optionalDateTimeConversion(startTime.value()),
            optionalDateTimeConversion(endTime.value()),
            static_cast<unsigned int>(scheduleItemType),
            title,
            location,
            optionalDateTimeConversion(creationTimeStamp),
            optionalDateTimeConversion(lastUpdate)
    );
}

std::string ScheduleItemModel::formatUpdateStatement()
{
    initFormatOptions();

    lastUpdate = std::chrono::system_clock::now();

    return boost::mysql::format_sql(format_opts.value(),
        "UPDATE ScheduleItems SET"
            " StartDateTime = {0},"
            " EndDateTime = {1},"
            " ItemType = {2},"
            " Title = {3},"
            " Location = {4},"
            " LastUpdateTS = {5}"
        " WHERE idUserScheduleItem = {6} AND UserID = {7} ",
            optionalDateTimeConversion(startTime),
            optionalDateTimeConversion(endTime),
            static_cast<unsigned int>(scheduleItemType),
            title,
            location,
            optionalDateTimeConversion(lastUpdate),
        primaryKey, userID
    );
}

std::string ScheduleItemModel::formatSelectStatement()
{
    errorMessages.clear();

    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, baseQuery);
    boost::mysql::format_sql_to(fctx, " WHERE idUserScheduleItem = {} AND UserID = {}", primaryKey, userID);

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
    scheduleItemType = static_cast<ScheduleItemModel::ScheduleItemType>(rv.at(itemTypeIdx).as_uint64());
    title = rv.at(titleIdx).as_string();
    if (!rv.at(locationIdx).is_null())
    {
        location = rv.at(locationIdx).as_string();
    }
    creationTimeStamp = boostMysqlDateTimeToChronoTimePoint(rv.at(createdOnIdx).as_datetime());
    lastUpdate = boostMysqlDateTimeToChronoTimePoint(rv.at(lastUpdate_Idx).as_datetime());

    personal = (scheduleItemType == ScheduleItemModel::ScheduleItemType::Personal_Appointment ||
        scheduleItemType == ScheduleItemModel::ScheduleItemType::Personal_Other) ? true : false;
}

