#include <iostream>
#include <string_view>
#include <vector>
#include "UserModel.h"
#include "UserModelToDBInterface.h"
#include "UserProfileTestData.h"

static std::vector<UserModel> userProfileTestData = 
{
    {"Chernick", "Paul", "A", "paul.chernick@chernicksw.com"},
    {"Chernick", "Nina", "L", "ChernickNinaL@gmail.com"},
    {"Chernick", "Dina", "B", "ChernickDinaB@gmail.com"},
    {"Shiminovics", "Eitan", "I", "ShimonvicsEitanI@gmail.com"},
    {"PacMan", "IN", "BW", "pacmaninbw@gmail.com"},
    {"Black", "Patrick", "A", "BlackPatrickA@gmail.com"}
};


void loadUserProfileTestDataIntoDatabase()
{
    UserModelToDBInterface userDBInterface;

    for (auto user: userProfileTestData)
    {
        std::cout << user << "\n\n";
        userDBInterface.addUserToDB(user);
    }
}
