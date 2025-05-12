#include <iostream>
#include <string_view>
#include <vector>
#include "UserModel.h"
#include "UserModelToDBInterface.h"
#include "UserProfileTestData.h"

static std::vector<UserModel*> userProfileTestData = 
{
    {new UserModel("Chernick", "Paul", "A", "paul.chernick@chernicksw.com")},
    {new UserModel("Chernick", "Nina", "L", "ChernickNinaL@gmail.com")},
    {new UserModel("Chernick", "Dina", "B", "ChernickDinaB@gmail.com")},
    {new UserModel("Shiminovics", "Eitan", "I", "ShimonvicsEitanI@gmail.com")},
    {new UserModel("PacMan", "IN", "BW", "pacmaninbw@gmail.com")},
    {new UserModel("Black", "Patrick", "A", "BlackPatrickA@gmail.com")}
};


void loadUserProfileTestDataIntoDatabase()
{
    UserModelToDBInterface userDBInterface;

    for (auto user: userProfileTestData)
    {
        if (!userDBInterface.addToDatabaseTable(user))
        {
            std::cerr << userDBInterface.getAllErrorMessages() << "\n" << *user << "\n";
        }
    }
}


