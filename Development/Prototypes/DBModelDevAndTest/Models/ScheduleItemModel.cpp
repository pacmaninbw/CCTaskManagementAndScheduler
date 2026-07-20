// Project Header Files
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <string>
#include <vector>

ScheduleItemModel::ScheduleItemModel()
: ModelDBInterface("ScheduleItem", "idUserScheduleItem")
{
    m_userID = 0;
    m_title = "";
    m_personal = false;
}

ScheduleItemModel::ScheduleItemModel(
    std::size_t eventId,
    std::size_t userId,
    std::string title,
    std::chrono::system_clock::time_point startTime,
    std::chrono::system_clock::time_point endTime,
    std::string location,
    bool personal,
    std::chrono::system_clock::time_point creationDate,
    std::chrono::system_clock::time_point lastUpdate
)
: ScheduleItemModel()
{
    m_primaryKey = eventId;
    m_userID = userId;
    m_title = title;
    m_startTime = startTime;
    m_endTime = endTime;
    m_personal = personal;
    m_location = location;
    m_creation = creationDate;
    m_lastUpdate = lastUpdate;
    m_deleted = false;
}


void ScheduleItemModel::setUserID(std::size_t userId)
{
    m_modified = true;
    m_userID = userId;
}

void ScheduleItemModel::setTitle(std::string title)
{
    m_modified = true;
    m_title = title;
}

void ScheduleItemModel::setStartDateAndTime(std::chrono::system_clock::time_point startTime)
{
    m_modified = true;
    m_startTime = startTime;
}

void ScheduleItemModel::setEndDateAndTime(std::chrono::system_clock::time_point endTime)
{
    m_modified = true;
    m_endTime = endTime;
}

void ScheduleItemModel::setCreationDate(std::chrono::system_clock::time_point creationDate)
{
    m_modified = true;
    m_creation = creationDate;
}

void ScheduleItemModel::setLastUpdate(std::chrono::system_clock::time_point lastUpdate)
{
    m_modified = true;
    m_lastUpdate = lastUpdate;
}

void ScheduleItemModel::setPersonal(bool personal)
{
    m_modified = true;
    m_personal = personal;
}

void ScheduleItemModel::setLocation(std::string location)
{
    m_modified = true;
    m_location = location;
}

void ScheduleItemModel::setScheduleItemID(std::size_t eventId)
{
    m_modified = true;
    m_primaryKey = eventId;
}

bool ScheduleItemModel::diffScheduleItem(ScheduleItemModel &other)
{
    std::chrono::system_clock::time_point localStartTime = m_startTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point otherStartTime = other.m_startTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point localEndTime = m_endTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point otherEndTime = other.m_endTime.value_or(std::chrono::system_clock::now());

    return (m_primaryKey == other.m_primaryKey &&
        m_userID == other.m_userID &&
        m_title == other.m_title &&
        localStartTime == otherStartTime &&
        localEndTime == otherEndTime &&
        m_personal == other.m_personal
    );
}

/*
 * To make the code more maintainable each field / column in a table will have
 * its own line in insert and update statements. 
 */
std::string ScheduleItemModel::formatInsertStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "INSERT INTO user_schedule_item  (");
    boost::mysql::format_sql_to(fctx, "user_id, ");
    boost::mysql::format_sql_to(fctx, "start_date_time, ");
    boost::mysql::format_sql_to(fctx, "end_date_time, ");
    boost::mysql::format_sql_to(fctx, "title, ");
    boost::mysql::format_sql_to(fctx, "personal, ");
    boost::mysql::format_sql_to(fctx, "location, ");
    boost::mysql::format_sql_to(fctx, "deleted");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_userID);
    boost::mysql::format_sql_to(fctx, "{}, ", optionalDateTimeConversion(m_startTime.value()));
    boost::mysql::format_sql_to(fctx, "{}, ", optionalDateTimeConversion(m_endTime.value()));
    boost::mysql::format_sql_to(fctx, "{}, ", m_title);
    boost::mysql::format_sql_to(fctx, "{}, ", static_cast<unsigned int>(m_personal?1:0));
    boost::mysql::format_sql_to(fctx, "{}, ", m_location);
    boost::mysql::format_sql_to(fctx, "{} ", m_deleted);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string ScheduleItemModel::formatUpdateStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_schedule_item SET ");
    boost::mysql::format_sql_to(fctx, "user_schedule_item.start_date_time = {}, ", optionalDateTimeConversion(m_startTime));
    boost::mysql::format_sql_to(fctx, "user_schedule_item.end_date_time = {}, ", optionalDateTimeConversion(m_endTime));
    boost::mysql::format_sql_to(fctx, "user_schedule_item.title = {}, ", m_title);
    boost::mysql::format_sql_to(fctx, "user_schedule_item.personal = {}, ", static_cast<unsigned int>(m_personal?1:0));
    boost::mysql::format_sql_to(fctx, "user_schedule_item.location = {} ", m_location);
    boost::mysql::format_sql_to(fctx, "WHERE user_schedule_item.id_user_schedule_item = {} ", m_primaryKey);
    boost::mysql::format_sql_to(fctx, "AND user_schedule_item.user_id = user_id", m_userID);

    return (std::move(fctx).get().value());
}

std::string ScheduleItemModel::formatDeleteStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_schedule_item SET user_schedule_item.deleted = 1 ");
    boost::mysql::format_sql_to(fctx, "WHERE user_schedule_item.user_id = {} ", m_userID);
    boost::mysql::format_sql_to(fctx, "AND user_schedule_item.id_user_schedule_item = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}

void ScheduleItemModel::initRequiredFields()
{
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingUserID, this), "User ID"});
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingTitle, this), "Title"});
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingStartTime, this), "Start Time"});
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingEndTime, this), "End Time"});
}

