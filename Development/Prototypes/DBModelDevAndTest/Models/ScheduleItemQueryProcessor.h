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
    virtual ScheduleItemModel_shp processResultRow(boost::mysql::row_view& queryRow);
    virtual void fillRequiredIndexes() override;

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetUserDaySchedule() noexcept;
    TestStatus testExceptionFindUserScheduleItemsByContentAndDateRange() noexcept;
    TestStatus testExceptionFindEventSToRepeat() noexcept;
    TestStatus testExceptionFindEventsForRepeatCompletion() noexcept;
    TestStatus testExceptionFindLocationsForRepeatCompletion() noexcept;
    TestStatus testExceptionGetScheduleItemByID() noexcept;


    std::size_t m_userID;
    std::size_t m_scheduleItemIdIdx = IndexNotSet;
    std::size_t m_userIdIdx = IndexNotSet;
    std::size_t m_startTimeIdx = IndexNotSet;
    std::size_t m_endTimeIdx = IndexNotSet;
    std::size_t m_titleIdx = IndexNotSet;
    std::size_t m_personalIdx = IndexNotSet;
    std::size_t m_locationIdx = IndexNotSet;
    std::size_t m_createdOnIdx = IndexNotSet;
    std::size_t m_lastUpdateIdx = IndexNotSet;
    std::size_t m_hiddenIdx = IndexNotSet;
};

#endif // SCHEDULEITEMLQUERYPROCESSOR_H_

