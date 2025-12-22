#ifndef TESTSCHEDULEITEMMODEL_H_
#define TESTSCHEDULEITEMMODEL_H_

// Project Header Files
#include "TestDBInterfaceCore.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <functional>
#include <string>
#include <vector>

class TestScheduleItemModel : public TestDBInterfaceCore
{
public:
    struct TestScheduleItemInput
    {
        std::string startTimeStr;
        std::string endTimeStr;
        std::string title;
        bool personal;
        std::string location;
    };

    TestScheduleItemModel();
    ~TestScheduleItemModel() = default;

private:
    TestStatus testInsertScheduleItem(TestScheduleItemInput testScheduleItem);
    TestStatus testPositivePathScheduleItemInsertions();
    TestStatus testPositivePathGetScheduleItemsForUserWithSimilarContentDateRange();
    TestStatus testPositivePathGetScheduleItemsForUserByDate();
    TestStatus testNegativePathAlreadyInDataBase();
    TestStatus testnegativePathNotModified();
    TestStatus negativePathMissingRequiredFields();
    TestStatus testMissingRequiredFieldsAddUserID(
        ScheduleItemModel& testScheduleItem, std::vector<std::string>& expectedErrors);
    TestStatus testMissingRequiredFieldsAddTitle(
        ScheduleItemModel& testScheduleItem, std::vector<std::string>& expectedErrors);
    TestStatus testMissingRequiredFieldsAddStartTime(
        ScheduleItemModel& testScheduleItem, std::vector<std::string>& expectedErrors);
    TestStatus testMissingRequiredFieldsAddEndTime(
        ScheduleItemModel& testScheduleItem, std::vector<std::string>& expectedErrors);

    bool verboseOutput;
    std::vector<std::function<bool(ScheduleItemModel_shp)>>positiveTestFuncs;
    std::vector<std::function<bool(void)>> negativePathTestFuncs;
    std::vector<TestScheduleItemInput> testInput;
};

#endif // TESTSCHEDULEITEMMODEL_H_

