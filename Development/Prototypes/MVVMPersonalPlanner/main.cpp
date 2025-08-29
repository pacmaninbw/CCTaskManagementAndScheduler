#include "CommandLineParser.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include "TestTaskDBInterface.h"
#include "TestUserDBInterface.h"
#include "UtilityTimer.h"

/*
 * All of the DBInterface classes need access to the programOptions global variable for the
 * MySQL user name and password, as well as the database name and other connection details.
 */
ProgramOptions programOptions;

int main(int argc, char* argv[])
{
    try {
		if (const auto progOptions = parseCommandLine(argc, argv); progOptions.has_value())
		{
			programOptions = *progOptions;
            UtilityTimer stopWatch;
            TestUserDBInterface userTests(programOptions.userTestDataFile);

            if (userTests.runAllTests() == TestDBInterfaceCore::TestStatus::TestPassed)
            {
                TestTaskDBInterface tasktests(programOptions.taskTestDataFile);
                if (tasktests.runAllTests() != TestDBInterfaceCore::TestStatus::TestPassed)
                {
                    return EXIT_FAILURE;
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

