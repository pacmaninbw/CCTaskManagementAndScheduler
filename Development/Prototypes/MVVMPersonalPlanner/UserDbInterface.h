#ifndef USERDBINTERFACE_H_
#define USERDBINTERFACE_H_

#include "CommandLineParser.h"
#include "DBInterface.h"
#include <string_view>
#include "UserModel.h"

class UserDbInterface : public DBInterface
{
public:
    UserDbInterface();
    ~UserDbInterface() = default;
    std::size_t insert(const UserModel& user);
    std::size_t insert(UserModel_shp userP) { return insert(*userP); };
    UserModel_shp getUserByUserID(std::size_t userID);
    UserModel_shp getUserByFullName(std::string_view lastName, std::string_view firstName, std::string_view middleI);
    UserModel_shp getUserByEmail(std::string_view emailAddress);
    UserModel_shp getUserByLoginName(std::string_view loginName);

private:
    UserModel_shp processResults(boost::mysql::results& results);
    boost::asio::awaitable<boost::mysql::results>  coRoSelectUserByID(std::size_t userID);
    boost::asio::awaitable<boost::mysql::results>  coRoSelectUserByFullName(std::string_view lastName, 
        std::string_view firstName, std::string_view middleI);
    boost::asio::awaitable<boost::mysql::results>  coRoSelectUserByEmailAddress(std::string_view emailAddr);
    boost::asio::awaitable<boost::mysql::results>  coRoSelectUserByLoginName(std::string_view loginName);
    boost::asio::awaitable<boost::mysql::results>  coRoInsertUser(const UserModel& user);

};

#endif // USERDBINTERFACE_H_

