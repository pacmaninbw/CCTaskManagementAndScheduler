// Project Header Files
#include "DataBaseConnectionDialog.h"
#include "LoginDialog.h"
#include "PlannerInitializer.h"
#include "UserDashboard.h"

// QT Header Files
#include <QApplication>

// Standard C++ Header Files
#include <memory>


// Standard C++ Header Files
/*
 * All of the DBInterface classes need access to the programOptions global variable for the
 * MySQL user name and password, as well as the database name and other connection details.
 */
ProgramOptions programOptions;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::shared_ptr<UserModel> logedInUser;

    PlannerInitializer plannerOptions(argc, argv);
    
    plannerOptions.getProgramOptions();

    if (programOptions.canConnectToDatabase)
    {
        logedInUser = plannerOptions.getUserDataWhenLoginAvailable();
    }
    else
    {
        DataBaseConnectionDialog dbConnectionDialog;

        dbConnectionDialog.exec();
    }

    UserDashboard userDashboard(logedInUser);
    userDashboard.show();
    return a.exec();
}
