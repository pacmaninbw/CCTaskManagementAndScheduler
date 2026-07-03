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
    m_UserID = 0;
    m_Title = "";
    m_Personal = false;
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
    m_PrimaryKey = eventId;
    m_UserID = userId;
    m_Title = title;
    m_StartTime = startTime;
    m_EndTime = endTime;
    m_Personal = personal;
    m_Location = location;
    m_Creation = creationDate;
    m_LastUpdate = lastUpdate;
    m_Deleted = false;
}


void ScheduleItemModel::setUserID(std::size_t userId)
{
    m_Modified = true;
    m_UserID = userId;
}

void ScheduleItemModel::setTitle(std::string title)
{
    m_Modified = true;
    m_Title = title;
}

void ScheduleItemModel::setStartDateAndTime(std::chrono::system_clock::time_point startTime)
{
    m_Modified = true;
    m_StartTime = startTime;
}

void ScheduleItemModel::setEndDateAndTime(std::chrono::system_clock::time_point endTime)
{
    m_Modified = true;
    m_EndTime = endTime;
}

void ScheduleItemModel::setCreationDate(std::chrono::system_clock::time_point creationDate)
{
    m_Modified = true;
    m_Creation = creationDate;
}

void ScheduleItemModel::setLastUpdate(std::chrono::system_clock::time_point lastUpdate)
{
    m_Modified = true;
    m_LastUpdate = lastUpdate;
}

void ScheduleItemModel::setPersonal(bool personal)
{
    m_Modified = true;
    m_Personal = personal;
}

void ScheduleItemModel::setLocation(std::string location)
{
    m_Modified = true;
    m_Location = location;
}

void ScheduleItemModel::setScheduleItemID(std::size_t eventId)
{
    m_Modified = true;
    m_PrimaryKey = eventId;
}

bool ScheduleItemModel::diffScheduleItem(ScheduleItemModel &other)
{
    std::chrono::system_clock::time_point localStartTime = m_StartTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point otherStartTime = other.m_StartTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point localEndTime = m_EndTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point otherEndTime = other.m_EndTime.value_or(std::chrono::system_clock::now());

    return (m_PrimaryKey == other.m_PrimaryKey &&
        m_UserID == other.m_UserID &&
        m_Title == other.m_Title &&
        localStartTime == otherStartTime &&
        localEndTime == otherEndTime &&
        m_Personal == other.m_Personal
    );
}

std::string ScheduleItemModel::formatInsertStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),
        "CALL AddScheduleEvent({0}, {1}, {2}, {3}, {4}, {5})",
            m_UserID,
            optionalDateTimeConversion(m_StartTime.value()),
            optionalDateTimeConversion(m_EndTime.value()),
            m_Title,
            static_cast<unsigned int>(m_Personal?1:0),
            m_Location
    );
}

std::string ScheduleItemModel::formatUpdateStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),
        "CALL UpdateScheduleItemAllFields({0}, {1}, {2}, {3}, {4}, {5}, {6})",
            m_UserID, m_PrimaryKey,
            optionalDateTimeConversion(m_StartTime),
            optionalDateTimeConversion(m_EndTime),
            m_Title,
            static_cast<unsigned int>(m_Personal?1:0),
            m_Location
    );
}

std::string ScheduleItemModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(), "CALL HideScheduleItem({}, {})", m_UserID, m_PrimaryKey);
}

void ScheduleItemModel::initRequiredFields()
{
    m_MissingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingUserID, this), "User ID"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingTitle, this), "Title"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingStartTime, this), "Start Time"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingEndTime, this), "End Time"});
}

