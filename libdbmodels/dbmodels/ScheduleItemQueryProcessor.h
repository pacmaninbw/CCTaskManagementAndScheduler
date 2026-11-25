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
using StaticQueryEvent = boost::mysql::static_results<boost::mysql::pfr_by_name<ScheduleItemDbQueryValues>>;

class ScheduleItemQueryProcessor : public QueryProcessor<ScheduleItemModel, ScheduleItemDbQueryValues>
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
    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetUserDaySchedule() noexcept;
    TestStatus testExceptionFindUserScheduleItemsByContentAndDateRange() noexcept;
    TestStatus testExceptionFindEventSToRepeat() noexcept;
    TestStatus testExceptionFindEventsForRepeatCompletion() noexcept;
    TestStatus testExceptionFindLocationsForRepeatCompletion() noexcept;
    TestStatus testExceptionGetScheduleItemByID() noexcept;


    std::size_t m_userID;
};

#endif // SCHEDULEITEMLQUERYPROCESSOR_H_

