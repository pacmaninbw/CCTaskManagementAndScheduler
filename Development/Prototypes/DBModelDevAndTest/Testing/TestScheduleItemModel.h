#ifndef TESTSCHEDULEITEMMODEL_H_
#define TESTSCHEDULEITEMMODEL_H_

class UserModel;

// Project Header Files
#include "TestDBInterfaceCore.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <functional>
#include <memory>
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
    virtual bool databaseObjectInitAfterConstructor() override;
    TestStatus testInsertScheduleItem(TestScheduleItemInput testScheduleItem);
    TestStatus testPositivePathScheduleItemInsertions();
    TestStatus testPositivePathGetScheduleItemsForUserWithSimilarTitleDateRange() const;
    TestStatus testPositivePathGetScheduleItemsForUserByDate() const;
    TestStatus testPositivePathUpdateScheduleItem() const;
    TestStatus testPositivePathFindEventSToRepeat() const;
    TestStatus testPositivePathFindEventsToRepeatCompletion() const;
    TestStatus testPositivePathFindLocationsToRepeatCompletion() const;
    TestStatus testPositivePathDeleteScheduleItem() const;
    TestStatus testNegativePathAlreadyInDataBase() const;
    TestStatus testnegativePathNotModified() const;
    TestStatus negativePathMissingRequiredFields() const;
    TestStatus testMissingRequiredFieldsAddUserID(
        ScheduleItemModel& testScheduleItem, std::vector<std::string>& expectedErrors) const;
    TestStatus testMissingRequiredFieldsAddTitle(
        ScheduleItemModel& testScheduleItem, std::vector<std::string>& expectedErrors) const;
    TestStatus testMissingRequiredFieldsAddStartTime(
        ScheduleItemModel& testScheduleItem, std::vector<std::string>& expectedErrors) const;
    TestStatus testMissingRequiredFieldsAddEndTime(
        ScheduleItemModel& testScheduleItem, std::vector<std::string>& expectedErrors) const;

    std::vector<TestScheduleItemInput> m_testInput;
    std::shared_ptr<UserModel> m_userOne;
    std::size_t m_firstScheduleItem = 0;
};

#endif // TESTSCHEDULEITEMMODEL_H_

