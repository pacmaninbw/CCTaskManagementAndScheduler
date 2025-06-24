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
            if (user->isInDataBase())
            {
                UserModel_shp testInDB = userDBInterface.getUserByLogin(user->getLoginName());
                if (testInDB)
                {
                    if (*testInDB == *user)
                    {
                        std::cout << "User:" << user->getLastName() << ", " << user->getFirstName() <<
                            "Successfully iserted and retrieved from database\n";
                    }
                    else
                    {
                        std::cout << "Insertion and retrieval of User Failed\nInserted User:\n" <<
                        *user << "\n" "Retreived User:\n" << *testInDB << "\n";
                    }
                }
            }
            else
            {
                std::cout << "Primary key for user: " << user->getLastName() << ", " << user->getFirstName() <<
                " not set!\n";
            }
        }
    }

    for (UserModel* user: userProfileTestData)
    {
        delete user;
    }
    userProfileTestData.clear();
}


