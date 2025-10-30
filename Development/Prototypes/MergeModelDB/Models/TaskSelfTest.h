#ifndef TASKSELFTEST_H_
#define TASKSELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelSelfTest.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class TaskSelfTest : public ModelSelfTest<TaskModel>
{
public:
    TaskSelfTest();
    virtual ~TaskSelfTest() = default;
    virtual bool runSelfTest() override;

private:
/*
 * Unit Testing
 */
    virtual void selfTestResetAllValues() override;
    virtual bool testExceptionHandling() noexcept override;
    bool testExceptionSelectByTaskID();
    bool testExceptionSelectByDescriptionAndAssignedUser();
    bool testExceptionFormatSelectActiveTasksForAssignedUser();
    bool testExceptionFormatSelectUnstartedDueForStartForAssignedUser();
    bool testExceptionFormatSelectTasksCompletedByAssignedAfterDate();
    bool testExceptionFormatSelectTasksByAssignedIDandParentID();
    virtual bool testExceptionInsert() noexcept override;
    virtual bool testExceptionUpdate() noexcept override;
    virtual bool testExceptionRetrieve() noexcept override;
    virtual ModelTestStatus testAllInsertFailures() override;
    bool testDiff();

    virtual bool testAccessorFunctionsPassed() override;
    bool testTaskIdAccesss();
    bool testCreatorIDAccess();
    bool testAssignToIDAccess();
    bool testDescriptionAccess();
    bool testStatusAccess();
    bool testStatusVerifyValueAndGetStatus(TaskStatus testValue);
    bool testParentTaskIDAccess();
    bool testParentTaskIDVerifyValueAndGetParentTaskID(std::size_t testValue);
    bool testPercentageCompleteAccess();
    bool testCreationDateAccess();
    bool testDueDateAccess();
    bool testScheduledStartAccess();
    bool testActualStartDateAccess();
    bool testEstimatedCompletionAccess();
    bool testCompletionDateAccess();
    bool testEstimatedEffortAccess();
    bool testActualEffortToDateAccess();
    bool testPriorityGroupAccess();
    bool testPriorityGroupCAccess();
    bool testPriorityAccess();
    bool testPersonalAccess();
    bool testDependenciesAccess();
    bool testMarkComplete();
    bool testAddEffort();

};

using TaskSelfTest_shp = std::shared_ptr<TaskSelfTest>;

#endif // TASKSELFTEST_H_


