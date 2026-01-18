// Project Header Files
#include "CommandLineParser.h"
#include "UserDashboard.h"

// QT Header Files
#include <QApplication>

// Standard C++ Header Files


// Standard C++ Header Files
/*
 * All of the DBInterface classes need access to the programOptions global variable for the
 * MySQL user name and password, as well as the database name and other connection details.
 */
ProgramOptions programOptions;

static void initPlannerOptions(ProgramOptions& programOptions, char* arg0)
{
    programOptions.progName = arg0;
    programOptions.mySqlUrl = "127.0.0.1";
    programOptions.mySqlPort = 3306;
    programOptions.mySqlDBName = "PlannerTaskScheduleDB";
    programOptions.userTestDataFile = "";
    programOptions.taskTestDataFile = "";
    programOptions.enableExecutionTime = false;
    programOptions.verboseOutput = false;
    programOptions.runSelfTest = false;
    programOptions.forceErrors = false;
    programOptions.forceExceptions = false;
    programOptions.quitFirstFail = false;
}

int main(int argc, char *argv[])
{
    initPlannerOptions(programOptions, argv[0]);

    QApplication a(argc, argv);
    UserDashboard userDashboard;
    userDashboard.show();
    return a.exec();
}
