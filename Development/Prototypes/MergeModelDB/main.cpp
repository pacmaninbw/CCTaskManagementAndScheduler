// Project Header Files
#include "CommandLineParser.h"
#include "NoteModel.h"
#include "NoteSelfTest.h"
#include "TaskModel.h"
#include "TaskSelfTest.h"
#include "TestTaskDBInterface.h"
#include "TestUserDBInterface.h"
#include "TestGoalModel.h"
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

static bool runUnitTests()
{
    UserSelfTest userTest;
    
    if (!userTest.runSelfTest())
    {
        std::clog << std::format("*** {} FAILED Self Test ***\n", userTest.getModelName());
        if (programOptions.quitFirstFail)
        {
            return false;
        }
    }
    else
    {
        if (programOptions.verboseOutput)
        {
            std::clog << std::format("{} PASSED Self Test\n", userTest.getModelName());
        }
    }

    TaskSelfTest taskTest;
    
    if (!taskTest.runSelfTest())
    {
        std::clog << std::format("*** {} FAILED Self Test ***\n", taskTest.getModelName());
        if (programOptions.quitFirstFail)
        {
            return false;
        }
    }
    else
    {
        if (programOptions.verboseOutput)
        {
            std::clog << std::format("{} PASSED Self Test\n", taskTest.getModelName());
        }
    }

    UserGoalSelfTest goalTest;
    
    if (!goalTest.runSelfTest())
    {
        std::clog << std::format("*** {} FAILED Self Test ***\n", goalTest.getModelName());
        if (programOptions.quitFirstFail)
        {
            return false;
        }
    }
    else
    {
        if (programOptions.verboseOutput)
        {
            std::clog << std::format("{} PASSED Self Test\n", goalTest.getModelName());
        }
    }

    NoteSelfTest noteTest;
    
    if (!noteTest.runSelfTest())
    {
        std::clog << std::format("*** {} FAILED Self Test ***\n", noteTest.getModelName());
        if (programOptions.quitFirstFail)
        {
            return false;
        }
    }
    else
    {
        if (programOptions.verboseOutput)
        {
            std::clog << std::format("{} PASSED Self Test\n", noteTest.getModelName());
        }
    }
    return true;
}


int main(int argc, char* argv[])
{
    try {
		if (const auto progOptions = parseCommandLine(argc, argv); progOptions.has_value())
		{
			programOptions = *progOptions;
            UtilityTimer stopWatch;

            if (!runUnitTests())
            {
                if (programOptions.quitFirstFail)
                {
                    return EXIT_FAILURE;
                }
            }
#if 0
            TestUserDBInterface userTests(programOptions.userTestDataFile);
            if (userTests.runAllTests() == TestDBInterfaceCore::TestStatus::TestPassed)
            {
                TestTaskDBInterface tasktests(programOptions.taskTestDataFile);
                if (tasktests.runAllTests() != TestDBInterfaceCore::TestStatus::TestPassed)
                {
                    return EXIT_FAILURE;
                }
                else
                {
                    TestGoalModel goalTests;
                    if (goalTests.runAllTests() != TestDBInterfaceCore::TestStatus::TestPassed)
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

