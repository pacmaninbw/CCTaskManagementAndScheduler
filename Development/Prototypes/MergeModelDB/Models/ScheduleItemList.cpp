// Project Header Files
#include "ListDBInterface.h"
#include "ScheduleItemList.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>

ScheduleItemList::ScheduleItemList(std::size_t userId)
: ListDBInterface<ScheduleItemModel>()
{
    userID = userId;
}

ScheduleItemListValues ScheduleItemList::getUserDaySchedule(std::chrono::year_month_day scheduleDate) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In ScheduleItemList::getUserDaySchedule : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectScheduleItemsByDateAndUser(scheduleDate, userID);
        return runQueryFillScheduleItemList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return ScheduleItemListValues();
}

ScheduleItemListValues ScheduleItemList::findUserScheduleItemsByContentAndDateRange(
    std::string searchTitle,
    std::chrono::year_month_day searchStart,
    std::chrono::year_month_day searchEnd
) noexcept
{
    errorMessages.clear();
    appendErrorMessage("In ScheduleItemList::getUserDaySchedule : ");

    try
    {
        firstFormattedQuery = queryGenerator->formatSelectSiByContentDateRangeUser(searchTitle, searchStart, searchEnd, userID);
        return runQueryFillScheduleItemList();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return ScheduleItemListValues();
}

ScheduleItemListValues ScheduleItemList::fillScheduleItemList()
{
    ScheduleItemListValues scheduleItemList;

    for (auto scheduleItemID: primaryKeyResults)
    {
        ScheduleItemModel_shp newScheduleItem = std::make_shared<ScheduleItemModel>(ScheduleItemModel());
        newScheduleItem->setScheduleItemID(scheduleItemID);
        newScheduleItem->setUserID(userID);
        newScheduleItem->retrieve();
        scheduleItemList.push_back(newScheduleItem);
    }

    return scheduleItemList;
}

ScheduleItemListValues ScheduleItemList::runQueryFillScheduleItemList()
{
    if (firstFormattedQuery.empty())
    {
        appendErrorMessage(std::format("Formatting select multiple schedule items query string failed {}",
            queryGenerator->getAllErrorMessages()));
        return ScheduleItemListValues();
    }
    
    if (runFirstQuery())
    {
        return fillScheduleItemList();
    }

    return ScheduleItemListValues();
}

std::vector<ListExceptionTestElement> ScheduleItemList::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&ScheduleItemList::testExceptionGetUserDaySchedule, this), "getUserDaySchedule"});
    exceptionTests.push_back({std::bind(&ScheduleItemList::testExceptionFindUserScheduleItemsByContentAndDateRange, this),
        "findUserScheduleItemsByContentAndDateRange"});

    return exceptionTests;
}

TestStatus ScheduleItemList::testExceptionGetUserDaySchedule() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("ScheduleItemList::testExceptionGgetUserDaySchedule()",
         std::bind(&ScheduleItemList::getUserDaySchedule, this, std::placeholders::_1), getTodaysDate());
}

TestStatus ScheduleItemList::testExceptionFindUserScheduleItemsByContentAndDateRange() noexcept
{
    selfTestResetAllValues();

    std::string titleSearch("Title search");
    std::chrono::year_month_day testStart(getTodaysDateMinus(1));
    std::chrono::year_month_day testEnd(getTodaysDatePlus(1));

    return testListExceptionAndSuccessNArgs("ScheduleItemList::testExceptionFindUserScheduleItemsByContentAndDateRange()",
         std::bind(&ScheduleItemList::findUserScheduleItemsByContentAndDateRange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
        titleSearch, testStart, testEnd);
}
