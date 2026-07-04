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
    void testAllConnections() { m_testProductionConn = true; };
    
private:
    void initConnectionData(ProgramOptions &po);
    boost::mysql::format_options getConnectionFormatOptsAsync();
    boost::asio::awaitable<boost::mysql::format_options> coRoutineGetFormatOptions();

    std::string m_errorMessages;
    boost::mysql::connect_params m_dbConnection;
    bool m_verboseOutput;
    bool m_forceError;
    bool m_forceException;
    bool m_selfTest;
    bool m_testProductionConn;
    std::optional<boost::mysql::format_options> m_formatOpts;

};

#endif