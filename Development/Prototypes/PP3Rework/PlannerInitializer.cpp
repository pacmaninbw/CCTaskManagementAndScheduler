// Project Header Files
#include "CommandLineParser.h"
#include "PlannerInitializer.h"
#include "TestDBConnection.h"
#include "UserModel.h"

// QT Header Files
#include <QtGlobal>
#include <QString>

// Standard C++ Header Files
#include <iostream>
#include <string>

PlannerInitializer::PlannerInitializer(int argc, char *argv[])
: loggedInUser{nullptr}
{
    if (!argc)
    {
        std::cerr << "No arguments to process, can't get program name" << std::endl;
        exit(EXIT_FAILURE);
    }

    programOptions.progName = argv[0];
    programOptions.userTestDataFile = "";
    programOptions.taskTestDataFile = "";
    programOptions.enableExecutionTime = false;
    programOptions.verboseOutput = false;
    programOptions.runSelfTest = false;
    programOptions.forceErrors = false;
    programOptions.forceExceptions = false;
    programOptions.quitFirstFail = false;
}

void PlannerInitializer::getProgramOptions()
{
    getEnvironmentVariables();
    
    testDbConnection();
}

std::shared_ptr<UserModel> PlannerInitializer::getUserDataWhenLoginAvailable()
{
    return loggedInUser;
}

void PlannerInitializer::getEnvironmentVariables()
{
    programOptions.mySqlUser = qEnvironmentVariable(makeEnvironmentVariableName("DBUSERNAME").c_str()).toStdString();

    programOptions.mySqlPassword = qEnvironmentVariable(makeEnvironmentVariableName("DBPASSWORD").c_str()).toStdString();

    programOptions.mySqlDBName = qEnvironmentVariable(makeEnvironmentVariableName("DATABASE").c_str()).toStdString();

    programOptions.mySqlUrl = qEnvironmentVariable(makeEnvironmentVariableName("DBURL").c_str()).toStdString();

    programOptions.mySqlPort = qEnvironmentVariableIntValue(makeEnvironmentVariableName("DBPORT").c_str());

    programOptions.userName = qEnvironmentVariable(makeEnvironmentVariableName("USERNAME").c_str()).toStdString();

    programOptions.userPassword = qEnvironmentVariable(makeEnvironmentVariableName("PASSWORD").c_str()).toStdString();
}

bool PlannerInitializer::testDbConnection()
{
    // Attempting user login really tests the connection by using the full capability
    if (!programOptions.userName.empty() && !programOptions.userPassword.empty())
    {
        programOptions.canConnectToDatabase = attemptUserLogin(programOptions.userName, programOptions.userPassword);
    }
    else
    {
        // Test if the database format options get set. 
        TestDBConnection dbConnectionTest;

        programOptions.canConnectToDatabase = dbConnectionTest.runTestConnect();
    }

    return programOptions.canConnectToDatabase;
}

std::string PlannerInitializer::makeEnvironmentVariableName(std::string vName)
{
    std::string environmentVariableName("CHERNICKSW_PLANNERORGANIZER_");

    environmentVariableName.append(vName);

    return environmentVariableName;
}

bool PlannerInitializer::attemptUserLogin(std::string loginName, std::string password)
{
    bool loginSuccessful = false;

    if (loginName.empty() || password.empty())
    {
        return loginSuccessful;
    }

    UserModel_shp retrievedUser = std::make_shared<UserModel>();
    if (retrievedUser->selectByLoginAndPassword(loginName, password))
    {
        retrievedUser->setLastLogin(std::chrono::system_clock::now());
        if (retrievedUser->update())
        {
            loggedInUser = retrievedUser;
            loginSuccessful = true;
        }
        else
        {
            std::cerr << "Last login update failed: "
                << retrievedUser->getAllErrorMessages() << std::endl;
        }
    }
    else
    {
        std::cerr << "Select User by login and password failed: "
                  << retrievedUser->getAllErrorMessages() << std::endl;
    }

    return loginSuccessful;

}
