#ifndef PLANNERINITIALIZER_H_
#define PLANNERINITIALIZER_H_

class UserModel;

// Project Header Files
#include "CommandLineParser.h"

// QT Header Files

// Standard C++ Header Files
#include <memory>
#include <string>


class PlannerInitializer
{
public:
    PlannerInitializer(int argc, char *argv[]);
    void getProgramOptions();
    std::shared_ptr<UserModel> getUserDataWhenLoginAvailable();

private:
    void getEnvironmentVariables();
    bool testDbConnection();
    std::string makeEnvironmentVariableName(std::string vName);
    bool attemptUserLogin(std::string loginName, std::string password);

    std::shared_ptr<UserModel> m_loggedInUser;
};

#endif // PLANNERINITIALIZER_H_

