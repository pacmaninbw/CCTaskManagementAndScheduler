/*
 * Perform unit tests and integration tests on the database models and the
 * database queries.
 */

// Project Header Files
#include "CommandLineParser.h"
#include "commonTestValues.h"
#include "GoalQueryProcessor.h"
#include "NoteQueryProcessor.h"
#include "NoteModel.h"
#include "OrganizationModel.h"
#include "OrganizationQueryProcessor.h"
#include "ScheduleItemModel.h"
#include "ScheduleItemQueryProcessor.h"
#include "TaskModel.h"
#include "TaskQueryProcessor.h"
#include "TestDBConnection.h"
#include "TestGoalModel.h"
#include "TestNoteModel.h"
#include "TestOrganizationModel.h"
#include "TestScheduleItemModel.h"
#include "TestStatus.h"
#include "TestTaskDBInterface.h"
#include "TestUserDBInterface.h"
#include "UserGoalModel.h"
#include "UserModel.h"
#include "UserQueryProcessor.h"
#include "UtilityTimer.h"

// Standard C++ Header Files
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

/*
 * All of the DBInterface classes need access to the programOptions global variable for the
 * MySQL user name and password, as well as the database name and other connection details.
 */
ProgramOptions programOptions;

static void reportUnitTestStatusExitIfQuitFirstFail(TestStatus result, std::string itemUnderTest)
{
    if (result != TESTPASSED)
    {
        std::string failMessage = std::format("*** {} FAILED Self Test ***", itemUnderTest);
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
            std::cout << std::format("{} PASSED Self Test\n", itemUnderTest);
        }
    }
}

static TestStatus runAllUnitTests()
{
    TestStatus allUnintTestsPassed = TESTPASSED;

    std::vector<std::shared_ptr<ModelDBInterface>> unitTests;
    unitTests.push_back(std::make_shared<OrganizationModel>());
    unitTests.push_back(std::make_shared<UserModel>());
    unitTests.push_back(std::make_shared<TaskModel>());
    unitTests.push_back(std::make_shared<UserGoalModel>());
    unitTests.push_back(std::make_shared<NoteModel>());
    unitTests.push_back(std::make_shared<ScheduleItemModel>());

    for (const auto& unitTest: unitTests)
    {
        common::separateTestCaseOutput("-");

        if (!unitTest)
        {
            std::cerr << "NULL POINTER in runAllUnitTests" << std::endl;
            allUnintTestsPassed = TESTFAILED;
            continue;
        }

        TestStatus result = unitTest->runSelfTest();
        reportUnitTestStatusExitIfQuitFirstFail(result,  unitTest->getModelName());

        if (result != TESTPASSED)
        {
            allUnintTestsPassed = TESTFAILED;
        }
    }

    return allUnintTestsPassed;
}

/*
 * Run the Query generator self tests to internally check the queries. This will
 * not execute actual queries bu will go through all the code in each query
 * prior to database execution.
 */
static TestStatus runAllQueryUnitTests()
{
    TestStatus allUnintTestsPassed = TESTPASSED;

    using VQP = std::variant<
        OrganizationQueryProcessor,
        UserQueryProcessor,
        TaskQueryProcessor,
        GoalQueryProcessor,
        NoteQueryProcessor,
        ScheduleItemQueryProcessor>;

    std::vector<std::shared_ptr<VQP>> QueryProcessorTests;
    QueryProcessorTests.push_back(std::make_shared<VQP>(OrganizationQueryProcessor{}));
    QueryProcessorTests.push_back(std::make_shared<VQP>(UserQueryProcessor{}));
    QueryProcessorTests.push_back(std::make_shared<VQP>(TaskQueryProcessor{}));
    QueryProcessorTests.push_back(std::make_shared<VQP>(GoalQueryProcessor{}));
    QueryProcessorTests.push_back(std::make_shared<VQP>(NoteQueryProcessor{}));
    QueryProcessorTests.push_back(std::make_shared<VQP>(ScheduleItemQueryProcessor{1}));

    for (const auto& queryTest: QueryProcessorTests)
    {
        common::separateTestCaseOutput("-");

        if (!queryTest)
        {
            std::cerr << "NULL POINTER in runListUnitTest" << std::endl;
            allUnintTestsPassed = TESTFAILED;
            continue;
        }
        TestStatus thisTestStatus = TESTPASSED;
        std::string listTypeName;
        std::visit([&](auto &&vqp) { listTypeName = vqp.getListTypeName(); }, *queryTest);
        std::visit([&](auto &&vqp ) { thisTestStatus = vqp.runSelfTest(); }, *queryTest);
        if (thisTestStatus != TESTPASSED)
        {
            allUnintTestsPassed = TESTFAILED;
        }
        reportUnitTestStatusExitIfQuitFirstFail(thisTestStatus,  listTypeName);
    }

    return allUnintTestsPassed;
}

/*
 * The integration tests perform actual database insertions and queries.
 */
static TestStatus runAllIntegrationTests() noexcept
{
    TestStatus allInitgrationTests = TESTPASSED;
    std::string_view modelBeingTested;

    std::vector<std::shared_ptr<TestDBInterfaceCore>> integrationTests;
    integrationTests.push_back(std::make_shared<TestUserDBInterface>(programOptions.userTestDataFile));
    integrationTests.push_back(std::make_shared<TestOrganizationModel>());
    integrationTests.push_back(std::make_shared<TestTaskDBInterface>(programOptions.taskTestDataFile));
    integrationTests.push_back(std::make_shared<TestGoalModel>());
    integrationTests.push_back(std::make_shared<TestNoteModel>());
    integrationTests.push_back(std::make_shared<TestScheduleItemModel>());

    try {
        for (const auto& integrationTest: integrationTests)
        {
        common::separateTestCaseOutput("-");

            modelBeingTested = integrationTest->getModelName();
            
            if (integrationTest->runAllTests() != TESTPASSED)
            {
                allInitgrationTests = TESTFAILED;

                if (programOptions.quitFirstFail)
                {
                    return TESTFAILED;
                }
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

/*
 * First check to see that the database connection can be established, if not 
 * there will be only errors and no tests will pass.
 * 
 * Run the model unit tests first, since the query unit tests depend on the models
 * working as expected. Finally run the integration tests after all the unit tests
 * have been performed.
 */
static bool runAllTests()
{
    bool allTestsPassed = true;
    std::vector<std::function<TestStatus()>> tests = {runAllUnitTests, runAllQueryUnitTests, runAllIntegrationTests};

    TestDBConnection dbConnectionTest;
    if (dbConnectionTest.runTestConnect() == TESTFAILED)
    {
        // Prevent cascading failures
        return false;
    }

    for (const auto& test: tests)
    {
        if (test() == TESTFAILED)
        {
            allTestsPassed = false;
            if (programOptions.quitFirstFail)
            {
                return allTestsPassed;
            }
        }
    }

    return allTestsPassed;
}

int main(int argc, char* argv[])
{
    bool allTestsPassed = true;

    try {
		if (const auto progOptions = parseCommandLine(argc, argv); progOptions.has_value())
		{
			programOptions = *progOptions;
            UtilityTimer stopWatch;

            // User a common date to reduce the differences in the regression test
            // output. The date chosen is the date development started.
            common::initCommonTestValues("2024-05-14 08:31:53");

            allTestsPassed = runAllTests();
	
            if (programOptions.enableExecutionTime)
			{
                stopWatch.stopTimerAndReport("Unit and Integration testing of all models MySQL database\n");
			}

            if (!allTestsPassed)
            {
                return EXIT_FAILURE;
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

    std::cout << "All tests Passed\n";
     
    return EXIT_SUCCESS;
}

