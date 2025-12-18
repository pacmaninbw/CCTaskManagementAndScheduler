#ifndef SCHEDULEITEMSELFTEST_H_
#define SCHEDULEITEMSELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelSelfTest.h"
#include "ScheduleItemModel.h"

// Standard C++ Header Files
#include <memory>
#include <vector>

class ScheduleItemSelfTest : public ModelSelfTest<ScheduleItemModel>
{
public:
    ScheduleItemSelfTest();
    ~ScheduleItemSelfTest() = default;
    virtual TestStatus runSelfTest() noexcept override;

private:
/*
 * Unit Testing
 */
    virtual void selfTestResetAllValues() noexcept override;
    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept override;
    TestStatus testExceptionSelectByTaskID();
    TestStatus testExceptionSelectByDescriptionAndAssignedUser();
    TestStatus testExceptionFormatSelectActiveTasksForAssignedUser();
    TestStatus testExceptionFormatSelectUnstartedDueForStartForAssignedUser();
    TestStatus testExceptionFormatSelectTasksCompletedByAssignedAfterDate();
    TestStatus testExceptionFormatSelectTasksByAssignedIDandParentID();
    virtual TestStatus testExceptionInsert() noexcept override;
    virtual TestStatus testExceptionUpdate() noexcept override;
    virtual TestStatus testExceptionRetrieve() noexcept override;
    virtual TestStatus testAllInsertFailures() override;
    virtual TestStatus testEqualityOperator() noexcept override;
    virtual void testOutput() noexcept override;

    virtual std::vector<AttributeTestFunction> initAttributeAccessTests() noexcept override;
    TestStatus testScheduleItemIDAccess() noexcept;
    TestStatus testUserIDAccess() noexcept;
    TestStatus testTitleAccess() noexcept;
    TestStatus testScheduleItemTypeAccess() noexcept;
    TestStatus testStartTimeAccess() noexcept;
    TestStatus testEndTimeAccess() noexcept;
    TestStatus testCreationTimeStampAccess() noexcept;
    TestStatus testLastUpDateTimeStampAccess() noexcept;
    TestStatus testLocationAccess() noexcept;
    TestStatus testPersonalAccess() noexcept;

};

using ScheduleItemSelfTest_shp = std::shared_ptr<ScheduleItemSelfTest>;

#endif  // SCHEDULEITEMSELFTEST_H_

