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

bool TaskSelfTest::runSelfTest()
{
    inSelfTest = true;
    bool allSelfTestsPassed = true;

    if (verboseOutput)
    {
        std::clog <<  std::format("Running {} Self Test\n", modelName);
    }

    if (!testExceptionHandling())
    {
        std::clog << "Exception handling FAILED!\n";
        allSelfTestsPassed = false;
    }
    
    if (!testSave())
    {
        allSelfTestsPassed = false;
    }

    if (!testAccessorFunctionsPassed())
    {
        std::clog << "One or more get or set functions FAILED!\n";
        allSelfTestsPassed = false;
    }

    if (!testDiff())
    {
        std::clog << std::format("Comparing 2 {} FAILED!\n", modelName);
        allSelfTestsPassed = false;
    }

    if (verboseOutput)
    {
        std::clog << "Test Output: " << *this << "\n";
    }

    if (testAllInsertFailures() != TESTPASSED)
    {
        std::clog << "Test of all insertion failures FAILED!\n";
        allSelfTestsPassed = false;
    }

    inSelfTest = false;
    
    std::clog <<  std::format("{} Self Test {}\n", modelName, allSelfTestsPassed? "PASSED" : "FAILED");

    return allSelfTestsPassed;
}

void TaskSelfTest::selfTestResetAllValues()
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

bool TaskSelfTest::testExceptionHandling() noexcept
{
    selfTestResetAllValues();

    bool exceptionHandlingPassed = true;
    bool globalForceException = forceException;

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

    if (!forceExceptionsLoop(exceptionTests))
    {
        exceptionHandlingPassed = false;
    }

    forceException = globalForceException;

    return exceptionHandlingPassed;
}

bool TaskSelfTest::testExceptionSelectByTaskID()
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("TaskSelfTest::selectByTaskID", std::bind(&TaskModel::selectByTaskID, this, std::placeholders::_1), 0);
}

bool TaskSelfTest::testExceptionSelectByDescriptionAndAssignedUser()
{
    selfTestResetAllValues();
    return testExceptionAndSuccessNArgs
        ("TaskSelfTest::selectByDescriptionAndAssignedUser",
            std::bind(&TaskModel::selectByDescriptionAndAssignedUser, this, std::placeholders::_1, std::placeholders::_2),
            "A task description", 1);
}

bool TaskSelfTest::testExceptionFormatSelectActiveTasksForAssignedUser()
{
    selfTestResetAllValues();
    return testFormatExceptionAndSuccessNArgs("TaskSelfTest::formatSelectActiveTasksForAssignedUser",
        std::bind(&TaskModel::formatSelectActiveTasksForAssignedUser, this, std::placeholders::_1), 1);
}

bool TaskSelfTest::testExceptionFormatSelectUnstartedDueForStartForAssignedUser()
{
    selfTestResetAllValues();
    return testFormatExceptionAndSuccessNArgs("TaskSelfTest::formatSelectUnstartedDueForStartForAssignedUser",
        std::bind(&TaskModel::formatSelectUnstartedDueForStartForAssignedUser, this, std::placeholders::_1), 1);
}

bool TaskSelfTest::testExceptionFormatSelectTasksCompletedByAssignedAfterDate()
{
    selfTestResetAllValues();

    std::chrono::year_month_day searchStartDate = getTodaysDate();
    std::size_t assignedUser = 1;

    return testFormatExceptionAndSuccessNArgs("TaskSelfTest::formatSelectTasksCompletedByAssignedAfterDate",
        std::bind(&TaskModel::formatSelectTasksCompletedByAssignedAfterDate, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, searchStartDate);
}
 
bool TaskSelfTest::testExceptionFormatSelectTasksByAssignedIDandParentID()
{
    selfTestResetAllValues();

    std::size_t assignedUser = 1;
    std::size_t parentid = 1;

    return testFormatExceptionAndSuccessNArgs("TaskSelfTest::formatSelectTasksByAssignedIDandParentID",
        std::bind(&TaskModel::formatSelectTasksByAssignedIDandParentID, this, std::placeholders::_1, std::placeholders::_2),
        assignedUser, parentid);
}

bool TaskSelfTest::testExceptionInsert() noexcept
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

    if (!testFormatExceptionCatchSuccessNArgs("TaskModel::formatInsertStatement", std::bind(&TaskSelfTest::formatInsertStatement, this)))
    {
        std::clog << "TaskSelfTest::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return false;
    }

    return testExceptionAndSuccessNArgs("TaskModel::insert", std::bind(&TaskModel::insert, this));
}

bool TaskSelfTest::testExceptionUpdate() noexcept
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
    
    if (!testFormatExceptionCatchSuccessNArgs("TaskSelfTest::formatUpdateStatement", std::bind(&TaskSelfTest::formatUpdateStatement, this)))
    {
        std::clog << "TaskSelfTest::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return false;
    }

    return testExceptionAndSuccessNArgs("TaskSelfTest::update", std::bind(&TaskModel::update, this));
}

bool TaskSelfTest::testExceptionRetrieve() noexcept
{
    selfTestResetAllValues();
    
    setTaskID(37);

    if (!testFormatExceptionCatchSuccessNArgs("TaskSelfTest::formatSelectStatement", std::bind(&TaskSelfTest::formatSelectStatement, this)))
    {
        std::clog << "TaskSelfTest::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return false;
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
        std::clog << "TaskSelfTest::testAllInsertFailures() before successful insert *this = " << *this << "\n";
    }

    if (!insert())
    {
        std::clog << "In  TaskSelfTest::testAllInsertFailures() Expected successful insert failed\n" << errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

bool TaskSelfTest::testDiff()
{
    TaskModel other;

    other.setTaskID(primaryKey);

    if (*this == other)
    {
        return false;
    }

    other = *this;

    return *this == other;
}

bool TaskSelfTest::testAccessorFunctionsPassed()
{
    selfTestResetAllValues();

     bool allAccessorFunctionsPassed = true;
    std::vector<std::function<bool(void)>> accessTests; 
    accessTests.push_back({std::bind(&TaskSelfTest::testTaskIdAccesss, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testCreatorIDAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testAssignToIDAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testDescriptionAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testStatusAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testParentTaskIDAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testPercentageCompleteAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testCreationDateAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testDueDateAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testScheduledStartAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testActualStartDateAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testEstimatedCompletionAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testCompletionDateAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testEstimatedEffortAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testActualEffortToDateAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testPriorityGroupAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testPriorityGroupCAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testPriorityAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testPersonalAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testDependenciesAccess, this)});
    accessTests.push_back({std::bind(&TaskSelfTest::testMarkComplete, this)});

    for (auto accessTest: accessTests)
    {
        if (!accessTest())
        {
            allAccessorFunctionsPassed = false;
        }
    }

    return allAccessorFunctionsPassed;
}

bool TaskSelfTest::testTaskIdAccesss()
{
    std::size_t testPrimaryKey = 33;

    return testAccessorFunctions<std::size_t>(testPrimaryKey, &primaryKey, "Primary Key",
        std::bind(&TaskModel::setTaskID, this, std::placeholders::_1),
        std::bind(&TaskModel::getTaskID, this));
}

bool TaskSelfTest::testCreatorIDAccess()
{
    std::size_t testValue = 1;

    return testAccessorFunctions<std::size_t>(testValue, &creatorID, "Creator User ID",
        std::bind(&TaskModel::setCreatorID, this, std::placeholders::_1),
        std::bind(&TaskModel::getCreatorID, this));
}

bool TaskSelfTest::testAssignToIDAccess()
{
    std::size_t testValue = 2;

    return testAccessorFunctions<std::size_t>(testValue, &assignToID, "Assigned User ID",
        std::bind(&TaskModel::setAssignToID, this, std::placeholders::_1),
        std::bind(&TaskModel::getAssignToID, this));
}

bool TaskSelfTest::testDescriptionAccess()
{
    std::string testValue("Test task description access");
    return testAccessorFunctions<std::string>(testValue, &description, "Description",
        std::bind(&TaskModel::setDescription, this, std::placeholders::_1),
        std::bind(&TaskModel::getDescription, this));
}

bool TaskSelfTest::testStatusAccess()
{
    bool testStatusAccessPassed = true;
    TaskStatus testValue = TaskSelfTest::TaskStatus::On_Hold;
    if (verboseOutput)
    {
        std::clog << "Running self test on set and get functions for TaskSelfTest::status\n";
    }

    modified = false;
    setStatus(testValue);
    if (!testStatusVerifyValueAndGetStatus(testValue))
    {
        testStatusAccessPassed = false;
    }

    modified = false;
    testValue = TaskSelfTest::TaskStatus::Complete;
    std::string statusName = taskStatusString(testValue);
    setStatus(statusName);
    if (!testStatusVerifyValueAndGetStatus(testValue))
    {
        testStatusAccessPassed = false;
    }

    if (verboseOutput)
    {
        std::clog << "Self test on set and get functions for TaskSelfTest::status PASSED\n";
    }

    return testStatusAccessPassed;
}

bool TaskSelfTest::testStatusVerifyValueAndGetStatus(TaskStatus testValue)
{
    if (!isModified())
    {
        std::clog << "In self test for: TaskSelfTest::status set function for status FAILED to set modified\n";
        return false;
    }

    if (!status.has_value() || status.value() != testValue)
    {
        if (!status.has_value())
        {
            std::clog  << "In self test for: TaskSelfTest::status Set function for status FAILED to set member value\n";
        }
        if (status.value() != testValue)
        {
            std::clog  << "In self test for: TaskSelfTest::status expected value: " << static_cast<unsigned int>(testValue)
                        << "actual value: " << static_cast<unsigned int>(status.value()) << " FAILED to set member value\n";
        }
        return false;
    }

    std::optional<TaskSelfTest::TaskStatus> returnValue = getStatus();
    if (!returnValue.has_value() || returnValue.value() != testValue)
    {
        std::clog  << "In self test for: TaskSelfTest::status Get function for status FAILED\n";
        return false;
    }

    if (getStatusIntVal() != static_cast<unsigned int>(returnValue.value()))
    {
        std::clog  << "In self test for: TaskModel::getStatusIntVal() FAILED\n";
        return false;
    }

    return true;
}


bool TaskSelfTest::testParentTaskIDAccess()
{
    bool testParentTaskIDAccessPassed = true;
    std::size_t testValue = 37;
    if (verboseOutput)
    {
        std::clog << "Running self test on set and get functions for TaskSelfTest::parentTaskID\n";
    }

    modified = false;
    setParentTaskID(testValue);
    if (!testParentTaskIDVerifyValueAndGetParentTaskID(testValue))
    {
        testParentTaskIDAccessPassed = false;
    }

    testValue = 42;
    TaskModel_shp parent = std::make_shared<TaskModel>(TaskModel());
    parent->setTaskID(testValue);
    modified = false;
    setParentTaskID(parent);
    if (!testParentTaskIDVerifyValueAndGetParentTaskID(testValue))
    {
        testParentTaskIDAccessPassed = false;
    }

    if (verboseOutput)
    {
        std::clog << "Self test on set and get functions for TaskSelfTest::parentTaskID PASSED\n";
    }

    return testParentTaskIDAccessPassed;
}

bool TaskSelfTest::testParentTaskIDVerifyValueAndGetParentTaskID(std::size_t testValue)
{
    if (!isModified())
    {
        std::clog << "In self test for: TaskSelfTest::parentID set function for parentID FAILED to set modified\n";
        return false;
    }

    if (!parentTaskID.has_value() || parentTaskID.value() != testValue)
    {
        if (!parentTaskID.has_value())
        {
            std::clog  << "In self test for: TaskSelfTest::parentTaskID Set function for parentTaskID FAILED to set member value\n";
        }
        if (parentTaskID.value() != testValue)
        {
            std::clog  << "In self test for: TaskSelfTest::parentTaskID expected value: " << testValue <<
                "actual value: " << parentTaskID.value() << " FAILED to set member value\n";
        }
        return false;
    }

    std::optional<std::size_t> returnValue = getParentTaskID();
    if (!returnValue.has_value() || returnValue.value() != testValue)
    {
        std::clog  << "In self test for: TaskSelfTest::parentTaskID Get function for parentTaskID FAILED\n";
        return false;
    }

    return true;
}


bool TaskSelfTest::testPercentageCompleteAccess()
{
    double testValue = 86.5;
    return testAccessorFunctions<double>(testValue, &percentageComplete, "Percentage Complete",
        std::bind(&TaskModel::setPercentageComplete, this, std::placeholders::_1),
        std::bind(&TaskModel::getPercentageComplete, this));
}

bool TaskSelfTest::testCreationDateAccess()
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();
    return testTimeStampAccessorFunctions(testValue, &creationTimeStamp, "Creation TimeStamp",
        std::bind(&TaskModel::setCreationDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getCreationDate, this));
}

bool TaskSelfTest::testDueDateAccess()
{
    std::chrono::year_month_day testValue = getTodaysDate();
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &dueDate, "Due Date",
        std::bind(&TaskModel::setDueDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getDueDate, this));
}

bool TaskSelfTest::testScheduledStartAccess()
{
    std::chrono::year_month_day testValue = getTodaysDateMinus(OneWeek);
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &scheduledStart, "Scheduled Start Date",
        std::bind(&TaskModel::setScheduledStart, this, std::placeholders::_1),
        std::bind(&TaskModel::getScheduledStart, this));
}

bool TaskSelfTest::testActualStartDateAccess()
{
    std::chrono::year_month_day testValue = getTodaysDateMinus(2);
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &actualStartDate, "Actual Start Date",
        std::bind(&TaskModel::setactualStartDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getactualStartDate, this));
}

bool TaskSelfTest::testEstimatedEffortAccess()
{
    unsigned int testValue = 1024;

    return testAccessorFunctions<unsigned int>(testValue, &estimatedEffort, "Estimated Effort",
        std::bind(&TaskModel::setEstimatedEffort, this, std::placeholders::_1),
        std::bind(&TaskModel::getEstimatedEffort, this));
}

bool TaskSelfTest::testCompletionDateAccess()
{
    std::chrono::year_month_day testValue = getTodaysDate();
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &completionDate, "Completion Date",
        std::bind(&TaskModel::setCompletionDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getCompletionDate, this));
}

bool TaskSelfTest::testEstimatedCompletionAccess()
{
    std::chrono::year_month_day testValue = getTodaysDate();
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &estimatedCompletion, "Completion Date",
        std::bind(&TaskModel::setEstimatedCompletion, this, std::placeholders::_1),
        std::bind(&TaskModel::getEstimatedCompletion, this));
}

bool TaskSelfTest::testActualEffortToDateAccess()
{
    double testValue = 752.75;
    if (!testAccessorFunctions<double>(testValue, &actualEffortToDate, "Actual Effort to date",
        std::bind(&TaskModel::setActualEffortToDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getactualEffortToDate, this)))
    {
        return false;
    }

    return testAddEffort();
}

bool TaskSelfTest::testPriorityGroupAccess()
{
    unsigned int testValue = 3;

    return testAccessorFunctions<unsigned int>(testValue, &priorityGroup, "Priority Group Int Value",
        std::bind(&TaskModel::setPriorityGroup, this, std::placeholders::_1),
        std::bind(&TaskModel::getPriorityGroup, this));
}

bool TaskSelfTest::testPriorityGroupCAccess()
{
    const char testValue = 'B';
    unsigned int expectedInternalValue = testValue - 'A' + 1;
    std::string_view memberName("Priority Group Character");

    if (verboseOutput)
    {
        std::clog << "Running self test on set and get functions for " << modelName << "::" << memberName << "\n";
    }

    modified = false;

    setPriorityGroupC(testValue);
    if (!isModified())
    {
        std::clog << "In self test for: " << modelName << " set function for " << memberName << " FAILED to set modified\n";
        return false;
    }

    if (priorityGroup != expectedInternalValue)
    {
        std::clog  << "In self test for: " << modelName << "Set function for " << memberName << " FAILED to set member value\n";
        std::clog << "\tExpected Value: " << expectedInternalValue << "Actual Value: " << priorityGroup << "\n";
        return false;
    }

    if (getPriorityGroup() != expectedInternalValue)
    {
        std::clog  << "In self test for: " << modelName << " Get function for " << memberName << " FAILED\n";
        std::clog << "\tExpected Value: " << expectedInternalValue << "Actual Value: " << getPriorityGroup() << "\n";
        return false;
    }

    if (verboseOutput)
    {
        std::clog << "Self test on set and get functions for " << modelName << "::" << memberName << " PASSED\n";
    }

    return true;
}

bool TaskSelfTest::testPriorityAccess()
{
    unsigned int testValue = 5;

    return testAccessorFunctions<unsigned int>(testValue, &priority, "Priority",
        std::bind(&TaskModel::setPriority, this, std::placeholders::_1),
        std::bind(&TaskModel::getPriority, this));
}

bool TaskSelfTest::testPersonalAccess()
{
    bool testValue = true;

    return testAccessorFunctions<bool>(testValue, &personal, "Personal",
        std::bind(&TaskModel::setPersonal, this, std::placeholders::_1),
        std::bind(&TaskSelfTest::isPersonal, this));
}

bool TaskSelfTest::testDependenciesAccess()
{
    modified = false;

    if (verboseOutput)
    {
        std::clog << "Running self test on add and get functions for " << modelName << "::dependencies\n";
    }

    std::vector<std::size_t> testDependencies;
    testDependencies.push_back(1);
    testDependencies.push_back(3);
    testDependencies.push_back(5);

    for (auto dependency: testDependencies)
    {
        addDependency(dependency);
        if (!isModified())
        {
            std::clog << "In self test for: " << modelName << "::addDependency()" << " FAILED to set modified\n";
            return false;
        }
    }

    if (dependencies != testDependencies)
    {
        std::clog << "Self Test for " << modelName << "::addDependency()" << " FAILED to set values\n";
        return false;
    }

    if (getDependencies() != testDependencies)
    {
        std::clog << "Self Test for " << modelName << "::getDependencies()" << " FAILED to get values\n";
    }

    if (verboseOutput)
    {
        std::clog << "Self test on set and get functions for " << modelName << "::dependencoies PASSED\n";
    }

    return true;
}

/*
 * This test assumes that getStatus() and getCompletionDate() are tested elsewhere.
 */
bool TaskSelfTest::testMarkComplete()
{
    if (verboseOutput)
    {
        std::clog << "Running self test for TaskSelfTest::markComplete\n";
    }

    modified = false;
    status.reset();
    completionDate.reset();

    markComplete();

    if (!status.has_value() || status.value() != TaskStatus::Complete)
    {
        if (!status.has_value())
        {
            std::clog  << "In self test for: TaskSelfTest::markComplete FAILED to set status member value\n";
        }
        if (status.value() != TaskStatus::Complete)
        {
            std::clog  << "In self test for: TaskSelfTest::markComplete expected value: " << static_cast<unsigned int>(TaskStatus::Complete)
                << "actual value: " << static_cast<unsigned int>(status.value()) << " FAILED to set status member value\n";
        }
        return false;
    }

    if (!completionDate.has_value())
    {
        std::clog  << "In self test for: TaskSelfTest::markComplete FAILED to set completionDate member value\n";
        return false;
    }

    if (verboseOutput)
    {
        std::clog << "Self test for TaskSelfTest::markComplete PASSED\n";
    }

    return true;
}

bool TaskSelfTest::testAddEffort()
{
    double addedHours = 7.25;
    double expectedHours = actualEffortToDate + addedHours;

    modified = false;
    addEffortHours(addedHours);

    if (!isModified())
    {
        std::clog << "TaskSelfTest::addEffortHours() failed to set modified! test FAILED;\n";
        return false;
    }

    if (actualEffortToDate != expectedHours)
    {
        std::clog << "TaskSelfTest::actualEffortToDate does not show additional hours! test FAILED;\n";
        return false;

    }

    if (getactualEffortToDate() != expectedHours)
    {
        std::clog << "TaskModel::getactualEffortToDate() does not show additional hours! test FAILED;\n";
        return false;
    }

    return true;
}

