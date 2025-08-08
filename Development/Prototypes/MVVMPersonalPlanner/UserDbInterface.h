#ifndef USERDBINTERFACE_H_
#define USERDBINTERFACE_H_

#include "CommandLineParser.h"
#include "BoostDBInterfaceCore.h"
#include <string_view>
#include "UserModel.h"

class UserDbInterface : public BoostDBInterfaceCore
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
    NSBA::awaitable<NSBM::results> coRoSelectUserByID();
    NSBA::awaitable<NSBM::results> coRoSelectUserByFullName();
    NSBA::awaitable<NSBM::results> coRoSelectUserByEmailAddress();
    NSBA::awaitable<NSBM::results> coRoSelectUserByLoginName();
    NSBA::awaitable<NSBM::results> coRoInsertUser(const UserModel& user);
    NSBA::awaitable<NSBM::results> coRoSelectAllUsers();
    NSBA::awaitable<NSBM::results> coRoSelectUserByLoginAndPassword();

/*
 * The indexes below are based on the following select statement, maintain this order
 * for any new select statements, add any new field indexes at the end.
 *  SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
*      "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
*       "UseLettersForMajorPriority, SeparatePriorityWithDot FROM UserProfile WHERE UserID = {}", userID),
*/
private:
    const std::size_t UserIdIdx = 0;
    const std::size_t LastNameIdx = 1;
    const std::size_t FirstNameIdx = 2;
    const std::size_t MiddleInitialIdx = 3;
    const std::size_t EmailAddressIdx = 4;
    const std::size_t LoginNameIdx = 5;
    const std::size_t PasswordIdx = 6;
    const std::size_t StartDayIdx = 7;
    const std::size_t EndDayIdx = 8;
    const std::size_t PriorityGroupIdx = 9;
    const std::size_t PriorityIdx = 10;
    const std::size_t UseLettersIdx = 11;
    const std::size_t DotSeparationIdx = 12;
};

#endif // USERDBINTERFACE_H_

