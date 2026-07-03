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


    std::size_t m_UserID;
    std::size_t m_ScheduleItemIdIdx = IndexNotSet;
    std::size_t m_UserIdIdx = IndexNotSet;
    std::size_t m_StartTimeIdx = IndexNotSet;
    std::size_t m_EndTimeIdx = IndexNotSet;
    std::size_t m_TitleIdx = IndexNotSet;
    std::size_t m_PersonalIdx = IndexNotSet;
    std::size_t m_LocationIdx = IndexNotSet;
    std::size_t m_CreatedOnIdx = IndexNotSet;
    std::size_t m_LastUpdateIdx = IndexNotSet;
    std::size_t m_HiddenIdx = IndexNotSet;
};

#endif // SCHEDULEITEMLQUERYPROCESSOR_H_

