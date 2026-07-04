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

std::string ScheduleItemModel::formatInsertStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(m_formatOpts.value(),
        "CALL AddScheduleEvent({0}, {1}, {2}, {3}, {4}, {5})",
            m_userID,
            optionalDateTimeConversion(m_startTime.value()),
            optionalDateTimeConversion(m_endTime.value()),
            m_title,
            static_cast<unsigned int>(m_personal?1:0),
            m_location
    );
}

std::string ScheduleItemModel::formatUpdateStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(m_formatOpts.value(),
        "CALL UpdateScheduleItemAllFields({0}, {1}, {2}, {3}, {4}, {5}, {6})",
            m_userID, m_primaryKey,
            optionalDateTimeConversion(m_startTime),
            optionalDateTimeConversion(m_endTime),
            m_title,
            static_cast<unsigned int>(m_personal?1:0),
            m_location
    );
}

std::string ScheduleItemModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(m_formatOpts.value(), "CALL HideScheduleItem({}, {})", m_userID, m_primaryKey);
}

void ScheduleItemModel::initRequiredFields()
{
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingUserID, this), "User ID"});
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingTitle, this), "Title"});
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingStartTime, this), "Start Time"});
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingEndTime, this), "End Time"});
}

