#include "DBInterface.h"
#include <iostream>
#include <string_view>
#include <vector>
#include "UserModel.h"
#include "UserProfileTestData.h"


void loadUserProfileTestDataIntoDatabase()
{

    std::vector<UserModel*> userProfileTestData = 
    {
        {new UserModel("Chernick", "Paul", "A", "paul.chernick@chernicksw.com")},
        {new UserModel("Chernick", "Nina", "L", "ChernickNinaL@gmail.com")},
        {new UserModel("Chernick", "Dina", "B", "ChernickDinaB@gmail.com")},
        {new UserModel("Shiminovics", "Eitan", "I", "ShimonvicsEitanI@gmail.com")},
        {new UserModel("PacMan", "IN", "BW", "pacmaninbw@gmail.com")},
        {new UserModel("Black", "Patrick", "A", "BlackPatrickA@gmail.com")}
    };

    DBInterface userDBInterface;

    for (auto user: userProfileTestData)
    {
        if (!userDBInterface.insertIntoDataBase(*user))
        {
            std::cerr << userDBInterface.getAllErrorMessages() << "\n" << *user << "\n";
        }
        else
        {
            std::string login = user->getLoginName();
            UserModel_shp testInDB = userDBInterface.getUserByLogin(login);
            if (testInDB)
            {
                std::cout << *testInDB << "\n";
            }
        }
    }

    for (UserModel* user: userProfileTestData)
    {
        delete user;
    }
    userProfileTestData.clear();
}


