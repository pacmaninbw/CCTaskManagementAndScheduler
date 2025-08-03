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
    UserModel_shp processResult(NSBM::results& results);
    UserList processResults(NSBM::results& results);
    void processResultRow(NSBM::row_view rv, UserModel_shp newUser);
    NSBA::awaitable<NSBM::results> coRoSelectUserByID(std::size_t userID);
    NSBA::awaitable<NSBM::results> coRoSelectUserByFullName(std::string_view lastName, 
        std::string_view firstName, std::string_view middleI);
    NSBA::awaitable<NSBM::results> coRoSelectUserByEmailAddress(std::string_view emailAddr);
    NSBA::awaitable<NSBM::results> coRoSelectUserByLoginName(std::string_view loginName);
    NSBA::awaitable<NSBM::results> coRoInsertUser(const UserModel& user);
    NSBA::awaitable<NSBM::results> coRoSelectAllUsers();
    NSBA::awaitable<NSBM::results> coRoSelectUserByLoginAndPassword(std::string_view loginName, std::string_view password);
};

#endif // USERDBINTERFACE_H_

