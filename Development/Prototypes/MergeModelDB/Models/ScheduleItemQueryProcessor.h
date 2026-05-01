#ifndef SCHEDULEITEMLQUERYPROCESSOR_H_
#define SCHEDULEITEMLQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <string>
#include <vector>

using ScheduleItemQueryProcessorValues = std::vector<ScheduleItemModel_shp>;

class ScheduleItemQueryProcessor : public QueryProcessor<ScheduleItemModel>
{
public:
    ScheduleItemQueryProcessor(std::size_t userId);
    virtual ~ScheduleItemQueryProcessor() = default;

    ScheduleItemQueryProcessorValues getUserDaySchedule(std::chrono::year_month_day scheduleDate) noexcept;
    ScheduleItemQueryProcessorValues findUserScheduleItemsByContentAndDateRange(std::string searchTitle,
        std::chrono::year_month_day searchStart, std::chrono::year_month_day searchEnd) noexcept;
    std::vector<std::string> findEventSToRepeat(std::string searchTitle) noexcept;
    std::vector<std::string> findEventsForRepeatCompletion() noexcept;
    std::vector<std::string> findLocationsForRepeatCompletion() noexcept; 

private:
    ScheduleItemQueryProcessorValues fillScheduleItemQueryProcessor();
    ScheduleItemQueryProcessorValues runQueryFillScheduleItemQueryProcessor();

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetUserDaySchedule() noexcept;
    TestStatus testExceptionFindUserScheduleItemsByContentAndDateRange() noexcept;
    TestStatus testExceptionFindEventSToRepeat() noexcept;
    TestStatus testExceptionFindEventsForRepeatCompletion() noexcept;
    TestStatus testExceptionFindLocationsForRepeatCompletion() noexcept;

    std::size_t userID;
};

#endif // SCHEDULEITEMLQUERYPROCESSOR_H_

