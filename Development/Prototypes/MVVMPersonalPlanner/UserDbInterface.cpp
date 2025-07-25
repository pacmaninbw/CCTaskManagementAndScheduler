#include "CommandLineParser.h"
#include "DBInterface.h"
#include "UserDbInterface.h"
#include "UserModel.h"

UserDbInterface::UserDbInterface(ProgramOptions &programOptions)
: DBInterface(programOptions)
{
}

std::size_t UserDbInterface::insert(const UserModel &user)
{
    try
    {
        boost::asio::io_context ctx;
        boost::mysql::results localResult;

        boost::asio::co_spawn(
            ctx, coRoInsertUser(user),
            [&localResult, this](std::exception_ptr ptr, boost::mysql::results result)
            {
                if (ptr)
                {
                    std::rethrow_exception(ptr);
                }
                localResult = std::move(result);
            }
        );

        ctx.run();

        return localResult.last_insert_id();
    }

    catch(const std::exception& e)
    {
        std::string eMsg("In UserDbInterface::getUserByUserID ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return 0;
    }
}

UserModel_shp UserDbInterface::getUserByUserID(std::size_t userID)
{
    try
    {
        boost::asio::io_context ctx;
        boost::mysql::results localResult;

        boost::asio::co_spawn(
            ctx, coRoSelectUserByID(userID),
            [&localResult, this](std::exception_ptr ptr, boost::mysql::results result)
            {
                if (ptr)
                {
                    std::rethrow_exception(ptr);
                }
                localResult = std::move(result);
            }
        );

        ctx.run();

        return processResults(localResult);
    }

    catch(const std::exception& e)
    {
        std::string eMsg("In UserDbInterface::getUserByUserID ");
        eMsg += e.what();
        appendErrorMessage(eMsg);
        return nullptr;
    }    
}

UserModel_shp UserDbInterface::getUserByFullName(std::string_view lastName, std::string_view firstName, std::string_view middleI)
{
    std::cout << lastName << firstName << middleI << "\n";
    return UserModel_shp();
}

UserModel_shp UserDbInterface::getUserByEmail(std::string_view emailAddress)
{
    std::cout << emailAddress;
    return UserModel_shp();
}

UserModel_shp UserDbInterface::getUserByLoginName(std::string_view loginName)
{
    std::cout << loginName;
    return UserModel_shp();
}

UserModel_shp UserDbInterface::processResults(boost::mysql::results& results)
{
    if (results.rows().empty())
    {
        appendErrorMessage("User not found!");
        return nullptr;
    }

    if (results.rows().size() > 1)
    {
        appendErrorMessage("Too many users found to process!");
        return nullptr;
    }

    UserModel_shp newUser = std::make_shared<UserModel>(UserModel());
    boost::mysql::row_view rv = results.rows().at(0);
    newUser->setUserID(rv.at(0).as_uint64());
    newUser->setLastName(rv.at(1).as_string());
    newUser->setFirstName(rv.at(2).as_string());
    newUser->setMiddleInitial(rv.at(3).as_string());
    newUser->setEmail(rv.at(4).as_string());
    newUser->setLoginName(rv.at(5).as_string());
    newUser->setPassword(rv.at(6).as_string());
    newUser->setStartTime(rv.at(7).as_string());
    newUser->setEndTime(rv.at(8).as_string());
    if (!rv.at(9).is_null())
    {
        newUser->setPriorityInSchedule(rv.at(9).as_int64());
    }
    if (!rv.at(10).is_null())
    {
        newUser->setMinorPriorityInSchedule(rv.at(10).as_int64());
    }
    if (!rv.at(11).is_null())
    {
        newUser->setUsingLettersForMaorPriority(rv.at(11).as_int64());
    }
    if (!rv.at(12).is_null())
    {
        newUser->setSeparatingPriorityWithDot(rv.at(12).as_int64());
    }

    // All the set functions set modified, since this user is new in memory it is not modified.
    newUser->clearModified();

    return newUser;
}

boost::asio::awaitable<boost::mysql::results> UserDbInterface::coRoSelectUserByID(std::size_t userID)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    conn.set_meta_mode(boost::mysql::metadata_mode::full);

    boost::mysql::results result;

    co_await conn.async_execute(
        boost::mysql::with_params("SELECT UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
            "UseLettersForMajorPriority, SeparatePriorityWithDot FROM UserProfile WHERE UserID = {}", userID),
        result
    );

    co_await conn.async_close();

    co_return result;
}

boost::asio::awaitable<boost::mysql::results> UserDbInterface::coRoSelectUserByFullName(std::string lastName, std::string firstName, std::string middleInit)
{
    std::cout << lastName << firstName << middleInit << "\n";
    return boost::asio::awaitable<boost::mysql::results>();
}

boost::asio::awaitable<boost::mysql::results> UserDbInterface::coRoSelectUserByEmailAddress(std::string emailAddr)
{
    std::cout << emailAddr << "\n";
    return boost::asio::awaitable<boost::mysql::results>();
}

boost::asio::awaitable<boost::mysql::results> UserDbInterface::coRoSelectUserByLoginName(std::string loginName)
{
    std::cout << loginName << "\n";
    return boost::asio::awaitable<boost::mysql::results>();
}

boost::asio::awaitable<boost::mysql::results> UserDbInterface::coRoInsertUser(const UserModel& user)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    conn.set_meta_mode(boost::mysql::metadata_mode::full);

    boost::mysql::results result;

    co_await conn.async_execute(
        boost::mysql::with_params("INSERT INTO UserProfile (UserID, LastName, FirstName, MiddleInitial, EmailAddress, LoginName, "
            "HashedPassWord, ScheduleDayStart, ScheduleDayEnd, IncludePriorityInSchedule, IncludeMinorPriorityInSchedule, "
            "UseLettersForMajorPriority, SeparatePriorityWithDot) VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12})",
             user.getUserID(), user.getLastName(), user.getFirstName(), user.getMiddleInitial()),
        result
    );

    co_await conn.async_close();

    co_return result;
}

