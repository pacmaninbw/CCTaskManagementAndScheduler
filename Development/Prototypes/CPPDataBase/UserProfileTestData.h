#ifndef USERPROFILETESTDATA_H_
#define USERPROFILETESTDATA_H_

#include <string_view>

struct UserProfileTestData
{
    std::string_view lastName;
    std::string_view firstName;
    std::string_view middleInitial;
    std::string_view loginName;
    std::string_view hashedPassword;
};

#endif // USERPROFILETESTDATA_H_
