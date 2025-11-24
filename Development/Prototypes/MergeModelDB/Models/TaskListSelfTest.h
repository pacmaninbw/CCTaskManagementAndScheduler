#ifndef TASKLISTSELFTEST_H_
#define TASKLISTSELFTEST_H_

// Project Header Files
#include "ListSelfTest.h"
#include "TestStatus.h"
#include "TaskList.h"

// Standard C++ Header Files
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

/*
 */
class TaskListSelfTest : public ListSelfTest<TaskList>
{
public:
    TaskListSelfTest();
    virtual ~TaskListSelfTest() = default;

    virtual TestStatus runSelfTest() noexcept override;

protected:
    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetActiveTasksForAssignedUser() noexcept;
    TestStatus testExceptionGetUnstartedDueForStartForAssignedUser() noexcept;
    TestStatus testExceptionGetTasksCompletedByAssignedAfterDate() noexcept;
    TestStatus testExceptionGetTasksByAssignedIDandParentID() noexcept;
};

#endif // TASKLISTSELFTEST_H_


