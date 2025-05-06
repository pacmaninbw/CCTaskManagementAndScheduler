#include "DBInterface.h"
#include "UserModel.h"
#include <iostream>
#include <string_view>
#include <vector>
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


void loadUserProfileTestDataIntoDatabase(DBInterface& dbInterface)
{
    for (auto user: userProfileTestData)
    {
        std::cout << user << "\n\n";
        dbInterface.addUser(user);
    }
}
