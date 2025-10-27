// Project Header Files
#include "CommandLineParser.h"
#include "NoteModel.h"
#include "TaskModel.h"
#include "TestTaskDBInterface.h"
#include "TestUserDBInterface.h"
#include "TestGoalModel.h"
#include "UserGoalModel.h"
#include "UserModel.h"
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
    std::vector<AnyModel_shp> modelsToUnitTest;
    modelsToUnitTest.push_back(std::make_shared<UserModel>());
    UserModel alternateConstructor("LastName", "FirstName", "I", "FirstName.LastName@LastName.com");
    alternateConstructor.autoGenerateLoginAndPassword();
    modelsToUnitTest.push_back(std::make_shared<UserModel>(alternateConstructor));
    modelsToUnitTest.push_back(std::make_shared<TaskModel>());
    modelsToUnitTest.push_back(std::make_shared<TaskModel>(1));
    modelsToUnitTest.push_back(std::make_shared<TaskModel>(1, "Test alternate constructor for TaskModel"));
    modelsToUnitTest.push_back(std::make_shared<NoteModel>());
    modelsToUnitTest.push_back(std::make_shared<UserGoalModel>());

    for (auto modelUnderTest: modelsToUnitTest)
    {
        if (!modelUnderTest->runSelfTest())
        {
            std::clog << std::format("*** {} FAILED Self Test ***\n", modelUnderTest->getModelName());
            if (programOptions.quitFirstFail)
            {
                return false;
            }
        }
        else
        {
            if (programOptions.verboseOutput)
            {
                std::clog << std::format("{} PASSED Self Test\n", modelUnderTest->getModelName());
            }
        }
    }

    modelsToUnitTest.clear();
    
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

