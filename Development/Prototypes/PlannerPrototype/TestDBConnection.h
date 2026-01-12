#ifndef TESTDBCONNECTION_H
#define TESTDBCONNECTION_H

// Project Header Files
#include "CommandLineParser.h"
#include "commonTestValues.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

class TestDBConnection
{
public:
    TestDBConnection();
    ~TestDBConnection() = default;
    void debugShowVariables(std::string functionName) const noexcept;
    TestStatus runTestConnect();
    
private:
    void initConnectionData(ProgramOptions &po);
    boost::mysql::format_options getConnectionFormatOptsAsync();
    boost::asio::awaitable<boost::mysql::format_options> coRoutineGetFormatOptions();

    std::string errorMessages;
    boost::mysql::connect_params dbConnectionParameters;
    bool verboseOutput;
    bool forceError;
    bool forceException;
    bool inSelfTest;
    std::optional<boost::mysql::format_options> format_opts;

};

#endif