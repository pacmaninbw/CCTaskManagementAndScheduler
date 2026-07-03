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

    std::size_t getScheduleItemID() const { return m_PrimaryKey; };
    std::size_t getUserID() const { return m_UserID; };
    std::string getTitle() const { return m_Title; };
    std::chrono::system_clock::time_point getCreationDate() const { return m_Creation.value(); };
    std::chrono::system_clock::time_point getLastUpdate() const { return m_LastUpdate.value(); };
    std::chrono::system_clock::time_point getStartTime() const { return m_StartTime.value(); };
    std::chrono::system_clock::time_point getEndTime() const { return m_EndTime.value(); };
    std::string getLocation() const { return m_Location.value_or(""); };
    bool isPersonal() const { return m_Personal; };
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
    bool isMissingTitle() const noexcept { return (m_Title.empty() || m_Title.length() < MinimumTitleLength); };
    bool isMissingUserID() const noexcept { return m_UserID == 0; };
    bool isMissingCreationDate() const noexcept { return !m_Creation.has_value(); };
    bool isMissingStartTime() const noexcept { return !m_StartTime.has_value(); };
    bool isMissingEndTime() const noexcept { return !m_EndTime.has_value(); };

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
        os << std::format(outFmtStr, "ScheduleItem ID", scheduleItem.m_PrimaryKey);
        os << std::format(outFmtStr, "User ID", scheduleItem.m_UserID);
        os << std::format(outFmtStr, "Title", scheduleItem.m_Title);
        os << std::format(outFmtStr, "Start Date and Time", scheduleItem.m_StartTime.value_or(std::chrono::system_clock::now()));
        os << std::format(outFmtStr, "End Date and Time", scheduleItem.m_EndTime.value_or(std::chrono::system_clock::now()));
        os << std::format(outFmtStr, "Personal", (scheduleItem.m_Personal? "TRUE": "FALSE"));
        os << std::format(outFmtStr, "location", scheduleItem.getLocation());

        os << "Optional Fields\n";
        if (programOptions.showTimeStamps)
        {
            os << std::format(outFmtStr, "Creation Date", scheduleItem.m_Creation.value_or(std::chrono::system_clock::now()));
            if (scheduleItem.m_LastUpdate.has_value())
            {
                os << std::format(outFmtStr, "Last Update", scheduleItem.m_LastUpdate.value());
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

    std::size_t m_UserID;
    std::string m_Title;
/*
 * startTime and endTime are not optional in the database, We are using
 * std::optional for those 2 fields to remove errors in valgrind and possible
 * exceptions in some compilers.
 */
    std::optional<std::chrono::system_clock::time_point> m_StartTime;
    std::optional<std::chrono::system_clock::time_point> m_EndTime;
    std::optional<std::chrono::system_clock::time_point> m_Creation;
    std::optional<std::chrono::system_clock::time_point> m_LastUpdate;
    bool m_Personal;
    std::optional<std::string> m_Location;

    static const std::size_t MinimumTitleLength = 3;
};

using ScheduleItemModel_shp = std::shared_ptr<ScheduleItemModel>;

#endif // SCHEDULEITEMMODEL_H_


