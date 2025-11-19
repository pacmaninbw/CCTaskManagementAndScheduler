#ifndef TASKSELFTEST_H_
#define TASKSELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelSelfTest.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <memory>
#include <vector>

class TaskSelfTest : public ModelSelfTest<TaskModel>
{
public:
    TaskSelfTest();
    virtual ~TaskSelfTest() = default;
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
    TestStatus testTaskIdAccesss();
    TestStatus testCreatorIDAccess();
    TestStatus testAssignToIDAccess();
    TestStatus testDescriptionAccess();
    TestStatus testStatusAccess();
    TestStatus testStatusVerifyValueAndGetStatus(TaskStatus testValue);
    TestStatus testParentTaskIDAccess();
    TestStatus testParentTaskIDVerifyValueAndGetParentTaskID(std::size_t testValue);
    TestStatus testPercentageCompleteAccess();
    TestStatus testCreationDateAccess();
    TestStatus testDueDateAccess();
    TestStatus testScheduledStartAccess();
    TestStatus testActualStartDateAccess();
    TestStatus testEstimatedCompletionAccess();
    TestStatus testCompletionDateAccess();
    TestStatus testEstimatedEffortAccess();
    TestStatus testActualEffortToDateAccess();
    TestStatus testPriorityGroupAccess();
    TestStatus testPriorityGroupCAccess();
    TestStatus testPriorityAccess();
    TestStatus testPersonalAccess();
    TestStatus testDependenciesAccess();
    TestStatus testMarkComplete();
    TestStatus testAddEffort();

};

using TaskSelfTest_shp = std::shared_ptr<TaskSelfTest>;

#endif // TASKSELFTEST_H_


