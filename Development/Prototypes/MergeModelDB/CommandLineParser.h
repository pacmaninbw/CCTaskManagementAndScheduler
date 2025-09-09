#ifndef COMMAND_LINE_PARSER_H_
#define COMMAND_LINE_PARSER_H_

#include <expected>
#include <string>

struct ProgramOptions
{
    std::string progName;
    std::string mySqlUser;
    std::string mySqlPassword;
    std::string mySqlUrl;
    unsigned int mySqlPort;
    std::string mySqlDBName;
    std::string userTestDataFile;
    std::string taskTestDataFile;
	bool enableExecutionTime = false;
    bool verboseOutput = false;
};

enum class CommandLineStatus
{
    NoErrors,
    HelpRequested,
    HasErrors
};

auto parseCommandLine(int argc, char* argv[]) -> std::expected<ProgramOptions, CommandLineStatus>;

extern ProgramOptions programOptions;

#endif // COMMAND_LINE_PARSER_H_
