#ifndef SCHEDULEITEMLQUERYPROCESSOR_H_
#define SCHEDULEITEMLQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <string>
#include <vector>

using ScheduleItemList = std::vector<ScheduleItemModel_shp>;

class ScheduleItemQueryProcessor : public QueryProcessor<ScheduleItemModel>
{
public:
    ScheduleItemQueryProcessor(std::size_t userId);
    virtual ~ScheduleItemQueryProcessor() = default;

    ScheduleItemModel_shp getScheduleItemById(std::size_t eventId) noexcept;
    ScheduleItemList getUserDaySchedule(std::chrono::year_month_day scheduleDate) noexcept;
    ScheduleItemList findUserScheduleItemsByContentAndDateRange(std::string searchTitle,
        std::chrono::year_month_day searchStart, std::chrono::year_month_day searchEnd) noexcept;
    std::vector<std::string> findEventSToRepeat(std::string searchTitle) noexcept;
    std::vector<std::string> findEventsForRepeatCompletion() noexcept;
    std::vector<std::string> findLocationsForRepeatCompletion() noexcept; 

private:
    std::string formatGetUserDaySchedule(std::chrono::year_month_day scheduleDate, std::size_t userId);
    std::string formatSelectSiByContentDateRangeUser(std::string content, std::chrono::year_month_day searchStart, 
        std::chrono::year_month_day searchEnd, std::size_t userId);
    std::string formatSelectSiByContentAndUserSortByContent(std::string content, std::size_t userId);
    std::string formatGetUniqueContentsByUserSortByContent(std::string content, std::size_t userId);
    std::string formatGetAllUniqueContentsByUserSortByContent(std::size_t userId);
    std::string formatGetAllUniqueLocationsByUserSortByContent(std::size_t userId);
    virtual ScheduleItemModel_shp processResultRow(boost::mysql::row_view& queryRow);
    virtual void fillRequiredIndexes() override;

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetUserDaySchedule() noexcept;
    TestStatus testExceptionFindUserScheduleItemsByContentAndDateRange() noexcept;
    TestStatus testExceptionFindEventSToRepeat() noexcept;
    TestStatus testExceptionFindEventsForRepeatCompletion() noexcept;
    TestStatus testExceptionFindLocationsForRepeatCompletion() noexcept;


    std::size_t userID;
    std::size_t scheduleItemIdIdx = IndexNotSet;
    std::size_t userIdIdx = IndexNotSet;
    std::size_t startTimeIdx = IndexNotSet;
    std::size_t endTimeIdx = IndexNotSet;
    std::size_t titleIdx = IndexNotSet;
    std::size_t personalIdx = IndexNotSet;
    std::size_t locationIdx = IndexNotSet;
    std::size_t createdOnIdx = IndexNotSet;
    std::size_t lastUpdate_Idx = IndexNotSet;
    std::size_t hidden_Idx = IndexNotSet;
};

#endif // SCHEDULEITEMLQUERYPROCESSOR_H_

