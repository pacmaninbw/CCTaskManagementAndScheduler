#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <iostream>
#include <string_view>
#include "UserProfileTestData.h"
#include "UserTaskTestData.h"
#include <vector>

std::vector<struct UserProfileTestData> userProfileTestData = 
{
    {"Chernick", "Paul", "A", "ChernickPaulA", "WhereforeWhyFore1234"},
    {"Chernick", "Nina", "L", "ChernickNinaL", "PrimaLinguaLTD"},
    {"Chernick", "Dina", "B", "ChernickDinaB", "At@LawLegalEagle"},
    {"Shiminovics", "Eitan", "I", "ShimonvicsEitanI", "justAKid!!"},
    {"PacMan", "IN", "BW", "pacmaninbw", "m0rNingGlory"},
    {"Black", "Patrick", "A", "BlackPatrickA", "#n0oG13$TsS"}
};

struct UserTaskTestData
{
    std::string_view userLoginName;
    unsigned int priorityInAllTasks;
    char majorPriority;
    unsigned int minorPriority;
    std::string_view descrption;
    std::string_view dueDate;
    unsigned int estimatedEffortHours;
    double actualEffortHours;
    std::string_view parentTaskDescription;
};

int main()
{
    boost::asio::io_context connectionContext;

    // Connection object
    boost::mysql::any_connection conn(connectionContext);

    // Connection parameters
    boost::mysql::connect_params dbConnectionParameters;
    dbConnectionParameters.server_address.emplace_host_and_port("127.0.0.1", 3306);
    dbConnectionParameters.username = "pacman2";
    dbConnectionParameters.password = "M1nus+1r3th";

    try {
        // Connect to the server
        conn.connect(dbConnectionParameters);

        // Execute a query
        boost::mysql::results result;
        conn.execute("SELECT 'Hello, Boost.MySQL!'", result);

        // Print the result
        std::cout << "Result: " << result.rows()[0][0] << std::endl;

        // Close the connection
        conn.close();
    } catch (const boost::mysql::error_with_diagnostics& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    connectionContext.run();

    return EXIT_SUCCESS;
}
