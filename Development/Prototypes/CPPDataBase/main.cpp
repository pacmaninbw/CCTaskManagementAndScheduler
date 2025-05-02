#include "DBInterface.h"
#include <exception>
#include <iostream>
#include "UserProfileTestData.h"
#include "UserTaskTestData.h"

int main()
{
    DBInterface dbInterface;

    try {
        loadUserProfileTestDataIntoDatabase(dbInterface);
        loadUserTaskestDataIntoDatabase(dbInterface);
    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
