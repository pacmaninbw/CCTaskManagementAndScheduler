#ifndef SCHEDULEITEMMODEL_H_
#define SCHEDULEITEMMODEL_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class ScheduleItemModel : public ModelDBInterface
{
public:
    ScheduleItemModel();
    ScheduleItemModel(
        std::size_t newID,
        std::size_t userId,
        std::string titleIn,
        std::chrono::system_clock::time_point startTimeTS,
        std::chrono::system_clock::time_point endTimeTS,
        std::string locationStr,
        bool personalIn,
        std::chrono::system_clock::time_point creationDateTS,
        std::chrono::system_clock::time_point lastUpdateTS
    );
    virtual ~ScheduleItemModel() = default;

    std::size_t getScheduleItemID() const { return primaryKey; };
    std::size_t getUserID() const { return userID; };
    std::string getTitle() const { return title; };
    std::chrono::system_clock::time_point getCreationDate() const { return creationTimeStamp.value(); };
    std::chrono::system_clock::time_point getLastUpdate() const { return lastUpdate.value(); };
    std::chrono::system_clock::time_point getStartTime() const { return startTime.value(); };
    std::chrono::system_clock::time_point getEndTime() const { return endTime.value(); };
    std::string getLocation() const { return location.value_or(""); };
    bool isPersonal() const { return personal; };
    void setUserID(std::size_t userId);
    void setTitle(std::string titleIn);
    void setStartDateAndTime(std::chrono::system_clock::time_point startTimeTS);
    void setEndDateAndTime(std::chrono::system_clock::time_point endTimeTS);
    void setCreationDate(std::chrono::system_clock::time_point creationDateTS);
    void setLastUpdate(std::chrono::system_clock::time_point lastUpdateTS);
    void setPersonal(bool personalIn);
    void setLocation(std::string locationStr);
    void setScheduleItemID(std::size_t newID);

/*
 * Required fields.
 */
    bool isMissingTitle() const noexcept { return (title.empty() || title.length() < MinimumTitleLength); };
    bool isMissingUserID() const noexcept { return userID == 0; };
    bool isMissingCreationDate() const noexcept { return !creationTimeStamp.has_value(); };
    bool isMissingStartTime() const noexcept { return !startTime.has_value(); };
    bool isMissingEndTime() const noexcept { return !endTime.has_value(); };

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
        os << std::format(outFmtStr, "ScheduleItem ID", scheduleItem.primaryKey);
        os << std::format(outFmtStr, "User ID", scheduleItem.userID);
        os << std::format(outFmtStr, "Title", scheduleItem.title);
        os << std::format(outFmtStr, "Start Date and Time", scheduleItem.startTime.value_or(std::chrono::system_clock::now()));
        os << std::format(outFmtStr, "End Date and Time", scheduleItem.endTime.value_or(std::chrono::system_clock::now()));
        os << std::format(outFmtStr, "Personal", (scheduleItem.personal? "TRUE": "FALSE"));
        os << std::format(outFmtStr, "location", scheduleItem.getLocation());

        os << "Optional Fields\n";
        if (programOptions.showTimeStamps)
        {
            os << std::format(outFmtStr, "Creation Date", scheduleItem.creationTimeStamp.value_or(std::chrono::system_clock::now()));
            if (scheduleItem.lastUpdate.has_value())
            {
                os << std::format(outFmtStr, "Last Update", scheduleItem.lastUpdate.value());
            }
        }

        return os;
    };


protected:
/*
 * Implementation
 */
    bool diffScheduleItem(ScheduleItemModel& other);
    virtual std::string formatInsertStatement() override;
    virtual std::string formatUpdateStatement() override;
    virtual std::string formatDeleteStatement() override;
    void initRequiredFields() override;

 /*
  * Member Variables
  */
    std::size_t userID;
    std::string title;
/*
 * startTime and endTime are not optional in the database, We are using
 * std::optional for those 2 fields to remove errors in valgrind and possible
 * exceptions in some compilers.
 */
    std::optional<std::chrono::system_clock::time_point> startTime;
    std::optional<std::chrono::system_clock::time_point> endTime;
    std::optional<std::chrono::system_clock::time_point> creationTimeStamp;
    std::optional<std::chrono::system_clock::time_point> lastUpdate;
    bool personal;
    static const std::size_t MinimumTitleLength = 3;
    std::optional<std::string> location;
};

using ScheduleItemModel_shp = std::shared_ptr<ScheduleItemModel>;

#endif // SCHEDULEITEMMODEL_H_


