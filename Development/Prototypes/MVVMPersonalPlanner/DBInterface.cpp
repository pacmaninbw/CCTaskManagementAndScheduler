#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "CommandLineParser.h"
#include "DBInterface.h"
#include <iostream>

DBInterface::DBInterface()
:   errorMessages{""}, databaseName{programOptions.mySqlDBName}, verboseOutput{programOptions.verboseOutput}
{
    dbConnectionParameters.server_address.emplace_host_and_port(programOptions.mySqlUrl, programOptions.mySqlPort);
    dbConnectionParameters.username = programOptions.mySqlUser;
    dbConnectionParameters.password = programOptions.mySqlPassword;
    dbConnectionParameters.database = programOptions.mySqlDBName;
}

