#include "DBInterface.h"
#include <iostream>
#include <string_view>
#include <vector>
#include "UserProfileTestData.h"

struct UserProfileTestData
{
    std::string_view lastName;
    std::string_view firstName;
    std::string_view middleInitial;
    std::string_view loginName;
    std::string_view hashedPassword;
};

static std::vector<struct UserProfileTestData> userProfileTestData = 
{
    {"Chernick", "Paul", "A", "ChernickPaulA", "WhereforeWhyFore1234"},
    {"Chernick", "Nina", "L", "ChernickNinaL", "PrimaLinguaLTD"},
    {"Chernick", "Dina", "B", "ChernickDinaB", "At@LawLegalEagle"},
    {"Shiminovics", "Eitan", "I", "ShimonvicsEitanI", "justAKid!!"},
    {"PacMan", "IN", "BW", "pacmaninbw", "m0rNingGlory"},
    {"Black", "Patrick", "A", "BlackPatrickA", "#n0oG13$TsS"}
};


void loadUserProfileTestDataIntoDatabase(DBInterface& dbInterface)
{
    for (auto user: userProfileTestData)
    {
        dbInterface.addUser(user.lastName, user.firstName, user.middleInitial, user.loginName, user.hashedPassword);
    }
}
