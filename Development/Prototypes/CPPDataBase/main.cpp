#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "DBInterface.h"
#include <iostream>
#include "UserProfileTestData.h"
#include "UserTaskTestData.h"

int main()
{
    DBInterface dbInterface;

    try {
        loadUserProfileTestDataIntoDatabase(dbInterface);
        loadUserTaskestDataIntoDatabase(dbInterface);
    } catch (const boost::mysql::error_with_diagnostics& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
