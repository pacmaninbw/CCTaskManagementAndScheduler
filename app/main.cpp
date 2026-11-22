// Project Header Files
#include "DataBaseConnectionDialog.h"
#include "LoginDialog.h"
#include "PlannerInitializer.h"
#include "UserDashboard.h"
#include "UserModel.h"

// QT Header Files
#include <QApplication>

// Standard C++ Header Files
#include <memory>

/*
 * All of the DBInterface classes need access to the programOptions global variable for the
 * MySQL user name and password, as well as the database name and other connection details.
 */
ProgramOptions programOptions;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UserModel_shp logedInUser = nullptr;

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

        LoginDialog loginDialog;

        loginDialog.exec();

        logedInUser = loginDialog.GetUserData();
    }

    if (logedInUser != nullptr && logedInUser->getUserID() != 0)
    {
        UserDashboard* userDashboard = new UserDashboard(logedInUser);
        userDashboard->show();
    }
    else
    {
        return EXIT_FAILURE;
    }

    return a.exec();
}
