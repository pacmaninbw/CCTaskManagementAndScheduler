// Project Header Files
#include "CommandLineParser.h"
#include "NoteList.h"
#include "NoteModel.h"
#include "NoteSelfTest.h"
#include "ScheduleItemModel.h"
#include "ScheduleItemSelfTest.h"
#include "ScheduleItemList.h"
#include "TaskModel.h"
#include "TaskSelfTest.h"
#include "TestTaskDBInterface.h"
#include "TestUserDBInterface.h"
#include "TestGoalModel.h"
#include "TestNoteModel.h"
#include "TestScheduleItemModel.h"
#include "TestStatus.h"
#include "UserGoalModel.h"
#include "UserGoalSelfTest.h"
#include "UserModel.h"
#include "UserSelfTest.h"
#include "TaskList.h"
#include "UserList.h"
#include "UserGoalList.h"
#include "UtilityTimer.h"

// Standard C++ Header Files
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

/*
 * All of the DBInterface classes need access to the programOptions global variable for the
 * MySQL user name and password, as well as the database name and other connection details.
 */
ProgramOptions programOptions;

static constexpr std::size_t lineWidth = 80;

static void printDashes()
{
    for (std::size_t i = 0; i <= lineWidth; ++i)
    {
        std::cout << '-';
    }

}
static void separateTestCaseOutput()
{
    printDashes();
    std::cout << "\n\n";
    printDashes();
    std::cout << std::endl;     // Flush the output
}

template <class A>
static TestStatus runUnitTest(ModelSelfTest<A>* unitTest)
{
    separateTestCaseOutput();

    TestStatus thisTestStatus = unitTest->runSelfTest();
    if (thisTestStatus != TESTPASSED)
    {
        std::string failMessage = std::format("*** {} FAILED Self Test ***", unitTest->getModelName());
        std::cerr << failMessage << "\n";
        if (programOptions.quitFirstFail)
        {
            std::runtime_error firstFail(failMessage);
            throw firstFail;
        }
    }
    else
    {
        if (programOptions.verboseOutput)
        {
            std::cout << std::format("{} PASSED Self Test\n", unitTest->getModelName());
        }
    }

    return thisTestStatus;
}

static TestStatus runAllUnitTests()
{
    TestStatus allUnintTestsPassed = TESTPASSED;

    UserSelfTest userTest;
    if (runUnitTest(&userTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }
    
    TaskSelfTest taskTest;
    if (runUnitTest(&taskTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    UserGoalSelfTest goalTest;
    if (runUnitTest(&goalTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    NoteSelfTest noteTest;
    if (runUnitTest(&noteTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    ScheduleItemSelfTest scheduleItemTest;
    if (runUnitTest(&scheduleItemTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    return allUnintTestsPassed;
}

template <class A>
static TestStatus runListUnitTest(ListDBInterface<A>* unitTest)
{
    separateTestCaseOutput();

    TestStatus thisTestStatus = unitTest->runSelfTest();
    if (thisTestStatus != TESTPASSED)
    {
        std::string failMessage = std::format("*** {} FAILED Self Test ***", unitTest->getListTypeName());
        std::cerr << failMessage << "\n";
        if (programOptions.quitFirstFail)
        {
            std::runtime_error firstFail(failMessage);
            throw firstFail;
        }
    }
    else
    {
        if (programOptions.verboseOutput)
        {
            std::cout << std::format("{} PASSED Self Test\n", unitTest->getListTypeName());
        }
    }

    return thisTestStatus;
}

static TestStatus runAllListUnitTests()
{
    TestStatus allUnintTestsPassed = TESTPASSED;

    UserList userListTest;
    if (runListUnitTest(&userListTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    TaskList taskListTest;
    if (runListUnitTest(&taskListTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    UserGoalList goalListTest;
    if (runListUnitTest(&goalListTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }
    
    NoteList noteListTest;
    if (runListUnitTest(&noteListTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    ScheduleItemList scheduleItemList(1);
    if (runListUnitTest(&scheduleItemList) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    
    return allUnintTestsPassed;
}

static TestStatus runAllIntegrationTests()
{
    separateTestCaseOutput();
    TestUserDBInterface userTests(programOptions.userTestDataFile);
    if (userTests.runAllTests() == TESTFAILED)
    {
        return TESTFAILED;
    }

    separateTestCaseOutput();
    TestTaskDBInterface tasktests(programOptions.taskTestDataFile);
    if (tasktests.runAllTests() == TESTFAILED)
    {
        return TESTFAILED;
    }

    separateTestCaseOutput();
    TestGoalModel goalTests;
    if (goalTests.runAllTests() == TESTFAILED)
    {
        return TESTFAILED;
    }

    separateTestCaseOutput();
    TestNoteModel noteTests;
    if (noteTests.runAllTests() == TESTFAILED)
    {
        return TESTFAILED;
    }

    separateTestCaseOutput();
    TestScheduleItemModel scheduleItemTests;
    if (scheduleItemTests.runAllTests() == TESTFAILED)
    {
        return TESTFAILED;
    }

    return TESTPASSED;
}

int main(int argc, char* argv[])
{
    try {
		if (const auto progOptions = parseCommandLine(argc, argv); progOptions.has_value())
		{
			programOptions = *progOptions;
            UtilityTimer stopWatch;

            if (runAllUnitTests() == TESTFAILED)
            {
                if (programOptions.quitFirstFail)
                {
                    return EXIT_FAILURE;
                }
            }

            if (runAllListUnitTests() == TESTFAILED)
            {
                if (programOptions.quitFirstFail)
                {
                    return EXIT_FAILURE;
                }
            }

            if (runAllIntegrationTests() == TESTFAILED)
            {
                return EXIT_FAILURE;
            }

            std::cout << "All tests Passed\n";
			if (programOptions.enableExecutionTime)
			{
                stopWatch.stopTimerAndReport("Testing of Insertion and retrieval of users and tasks in MySQL database\n");
			}
        }
        else
		{
			if (progOptions.error() != CommandLineStatus::HelpRequested)
			{
				return EXIT_FAILURE;
			}
		}
    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

