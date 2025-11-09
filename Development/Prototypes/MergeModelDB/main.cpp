// Project Header Files
#include "CommandLineParser.h"
#include "NoteModel.h"
#include "NoteSelfTest.h"
#include "TaskModel.h"
#include "TaskSelfTest.h"
#include "TestTaskDBInterface.h"
#include "TestUserDBInterface.h"
#include "TestGoalModel.h"
#include "TestStatus.h"
#include "UserGoalModel.h"
#include "UserGoalSelfTest.h"
#include "UserModel.h"
#include "UserSelfTest.h"
#include "UtilityTimer.h"

// Standard C++ Header Files
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>

/*
 * All of the DBInterface classes need access to the programOptions global variable for the
 * MySQL user name and password, as well as the database name and other connection details.
 */
ProgramOptions programOptions;

template <class A>
static bool runUnitTest(ModelSelfTest<A>* unitTest)
{
    if (!unitTest->runSelfTest())
    {
        std::string failMessage = std::format("*** {} FAILED Self Test ***", unitTest->getModelName());
        std::clog << failMessage << "\n";
        if (programOptions.quitFirstFail)
        {
            std::runtime_error firstFail(failMessage);
            throw firstFail;
        }
        return false;
    }
    else
    {
        if (programOptions.verboseOutput)
        {
            std::clog << std::format("{} PASSED Self Test\n", unitTest->getModelName());
        }
    }

    return true;
}

static bool runAllUnitTests()
{
    bool allUnintTestsPassed = true;

    UserSelfTest userTest;
    if (!runUnitTest(&userTest))
    {
        allUnintTestsPassed = false;
    }
    
    TaskSelfTest taskTest;
    if (!runUnitTest(&taskTest))
    {
        allUnintTestsPassed = false;
    }

    UserGoalSelfTest goalTest;
    if (!runUnitTest(&goalTest))
    {
        allUnintTestsPassed = false;
    }

    NoteSelfTest noteTest;
    if (!runUnitTest(&noteTest))
    {
        allUnintTestsPassed = false;
    }

    return allUnintTestsPassed;
}


int main(int argc, char* argv[])
{
    try {
		if (const auto progOptions = parseCommandLine(argc, argv); progOptions.has_value())
		{
			programOptions = *progOptions;
            UtilityTimer stopWatch;

            if (!runAllUnitTests())
            {
                if (programOptions.quitFirstFail)
                {
                    return EXIT_FAILURE;
                }
            }
#if 1
            TestUserDBInterface userTests(programOptions.userTestDataFile);
            if (userTests.runAllTests() == TestStatus::TestPassed)
            {
                TestTaskDBInterface tasktests(programOptions.taskTestDataFile);
                if (tasktests.runAllTests() != TestStatus::TestPassed)
                {
                    return EXIT_FAILURE;
                }
                else
                {
                    TestGoalModel goalTests;
                    if (goalTests.runAllTests() != TestStatus::TestPassed)
                    {
                        return EXIT_FAILURE;
                    }
                }
            }
            else
            {
                return EXIT_FAILURE;
            }
            std::clog << "All tests Passed\n";
			if (programOptions.enableExecutionTime)
			{
                stopWatch.stopTimerAndReport("Testing of Insertion and retrieval of users and tasks in MySQL database\n");
			}
#endif
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

