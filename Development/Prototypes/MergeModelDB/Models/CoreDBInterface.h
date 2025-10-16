#ifndef COREDBINTERFACECORE_H_
#define COREDBINTERFACECORE_H_

// Project Header Files
#include "CommandLineParser.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <iostream>
#include <optional>
#include <string>

class CoreDBInterface
{
public:
    CoreDBInterface();
    virtual ~CoreDBInterface() = default;
    std::string getAllErrorMessages() const noexcept { return errorMessages; };
    void setForceExceptions(bool forceIt) { forceException = forceIt; };

protected:
    void initFormatOptions();
    void appendErrorMessage(const std::string& newError) { errorMessages.append(newError); errorMessages.append("\n");};
/*
 * All calls to runQueryAsync and getConnectionFormatOptsAsync should be
 * implemented within try blocks.
 */
    boost::mysql::results runQueryAsync(const std::string& query);
    boost::mysql::format_options getConnectionFormatOptsAsync();
    boost::asio::awaitable<boost::mysql::results> coRoutineExecuteSqlStatement(const std::string& query);
    boost::asio::awaitable<boost::mysql::format_options> coRoutineGetFormatOptions();

    std::string errorMessages;
    boost::mysql::connect_params dbConnectionParameters;
    bool verboseOutput;
    bool forceError;
    bool forceException;
    bool inSelfTest;
    std::optional<boost::mysql::format_options> format_opts;
};

#endif // COREDBINTERFACECORE_H_


