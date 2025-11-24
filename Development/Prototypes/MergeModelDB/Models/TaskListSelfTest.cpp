
// Project Header Files
#include "TaskListSelfTest.h"
#include "TestStatus.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

TaskListSelfTest::TaskListSelfTest()
{
    verboseOutput = true;
    inSelfTest = true;
    queryGenerator.setSelfTest(true);
    errorMessages.clear();
}

TestStatus TaskListSelfTest::runSelfTest() noexcept
{
    selfTestResetAllValues();

    std::cout << "\nRunning " << itemUnderTest << " Self Test\n";

    return testListExceptionHandling();
}

void TaskListSelfTest::selfTestResetAllValues() noexcept
{
    inSelfTest = true;
    format_opts.reset();
    primaryKeyResults.clear();
    returnType.clear();
    errorMessages.clear();

    queryGenerator.setSelfTest(true);
    queryGenerator.clearModified();
    queryGenerator.clearErrorMessages();
    queryGenerator.testResetFormatOpts();
}

std::vector<ListExceptionTestElement> TaskListSelfTest::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&TaskListSelfTest::testExceptionGetActiveTasksForAssignedUser, this),
        "getActiveTasksForAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskListSelfTest::testExceptionGetUnstartedDueForStartForAssignedUser, this),
        "getUnstartedDueForStartForAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskListSelfTest::testExceptionGetTasksCompletedByAssignedAfterDate, this),
        "getTasksCompletedByAssignedAfterDate"});
    exceptionTests.push_back({std::bind(&TaskListSelfTest::testExceptionGetTasksByAssignedIDandParentID, this),
        "getTasksByAssignedIDandParentID"});

    return exceptionTests;
}

TestStatus TaskListSelfTest::testExceptionGetActiveTasksForAssignedUser() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("TaskListSelfTest::testExceptionGetActiveTasksForAssignedUser()",
         std::bind(&TaskList::getActiveTasksForAssignedUser, this, std::placeholders::_1), 1);
}

TestStatus TaskListSelfTest::testExceptionGetUnstartedDueForStartForAssignedUser() noexcept
{
   selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("TaskListSelfTest::testExceptionGetUnstartedDueForStartForAssignedUser()",
         std::bind(&TaskList::getUnstartedDueForStartForAssignedUser, this, std::placeholders::_1), 1);
}

TestStatus TaskListSelfTest::testExceptionGetTasksCompletedByAssignedAfterDate() noexcept
{
    selfTestResetAllValues();

    std::chrono::year_month_day searchStartDate = getTodaysDate();
    std::size_t assignedUser = 1;

    return testListExceptionAndSuccessNArgs("TaskListSelfTest::testExceptionGetTasksCompletedByAssignedAfterDate()",
         std::bind(&TaskList::getTasksCompletedByAssignedAfterDate, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, searchStartDate);
}

TestStatus TaskListSelfTest::testExceptionGetTasksByAssignedIDandParentID() noexcept
{
    selfTestResetAllValues();

    std::size_t assignedUser = 1;
    std::size_t parentid = 1;

    return testListExceptionAndSuccessNArgs("TaskListSelfTest::testExceptionGetUnstartedDueForStartForAssignedUser()",
         std::bind(&TaskList::getTasksByAssignedIDandParentID, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, parentid);

}
