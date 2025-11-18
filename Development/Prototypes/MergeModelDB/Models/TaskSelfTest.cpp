// Project Header Files
#include "commonUtilities.h"
#include "GenericDictionary.h"
#include "TaskSelfTest.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

TaskSelfTest::TaskSelfTest()
{
}

TestStatus TaskSelfTest::runSelfTest() noexcept
{
    inSelfTest = true;
    TestStatus selfTestStatus = TESTPASSED;

    std::cout << "\nRunning " << modelName << " Self Test\n";

    if (testExceptionHandling()!= TESTPASSED)
    {
        std::cerr  << modelName << "::runSelfTest: Exception handling FAILED!\n";
        selfTestStatus = TESTFAILED;
    }
    
    if (testSave() == TESTFAILED)
    {
        selfTestStatus = TESTFAILED;
    }

    if (testAttributeAccessFunctions() == TESTFAILED)
    {
        std::cerr << modelName << "::runSelfTest: One or more get or set functions FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testEqualityOperator() == TESTFAILED)
    {
        std::cerr << std::format("Equality Operator Test: Comparing 2 {}s FAILED!\n", modelName);
        selfTestStatus = TESTFAILED;
    }

    testOutput();

    if (testAllInsertFailures() != TESTPASSED)
    {
        std::cerr << "Test of all insertion failures FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Insertion Failure Test PASSED!\n";
    }

    if (testCommonUpdateFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Update Failure Test PASSED!\n";
    }

    if (testTextFieldManipulation() == TESTFAILED)
    {
        selfTestStatus = TESTFAILED;
    }

    inSelfTest = false;
    
    if (selfTestStatus == TESTPASSED)
    {
        std::cout <<  std::format("{} Self Test {}\n", modelName, "PASSED");
    }
    else
    {
        std::cerr <<  std::format("{} Self Test {}\n", modelName, "FAILED");
    }

    return selfTestStatus;
}


void TaskSelfTest::selfTestResetAllValues() noexcept
{
    ModelSelfTest::selfTestResetAllValues();

    creatorID = 0;
    assignToID = 0;
    description.clear();
    status.reset();
    parentTaskID.reset();
    percentageComplete = 0.0;
    creationTimeStamp.reset();
    dueDate.reset();
    scheduledStart.reset();
    actualStartDate.reset();
    estimatedCompletion.reset();
    completionDate.reset();
    estimatedEffort = 0;
    actualEffortToDate = 0.0;
    priorityGroup = 0;
    priority = 0;
    personal = false;
    dependencies.clear();
    lastUpdate.reset();
}

std::vector<ExceptionTestElement> TaskSelfTest::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionSelectByTaskID, this), "selectByTaskID"});
    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionSelectByDescriptionAndAssignedUser, this), "selectByDescriptionAndAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionInsert, this), "testExceeptionInsert"});
    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionRetrieve, this), "testExceptionRetrieve"});
    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionFormatSelectActiveTasksForAssignedUser, this),
        "selectByDescriptionAndAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionFormatSelectUnstartedDueForStartForAssignedUser, this),
        "formatSelectActiveTasksForAssignedUser"});
    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionFormatSelectTasksCompletedByAssignedAfterDate, this),
        "formatSelectTasksCompletedByAssignedAfterDate"});
    exceptionTests.push_back({std::bind(&TaskSelfTest::testExceptionFormatSelectTasksByAssignedIDandParentID, this),
        "formatSelectTasksByAssignedIDandParentID"});

    return exceptionTests;
}

TestStatus TaskSelfTest::testExceptionSelectByTaskID()
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("TaskSelfTest::selectByTaskID", std::bind(&TaskModel::selectByTaskID, this, std::placeholders::_1), 0);
}

TestStatus TaskSelfTest::testExceptionSelectByDescriptionAndAssignedUser()
{
    selfTestResetAllValues();
    return testExceptionAndSuccessNArgs
        ("TaskSelfTest::selectByDescriptionAndAssignedUser",
            std::bind(&TaskModel::selectByDescriptionAndAssignedUser, this, std::placeholders::_1, std::placeholders::_2),
            "A task description", 1);
}

TestStatus TaskSelfTest::testExceptionFormatSelectActiveTasksForAssignedUser()
{
    selfTestResetAllValues();
    return testFormatExceptionAndSuccessNArgs("TaskSelfTest::formatSelectActiveTasksForAssignedUser",
        std::bind(&TaskModel::formatSelectActiveTasksForAssignedUser, this, std::placeholders::_1), 1);
}

TestStatus TaskSelfTest::testExceptionFormatSelectUnstartedDueForStartForAssignedUser()
{
    selfTestResetAllValues();
    return testFormatExceptionAndSuccessNArgs("TaskSelfTest::formatSelectUnstartedDueForStartForAssignedUser",
        std::bind(&TaskModel::formatSelectUnstartedDueForStartForAssignedUser, this, std::placeholders::_1), 1);
}

TestStatus TaskSelfTest::testExceptionFormatSelectTasksCompletedByAssignedAfterDate()
{
    selfTestResetAllValues();

    std::chrono::year_month_day searchStartDate = getTodaysDate();
    std::size_t assignedUser = 1;

    return testFormatExceptionAndSuccessNArgs("TaskSelfTest::formatSelectTasksCompletedByAssignedAfterDate",
        std::bind(&TaskModel::formatSelectTasksCompletedByAssignedAfterDate, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, searchStartDate);
}
 
TestStatus TaskSelfTest::testExceptionFormatSelectTasksByAssignedIDandParentID()
{
    selfTestResetAllValues();

    std::size_t assignedUser = 1;
    std::size_t parentid = 1;

    return testFormatExceptionAndSuccessNArgs("TaskSelfTest::formatSelectTasksByAssignedIDandParentID",
        std::bind(&TaskModel::formatSelectTasksByAssignedIDandParentID, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, parentid);
}

TestStatus TaskSelfTest::testExceptionInsert() noexcept
{
    selfTestResetAllValues();
    forceException = true;

    setCreatorID(1);
    setAssignToID(2);
    setDescription("Testing Exception handling");
    setEstimatedEffort(6);
    setScheduledStart(getTodaysDate());
    setDueDate(getTodaysDatePlus(2));
    setPriorityGroup('A');
    setPriority(1);
    setParentTaskID(1);
    addDependency(3);
    addDependency(5);
    addDependency(7);

    if (testFormatExceptionCatchSuccessNArgs(
        "TaskModel::formatInsertStatement", std::bind(&TaskSelfTest::formatInsertStatement, this)) == TESTFAILED)
    {
        std::cerr << "TaskSelfTest::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("TaskModel::insert", std::bind(&TaskModel::insert, this));
}

TestStatus TaskSelfTest::testExceptionUpdate() noexcept
{
    selfTestResetAllValues();

    forceException = true;
    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();

    setTaskID(1);
    setDescription("Testing Exception handling");
    setCreatorID(1);
    setAssignToID(2);
    setEstimatedEffort(6);
    setScheduledStart(getTodaysDate());
    setDueDate(getTodaysDatePlus(2));
    setPriorityGroup('A');
    setPriority(1);
    setCreationDate(timeStamp);
    addDependency(2);
    addDependency(4);
    addDependency(6);
    
    if (testFormatExceptionCatchSuccessNArgs(
        "TaskSelfTest::formatUpdateStatement", std::bind(&TaskSelfTest::formatUpdateStatement, this)) == TESTFAILED)
    {
        std::cerr << "TaskSelfTest::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("TaskSelfTest::update", std::bind(&TaskModel::update, this));
}

TestStatus TaskSelfTest::testExceptionRetrieve() noexcept
{
    selfTestResetAllValues();
    
    setTaskID(37);

    if (testFormatExceptionCatchSuccessNArgs(
        "TaskSelfTest::formatSelectStatement", std::bind(&TaskSelfTest::formatSelectStatement, this)) == TESTFAILED)
    {
        std::cerr << "TaskSelfTest::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("TaskSelfTest::retrieve", std::bind(&TaskModel::retrieve, this));
}


TestStatus TaskSelfTest::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "description", "user ID for creator", "user ID for assigned user", "estimated effort in hours",
            "priority", "scheduled start date", "due date (deadline)", " missing required values"
    };

    setTaskID(0);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setDescription("Test missing required fields: Set Description");

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setCreatorID(1);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setAssignToID(1);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setEstimatedEffort(5);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setPriorityGroup(1);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setScheduledStart(getTodaysDateMinus(OneWeek));

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setDueDate(getTodaysDatePlus(1));

    expectedErrors.clear();
    errorMessages.clear();

    if (verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", modelName, __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In  TaskSelfTest::testAllInsertFailures() Expected successful insert failed\n" << errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus TaskSelfTest::testEqualityOperator() noexcept
{
    TaskModel other;

    other.setTaskID(primaryKey);

    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;

    return (*this == other)? TESTPASSED : TESTFAILED;
}

std::vector<std::function<TestStatus(void)>> TaskSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<std::function<TestStatus(void)>> attributeAccessTests; 
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testTaskIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testCreatorIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testAssignToIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testDescriptionAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testStatusAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testParentTaskIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testPercentageCompleteAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testCreationDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testDueDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testScheduledStartAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testActualStartDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testEstimatedCompletionAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testCompletionDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testEstimatedEffortAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testActualEffortToDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testPriorityGroupAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testPriorityGroupCAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testPriorityAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testPersonalAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testDependenciesAccess, this)});
    attributeAccessTests.push_back({std::bind(&TaskSelfTest::testMarkComplete, this)});

    return attributeAccessTests;
}

TestStatus TaskSelfTest::testTaskIdAccesss()
{
    std::size_t testPrimaryKey = 33;

    return testAccessorFunctions<std::size_t>(testPrimaryKey, &primaryKey, "Primary Key",
        std::bind(&TaskModel::setTaskID, this, std::placeholders::_1),
        std::bind(&TaskModel::getTaskID, this));
}

TestStatus TaskSelfTest::testCreatorIDAccess()
{
    std::size_t testValue = 1;

    return testAccessorFunctions<std::size_t>(testValue, &creatorID, "Creator User ID",
        std::bind(&TaskModel::setCreatorID, this, std::placeholders::_1),
        std::bind(&TaskModel::getCreatorID, this));
}

TestStatus TaskSelfTest::testAssignToIDAccess()
{
    std::size_t testValue = 2;

    return testAccessorFunctions<std::size_t>(testValue, &assignToID, "Assigned User ID",
        std::bind(&TaskModel::setAssignToID, this, std::placeholders::_1),
        std::bind(&TaskModel::getAssignToID, this));
}

TestStatus TaskSelfTest::testDescriptionAccess()
{
    std::string testValue("Test task description access");
    return testAccessorFunctions<std::string>(testValue, &description, "Description",
        std::bind(&TaskModel::setDescription, this, std::placeholders::_1),
        std::bind(&TaskModel::getDescription, this));
}

TestStatus TaskSelfTest::testStatusAccess()
{
    TestStatus testStatus = TESTPASSED;
    TaskStatus testValue = TaskSelfTest::TaskStatus::On_Hold;

    std::cout << "Running self test on set and get functions for TaskSelfTest::status\n";

    modified = false;
    setStatus(testValue);
    if (testStatusVerifyValueAndGetStatus(testValue) == TESTFAILED)
    {
        testStatus = TESTFAILED;
    }

    modified = false;
    testValue = TaskSelfTest::TaskStatus::Complete;
    std::string statusName = taskStatusString(testValue);
    setStatus(statusName);
    if (testStatusVerifyValueAndGetStatus(testValue) == TESTFAILED)
    {
        testStatus = TESTFAILED;
    }

    std::cout << "Self test on set and get functions for TaskSelfTest::status PASSED\n";

    return testStatus;
}

TestStatus TaskSelfTest::testStatusVerifyValueAndGetStatus(TaskStatus testValue)
{
    if (!isModified())
    {
        std::cerr << "In self test for: TaskSelfTest::status set function for status FAILED to set modified\n";
        return TESTFAILED;
    }

    if (!status.has_value() || status.value() != testValue)
    {
        if (!status.has_value())
        {
            std::cerr  << "In self test for: TaskSelfTest::status Set function for status FAILED to set member value\n";
        }
        if (status.value() != testValue)
        {
            std::cerr  << "In self test for: TaskSelfTest::status expected value: " << static_cast<unsigned int>(testValue)
                        << "actual value: " << static_cast<unsigned int>(status.value()) << " FAILED to set member value\n";
        }
        return TESTFAILED;
    }

    std::optional<TaskSelfTest::TaskStatus> returnValue = getStatus();
    if (!returnValue.has_value() || returnValue.value() != testValue)
    {
        std::cerr  << "In self test for: TaskSelfTest::status Get function for status FAILED\n";
        return TESTFAILED;
    }

    if (getStatusIntVal() != static_cast<unsigned int>(returnValue.value()))
    {
        std::cerr  << "In self test for: TaskModel::getStatusIntVal() FAILED\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}


TestStatus TaskSelfTest::testParentTaskIDAccess()
{
    TestStatus testStatus = TESTPASSED;
    std::size_t testValue = 37;

    std::cout << "Running self test on set and get functions for TaskSelfTest::parentTaskID\n";

    modified = false;
    setParentTaskID(testValue);
    if (testParentTaskIDVerifyValueAndGetParentTaskID(testValue) == TESTFAILED)
    {
        testStatus = TESTFAILED;
    }

    testValue = 42;
    TaskModel_shp parent = std::make_shared<TaskModel>(TaskModel());
    parent->setTaskID(testValue);
    modified = false;
    setParentTaskID(parent);
    if (testParentTaskIDVerifyValueAndGetParentTaskID(testValue) == TESTFAILED)
    {
        testStatus = TESTFAILED;
    }

    if (testStatus == TESTPASSED)
    {
        std::cout << "Self test on set and get functions for TaskSelfTest::parentTaskID PASSED\n";
    }
    else
    {
        std::cout << "Self test on set and get functions for TaskSelfTest::parentTaskID FAILED\n";
    }

    return testStatus;
}

TestStatus TaskSelfTest::testParentTaskIDVerifyValueAndGetParentTaskID(std::size_t testValue)
{
    if (!isModified())
    {
        std::cerr << "In self test for: TaskSelfTest::parentID set function for parentID FAILED to set modified\n";
        return TESTFAILED;
    }

    if (!parentTaskID.has_value() || parentTaskID.value() != testValue)
    {
        if (!parentTaskID.has_value())
        {
            std::cerr  << "In self test for: TaskSelfTest::parentTaskID Set function for parentTaskID FAILED to set member value\n";
        }
        if (parentTaskID.value() != testValue)
        {
            std::cerr  << "In self test for: TaskSelfTest::parentTaskID expected value: " << testValue <<
                "actual value: " << parentTaskID.value() << " FAILED to set member value\n";
        }
        return TESTFAILED;
    }

    std::optional<std::size_t> returnValue = getParentTaskID();
    if (!returnValue.has_value() || returnValue.value() != testValue)
    {
        std::cerr  << "In self test for: TaskSelfTest::parentTaskID Get function for parentTaskID FAILED\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}


TestStatus TaskSelfTest::testPercentageCompleteAccess()
{
    double testValue = 86.5;
    return testAccessorFunctions<double>(testValue, &percentageComplete, "Percentage Complete",
        std::bind(&TaskModel::setPercentageComplete, this, std::placeholders::_1),
        std::bind(&TaskModel::getPercentageComplete, this));
}

TestStatus TaskSelfTest::testCreationDateAccess()
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();
    return testTimeStampAccessorFunctions(testValue, &creationTimeStamp, "Creation TimeStamp",
        std::bind(&TaskModel::setCreationDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getCreationDate, this));
}

TestStatus TaskSelfTest::testDueDateAccess()
{
    std::chrono::year_month_day testValue = getTodaysDate();
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &dueDate, "Due Date",
        std::bind(&TaskModel::setDueDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getDueDate, this));
}

TestStatus TaskSelfTest::testScheduledStartAccess()
{
    std::chrono::year_month_day testValue = getTodaysDateMinus(OneWeek);
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &scheduledStart, "Scheduled Start Date",
        std::bind(&TaskModel::setScheduledStart, this, std::placeholders::_1),
        std::bind(&TaskModel::getScheduledStart, this));
}

TestStatus TaskSelfTest::testActualStartDateAccess()
{
    std::chrono::year_month_day testValue = getTodaysDateMinus(2);
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &actualStartDate, "Actual Start Date",
        std::bind(&TaskModel::setactualStartDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getactualStartDate, this));
}

TestStatus TaskSelfTest::testEstimatedEffortAccess()
{
    unsigned int testValue = 1024;

    return testAccessorFunctions<unsigned int>(testValue, &estimatedEffort, "Estimated Effort",
        std::bind(&TaskModel::setEstimatedEffort, this, std::placeholders::_1),
        std::bind(&TaskModel::getEstimatedEffort, this));
}

TestStatus TaskSelfTest::testCompletionDateAccess()
{
    std::chrono::year_month_day testValue = getTodaysDate();
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &completionDate, "Completion Date",
        std::bind(&TaskModel::setCompletionDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getCompletionDate, this));
}

TestStatus TaskSelfTest::testEstimatedCompletionAccess()
{
    std::chrono::year_month_day testValue = getTodaysDate();
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &estimatedCompletion, "Completion Date",
        std::bind(&TaskModel::setEstimatedCompletion, this, std::placeholders::_1),
        std::bind(&TaskModel::getEstimatedCompletion, this));
}

TestStatus TaskSelfTest::testActualEffortToDateAccess()
{
    double testValue = 752.75;
    if (testAccessorFunctions<double>(testValue, &actualEffortToDate, "Actual Effort to date",
        std::bind(&TaskModel::setActualEffortToDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getactualEffortToDate, this)) == TESTFAILED)
    {
        return TESTFAILED;
    }

    return testAddEffort();
}

TestStatus TaskSelfTest::testPriorityGroupAccess()
{
    unsigned int testValue = 3;

    return testAccessorFunctions<unsigned int>(testValue, &priorityGroup, "Priority Group Int Value",
        std::bind(&TaskModel::setPriorityGroup, this, std::placeholders::_1),
        std::bind(&TaskModel::getPriorityGroup, this));
}

TestStatus TaskSelfTest::testPriorityGroupCAccess()
{
    const char testValue = 'B';
    unsigned int expectedInternalValue = testValue - 'A' + 1;
    std::string_view memberName("Priority Group Character");

    std::cout << "Running self test on set and get functions for " << modelName << "::" << memberName << "\n";

    modified = false;

    setPriorityGroupC(testValue);
    if (!isModified())
    {
        std::cerr << "In self test for: " << modelName << " set function for " << memberName << " FAILED to set modified\n";
        return TESTFAILED;
    }

    if (priorityGroup != expectedInternalValue)
    {
        std::cerr  << "In self test for: " << modelName << "Set function for " << memberName << " FAILED to set member value\n";
        std::cerr << "\tExpected Value: " << expectedInternalValue << "Actual Value: " << priorityGroup << "\n";
        return TESTFAILED;
    }

    if (getPriorityGroup() != expectedInternalValue)
    {
        std::cerr  << "In self test for: " << modelName << " Get function for " << memberName << " FAILED\n";
        std::cerr << "\tExpected Value: " << expectedInternalValue << "Actual Value: " << getPriorityGroup() << "\n";
        return TESTFAILED;
    }

    std::cout << "Self test on set and get functions for " << modelName << "::" << memberName << " PASSED\n";

    return TESTPASSED;
}

TestStatus TaskSelfTest::testPriorityAccess()
{
    unsigned int testValue = 5;

    return testAccessorFunctions<unsigned int>(testValue, &priority, "Priority",
        std::bind(&TaskModel::setPriority, this, std::placeholders::_1),
        std::bind(&TaskModel::getPriority, this));
}

TestStatus TaskSelfTest::testPersonalAccess()
{
    bool testValue = true;

    return testAccessorFunctions<bool>(testValue, &personal, "Personal",
        std::bind(&TaskModel::setPersonal, this, std::placeholders::_1),
        std::bind(&TaskSelfTest::isPersonal, this));
}

TestStatus TaskSelfTest::testDependenciesAccess()
{
    modified = false;

    std::cout << "Running self test on add and get functions for " << modelName << "::dependencies\n";

    std::vector<std::size_t> testDependencies;
    testDependencies.push_back(1);
    testDependencies.push_back(3);
    testDependencies.push_back(5);

    for (auto dependency: testDependencies)
    {
        addDependency(dependency);
        if (!isModified())
        {
            std::cerr << "In self test for: " << modelName << "::addDependency()" << " FAILED to set modified\n";
            return TESTFAILED;
        }
    }

    if (dependencies != testDependencies)
    {
        std::cerr << "Self Test for " << modelName << "::addDependency()" << " FAILED to set values\n";
        return TESTFAILED;
    }

    if (getDependencies() != testDependencies)
    {
        std::cerr << "Self Test for " << modelName << "::getDependencies()" << " FAILED to get values\n";
        return TESTFAILED;
    }

    std::cout << "Self test on set and get functions for " << modelName << "::dependencoies PASSED\n";

    return TESTPASSED;
}

/*
 * This test assumes that getStatus() and getCompletionDate() are tested elsewhere.
 */
TestStatus TaskSelfTest::testMarkComplete()
{
    std::cout << "Running self test for TaskSelfTest::markComplete\n";

    modified = false;
    status.reset();
    completionDate.reset();

    markComplete();

    if (!status.has_value() || status.value() != TaskStatus::Complete)
    {
        if (!status.has_value())
        {
            std::cerr  << "In self test for: TaskSelfTest::markComplete FAILED to set status member value\n";
        }
        if (status.value() != TaskStatus::Complete)
        {
            std::cerr  << "In self test for: TaskSelfTest::markComplete expected value: " << static_cast<unsigned int>(TaskStatus::Complete)
                << "actual value: " << static_cast<unsigned int>(status.value()) << " FAILED to set status member value\n";
        }
        return TESTFAILED;
    }

    if (!completionDate.has_value())
    {
        std::cerr  << "In self test for: TaskSelfTest::markComplete FAILED to set completionDate member value\n";
        return TESTFAILED;
    }

    std::cout << "Self test for TaskSelfTest::markComplete PASSED\n";

    return TESTPASSED;
}

TestStatus TaskSelfTest::testAddEffort()
{
    double addedHours = 7.25;
    double expectedHours = actualEffortToDate + addedHours;

    modified = false;
    addEffortHours(addedHours);

    if (!isModified())
    {
        std::cerr << "TaskSelfTest::addEffortHours() failed to set modified! test FAILED;\n";
        return TESTFAILED;
    }

    if (actualEffortToDate != expectedHours)
    {
        std::cerr << "TaskSelfTest::actualEffortToDate does not show additional hours! test FAILED;\n";
        return TESTFAILED;

    }

    if (getactualEffortToDate() != expectedHours)
    {
        std::cerr << "TaskModel::getactualEffortToDate() does not show additional hours! test FAILED;\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

void TaskSelfTest::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

