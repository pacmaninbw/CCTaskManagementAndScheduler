#ifndef COREDBINTERFACECORE_H_
#define COREDBINTERFACECORE_H_

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include <iostream>
#include <optional>
#include <string>

namespace NSBA = boost::asio;
namespace NSBM = boost::mysql;

class CoreDBInterface
{
public:
    CoreDBInterface();
    virtual ~CoreDBInterface() = default;
    std::string getAllErrorMessages() const noexcept { return errorMessages; };

protected:
    void initFormatOptions();
    void prepareForRunQueryAsync();
    void appendErrorMessage(const std::string& newError) { errorMessages.append(newError); errorMessages.append("\n");};
/*
 * All calls to runQueryAsync and getConnectionFormatOptsAsync should be
 * implemented within try blocks.
 */
    NSBM::results runQueryAsync(const std::string& query);
    NSBM::format_options getConnectionFormatOptsAsync();
    NSBA::awaitable<NSBM::results> coRoutineExecuteSqlStatement(const std::string& query);
    NSBA::awaitable<NSBM::format_options> coRoutineGetFormatOptions();

    std::string errorMessages;
    NSBM::connect_params dbConnectionParameters;
    bool verboseOutput;
    std::optional<NSBM::format_options> format_opts;
};

#endif // COREDBINTERFACECORE_H_


