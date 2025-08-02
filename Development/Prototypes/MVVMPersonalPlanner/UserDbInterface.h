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
    UserModel_shp getUserByLoginAndPassword(std::string_view loginName, std::string_view password);
    UserList getAllUsers();

private:
    UserModel_shp processResult(bMysql::results& results);
    UserList processResults(bMysql::results& results);
    void processResultRow(bMysql::row_view rv, UserModel_shp newUser);
    bAsio::awaitable<bMysql::results> coRoSelectUserByID(std::size_t userID);
    bAsio::awaitable<bMysql::results> coRoSelectUserByFullName(std::string_view lastName, 
        std::string_view firstName, std::string_view middleI);
    bAsio::awaitable<bMysql::results> coRoSelectUserByEmailAddress(std::string_view emailAddr);
    bAsio::awaitable<bMysql::results> coRoSelectUserByLoginName(std::string_view loginName);
    bAsio::awaitable<bMysql::results> coRoInsertUser(const UserModel& user);
    bAsio::awaitable<bMysql::results> coRoSelectAllUsers();
    bAsio::awaitable<bMysql::results> coRoSelectUserByLoginAndPassword(std::string_view loginName, std::string_view password);
};

#endif // USERDBINTERFACE_H_

