#ifndef SCHEDULEITEMMODEL_H_
#define SCHEDULEITEMMODEL_H_

// Project Header Files
#include "CommandLineParser.h"
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

class ScheduleItemModel : public ModelDBInterface
{
public:
    ScheduleItemModel();
    ScheduleItemModel(
        std::size_t eventID,
        std::size_t userId,
        std::string title,
        std::chrono::system_clock::time_point startTime,
        std::chrono::system_clock::time_point endTime,
        std::string location,
        bool personal,
        std::chrono::system_clock::time_point creationDate,
        std::chrono::system_clock::time_point lastUpdate
    );
    virtual ~ScheduleItemModel() = default;

    std::size_t getScheduleItemID() const { return m_primaryKey; };
    std::size_t getUserID() const { return m_userID; };
    std::string getTitle() const { return m_title; };
    std::chrono::system_clock::time_point getCreationDate() const { return m_creation.value(); };
    std::chrono::system_clock::time_point getLastUpdate() const { return m_lastUpdate.value(); };
    std::chrono::system_clock::time_point getStartTime() const { return m_startTime.value(); };
    std::chrono::system_clock::time_point getEndTime() const { return m_endTime.value(); };
    std::string getLocation() const { return m_location.value_or(""); };
    bool isPersonal() const { return m_personal; };
    void setUserID(std::size_t userId);
    void setTitle(std::string title);
    void setStartDateAndTime(std::chrono::system_clock::time_point startTime);
    void setEndDateAndTime(std::chrono::system_clock::time_point endTime);
    void setCreationDate(std::chrono::system_clock::time_point creationDate);
    void setLastUpdate(std::chrono::system_clock::time_point lastUpdate);
    void setPersonal(bool personal);
    void setLocation(std::string location);
    void setScheduleItemID(std::size_t eventID);

/*
 * Required fields.
 */
    bool isMissingTitle() const noexcept { return (m_title.empty() || m_title.length() < MinimumTitleLength); };
    bool isMissingUserID() const noexcept { return m_userID == 0; };
    bool isMissingCreationDate() const noexcept { return !m_creation.has_value(); };
    bool isMissingStartTime() const noexcept { return !m_startTime.has_value(); };
    bool isMissingEndTime() const noexcept { return !m_endTime.has_value(); };

/*
 * Operators
 */
    bool operator==(ScheduleItemModel& other)
    {
        return diffScheduleItem(other);
    }
    bool operator==(std::shared_ptr<ScheduleItemModel> other)
    {
        return diffScheduleItem(*other);
    }

    friend std::ostream& operator<<(std::ostream& os, const ScheduleItemModel& scheduleItem)
    {
        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << "ScheduleItemModel:\n";
        os << std::format(outFmtStr, "ScheduleItem ID", scheduleItem.m_primaryKey);
        os << std::format(outFmtStr, "User ID", scheduleItem.m_userID);
        os << std::format(outFmtStr, "Title", scheduleItem.m_title);
        os << std::format(outFmtStr, "Start Date and Time", scheduleItem.m_startTime.value_or(std::chrono::system_clock::now()));
        os << std::format(outFmtStr, "End Date and Time", scheduleItem.m_endTime.value_or(std::chrono::system_clock::now()));
        os << std::format(outFmtStr, "Personal", (scheduleItem.m_personal? "TRUE": "FALSE"));
        os << std::format(outFmtStr, "location", scheduleItem.getLocation());

        os << "Optional Fields\n";
        if (programOptions.showTimeStamps)
        {
            os << std::format(outFmtStr, "Creation Date", scheduleItem.m_creation.value_or(std::chrono::system_clock::now()));
            if (scheduleItem.m_lastUpdate.has_value())
            {
                os << std::format(outFmtStr, "Last Update", scheduleItem.m_lastUpdate.value());
            }
        }

        return os;
    };


protected:
    bool diffScheduleItem(ScheduleItemModel& other);
    virtual std::string formatInsertStatement() override;
    virtual std::string formatUpdateStatement() override;
    virtual std::string formatDeleteStatement() override;
    void initRequiredFields() override;

    std::size_t m_userID;
    std::string m_title;
/*
 * startTime and endTime are not optional in the database, We are using
 * std::optional for those 2 fields to remove errors in valgrind and possible
 * exceptions in some compilers.
 */
    std::optional<std::chrono::system_clock::time_point> m_startTime;
    std::optional<std::chrono::system_clock::time_point> m_endTime;
    std::optional<std::chrono::system_clock::time_point> m_creation;
    std::optional<std::chrono::system_clock::time_point> m_lastUpdate;
    bool m_personal;
    std::optional<std::string> m_location;

    static const std::size_t MinimumTitleLength = 3;
};

using ScheduleItemModel_shp = std::shared_ptr<ScheduleItemModel>;

#endif // SCHEDULEITEMMODEL_H_


