#ifndef SCHEDULEITEMLIST_H_
#define SCHEDULEITEMLIST_H_

// Project Header Files
#include "ListDBInterface.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <string>

using ScheduleItemListValues = std::vector<ScheduleItemModel_shp>;

class ScheduleItemList : public ListDBInterface<ScheduleItemModel>
{
public:
    ScheduleItemList(std::size_t userId);
    virtual ~ScheduleItemList() = default;

    ScheduleItemListValues getUserDaySchedule(std::chrono::year_month_day scheduleDate) noexcept;
    ScheduleItemListValues findUserScheduleItemsByContentAndDateRange(std::string searchTitle,
        std::chrono::year_month_day searchStart, std::chrono::year_month_day searchEnd) noexcept;

private:
    ScheduleItemListValues fillScheduleItemList();
    ScheduleItemListValues runQueryFillScheduleItemList();

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetUserDaySchedule() noexcept;
    TestStatus testExceptionFindUserScheduleItemsByContentAndDateRange() noexcept;

    std::size_t userID;
};

#endif // SCHEDULEITEMLIST_H_

