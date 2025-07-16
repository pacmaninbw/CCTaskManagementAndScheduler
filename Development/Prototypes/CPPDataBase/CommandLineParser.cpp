#include <algorithm>
#include <boost/program_options.hpp>
#include "CommandLineParser.h"
#include <expected>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

static std::string simplifyName(char *path)
{
	return std::filesystem::path{path ? path : "PhotoResizeTool"}.filename().string();
}

namespace po = boost::program_options;

static const char optionStarter = '-';

enum class ProgOptStatus
{
	NoErrors,
	MissingArgument
};

static po::options_description addOptions()
{
	po::options_description options("Options and arguments");
	options.add_options()
		("help,h", "Show this help message")
		("mysql-user,u", po::value<std::string>()->required(), "Existing MySQL user with sufficient privilege to insert and query")
		("mysql-password,p", po::value<std::string>()->required(), "password for the MySQL user")
		("mysql-URL", po::value<std::string>()->default_value("127.0.0.1"), "IP address for the MySQL server")
		("mysql-port", po::value<unsigned int>(), "The port value for the MySQL server. A default of 3306 is provided.")
		("mysql-dbname", po::value<std::string>()->default_value("PlannerTaskScheduleDB"), "The name of the database that contains the tables")
		("user-data-file", po::value<std::string>()->default_value("testData/userData.txt"), "File path including file name to user test data")
		("task-data-file", po::value<std::string>()->default_value("testData/planData.txt"), "File path including file name to task test data")
		("time-tests", "Time the execution of the tests")
		("verbose", "Output additional information for testing and debugging.")
	;

	return options;
}

/*
 * hasArgument is a workaround for the fact that the boost::program_options
 * doesn't or can't check to see if the string following an option is another
 * option. 
 */
static auto hasArgument(po::variables_map& inputOptions, const std::string& option) -> 
	std::expected<std::string, ProgOptStatus>
{
	std::string argument("");
	if (!inputOptions.count(option))
	{
		return argument;	// No argument to check
	}

	argument = inputOptions[option].as<std::string>();
	po::options_description options = addOptions();

	if (argument[0] == optionStarter)
	{
		/*
		 * Remove any preceeding optionStarter, boost::program_options does not store
		 * the option start.
		 */
		auto startOptionText = std::find_if_not(argument.begin(), argument.end(),
			[](char c) { return c == optionStarter; });
		if (startOptionText != argument.end())
		{
			if (options.find_nothrow(std::string(startOptionText, argument.end()), true))
			{
				std::cerr << "The option \'--" << option << "\' is missing the required argument!\n";
				return std::unexpected(ProgOptStatus::MissingArgument);
			}
		}
	}

	return argument;
}

struct ProgOptionValuePair
{
	std::string option;
	std::string *value;
};

static auto checkOptionsWithStringArguments(po::variables_map& inputOptions) -> 
	std::expected<ProgramOptions, ProgOptStatus>
{
	ProgramOptions progOptions;
	std::vector<ProgOptionValuePair> optionsAndValuesForArgCheck = 
	{
		{"mysql-user", &progOptions.mySqlUser},
		{"mysql-password", &progOptions.mySqlPassword},
		{"mysql-URL", &progOptions.mySqlUrl},
		{"mysql-dbname", &progOptions.mySqlDBName},
		{"user-data-file", &progOptions.userTestDataFile},
		{"task-data-file", &progOptions.taskTestDataFile}
	};
	ProgOptStatus hasArguments = ProgOptStatus::NoErrors;
	
	// Process as many options as possible.
	for (auto optionToCheck: optionsAndValuesForArgCheck)
	{
		const auto argCheck = hasArgument(inputOptions, optionToCheck.option);
		if (argCheck.has_value())
		{
			*optionToCheck.value = *argCheck;
		}
		else
		{
			hasArguments = (hasArguments == ProgOptStatus::NoErrors)?
				argCheck.error() : hasArguments;
		}
	}

	if (hasArguments != ProgOptStatus::NoErrors)
	{
		return std::unexpected(hasArguments);
	}

	return progOptions;
}

static auto processProgramOptions(po::variables_map& inputOptions,
	const std::string& progName) -> std::expected<ProgramOptions, ProgOptStatus>
{
	ProgramOptions programOptions;

	programOptions.progName = progName;

	const auto argCheck = checkOptionsWithStringArguments(inputOptions);
	if (argCheck.has_value())
	{
		programOptions = *argCheck;
	}
	else
	{
		return std::unexpected(argCheck.error());
	}

	programOptions.mySqlPort = (inputOptions.count("mysql-port")) ?
			inputOptions["scale-factor"].as<unsigned int>() : 3306;

	if (inputOptions.count("time-tests")) {
		programOptions.enableExecutionTime = true;
	}

	if (inputOptions.count("verbose")) {
		programOptions.verboseOutput = true;
	}

	return programOptions;
}

static const int MinArgCount = 3;

static CommandLineStatus usage(const std::string& progName,
	const po::options_description& options,
	const std::string errorMessage
	)
{
	std::string usageStr =
	" :\n\tTest storing and retrieving user profiles and tasks in a MySQL database.\n"
	"\tThe connection data must be listed with the appropriate flags. Default MySQL\n"
	"\tIP address is 127.0.0.1 and a default MySQL port 3306 are provided. Default\n"
	"\tvalues for the user data file name and task data file name are also provided\n"
;
	if (errorMessage.length())
	{
		std::cerr << errorMessage << "\n";
		std::cerr << progName << usageStr << "\n" << options << "\n";
	}
	else
	{
		std::cerr << progName << ":\n" << options << "\n";
	}
	return CommandLineStatus::HasErrors;
}

static CommandLineStatus help(const std::string& progName,
	const po::options_description& options)
{
	std::cerr << progName << options << "\n";
	return CommandLineStatus::HelpRequested;
}

auto parseCommandLine(int argc, char* argv[]) -> 
	std::expected<ProgramOptions, CommandLineStatus>
{
	ProgramOptions programOptions;
	std::string progName = simplifyName(argv[0]);
	po::options_description options = addOptions();

	if (argc < MinArgCount)
	{
		return std::unexpected(usage(progName, options,
			"Missing MySql User Name and Password"));
	}

	po::variables_map optionMemory;        
	try
	{
		po::store(po::parse_command_line(argc, argv, options), optionMemory);
		po::notify(optionMemory);    
	}
	/*
	 * Handle any exceptions thrown by boost::program_options.
	 */
	catch(const std::exception& e)
	{
		return std::unexpected(usage(progName, options, e.what()));
	}
	
	if (optionMemory.count("help")) {
		return std::unexpected(help(progName, options));
	}

	if (const auto progOptions = processProgramOptions(optionMemory, progName); progOptions.has_value())
	{
		programOptions = *progOptions;
	}
	else
	{
		return std::unexpected(usage(progName, options, ""));
	}

	return programOptions;
}


