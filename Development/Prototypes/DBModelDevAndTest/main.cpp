// Project Header Files
#include "CommandLineParser.h"
#include "commonTestValues.h"
#include "GoalQueryProcessor.h"
#include "OrganizationModel.h"
#include "OrganizationSelfTest.h"
#include "NoteQueryProcessor.h"
#include "NoteModel.h"
#include "NoteSelfTest.h"
#include "ScheduleItemModel.h"
#include "ScheduleItemQueryProcessor.h"
#include "ScheduleItemSelfTest.h"
#include "TaskModel.h"
#include "TaskQueryProcessor.h"
#include "TaskSelfTest.h"
#include "TestDBConnection.h"
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
#include "UserQueryProcessor.h"
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

    OrganizationSelfTest organizationTest;
    if (runUnitTest(&organizationTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

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

template <class A, class B>
static TestStatus runListUnitTest(QueryProcessor<A, B>* unitTest)
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

    UserQueryProcessor UserQueryProcessorTest;
    if (runListUnitTest(&UserQueryProcessorTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    TaskQueryProcessor TaskQueryProcessorTest;
    if (runListUnitTest(&TaskQueryProcessorTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    GoalQueryProcessor goalListTest;
    if (runListUnitTest(&goalListTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    NoteQueryProcessor NoteQueryProcessorTest;
    if (runListUnitTest(&NoteQueryProcessorTest) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }

    ScheduleItemQueryProcessor ScheduleItemQueryProcessor(1);
    if (runListUnitTest(&ScheduleItemQueryProcessor) == TESTFAILED)
    {
        allUnintTestsPassed = TESTFAILED;
    }
    
    return allUnintTestsPassed;
}

static TestStatus runAllIntegrationTests() noexcept
{
    separateTestCaseOutput();
    TestStatus allInitgrationTests = TESTPASSED;
    std::string_view modelBeingTested;

    try {
        TestUserDBInterface userTests(programOptions.userTestDataFile);
        modelBeingTested = userTests.getModelName();
        if (userTests.runAllTests() != TESTPASSED)
        {
            allInitgrationTests = TESTFAILED;

            if (programOptions.quitFirstFail)
            {
                return TESTFAILED;
            }
        }

        separateTestCaseOutput();
        TestTaskDBInterface tasktests(programOptions.taskTestDataFile);
        modelBeingTested = tasktests.getModelName();
        if (tasktests.runAllTests() != TESTPASSED)
        {
            allInitgrationTests = TESTFAILED;

            if (programOptions.quitFirstFail)
            {
                return TESTFAILED;
            }
        }

        separateTestCaseOutput();
        TestGoalModel goalTests;
        modelBeingTested = goalTests.getModelName();
        if (goalTests.runAllTests() != TESTPASSED)
        {
            allInitgrationTests = TESTFAILED;

            if (programOptions.quitFirstFail)
            {
                return TESTFAILED;
            }
        }

        separateTestCaseOutput();
        TestNoteModel noteTests;
        modelBeingTested = noteTests.getModelName();
        if (noteTests.runAllTests() != TESTPASSED)
        {
            allInitgrationTests = TESTFAILED;

            if (programOptions.quitFirstFail)
            {
                return TESTFAILED;
            }
        }

        separateTestCaseOutput();
        TestScheduleItemModel scheduleItemTests;
        modelBeingTested = scheduleItemTests.getModelName();
        if (scheduleItemTests.runAllTests() != TESTPASSED)
        {
            allInitgrationTests = TESTFAILED;

            if (programOptions.quitFirstFail)
            {
                return TESTFAILED;
            }
        }
    }
    catch (std::exception& e)
    {
        allInitgrationTests = TESTFAILED;
        std::cerr << std::format("Itegration testing for {} FAILED with excpetion: {}", modelBeingTested, e.what()) << std::endl;
    }

    return allInitgrationTests;
}

int main(int argc, char* argv[])
{
    try {
		if (const auto progOptions = parseCommandLine(argc, argv); progOptions.has_value())
		{
			programOptions = *progOptions;
            UtilityTimer stopWatch;

            common::initCommonTestValues("2024-05-14 08:31:53");

            TestDBConnection dbConnectionTest;
            if (dbConnectionTest.runTestConnect() == TESTFAILED)
            {
                return EXIT_FAILURE;
            }

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
                stopWatch.stopTimerAndReport("Unit and Integration testing of all models MySQL database\n");
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
        std::cerr << "Error: Test Program terminated abnormally!" << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

