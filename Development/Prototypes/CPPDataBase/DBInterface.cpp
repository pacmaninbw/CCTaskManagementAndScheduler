#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "dbConnectionParameters.h"
#include "DBInterface.h"

DBInterface::DBInterface()
{
    boost::mysql::connect_params* dbConnectionParameters = new(boost::mysql::connect_params);
    dbConnectionParameters->server_address.emplace_host_and_port("127.0.0.1", 3306);
    dbConnectionParameters->username = MySQLRootUser;
    dbConnectionParameters->password = MySQLRootPassword;
    dbConnectionParameters->database = PlannerDB;

    vDBConnectionParams = dbConnectionParameters;
}

DBInterface::~DBInterface()
{
    boost::mysql::connect_params* dbConnectionParameters = static_cast<boost::mysql::connect_params*>(vDBConnectionParams);

    delete(dbConnectionParameters);
}

static boost::asio::awaitable<void> coro_addUser(
    boost::mysql::connect_params& dbConnetParams,
    std::string_view lastName,
    std::string_view firstName,
    std::string_view middleInitial,
    std::string_view loginName,
    std::string_view hashedPassword
)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnetParams);

    boost::mysql::results result;
    co_await conn.async_execute(
        boost::mysql::with_params("CALL addNewUser({}, {}, {}, {}, {})", 
            lastName, firstName, middleInitial, loginName, hashedPassword),
        result
    );

    co_await conn.async_close();
}

void DBInterface::addUser(
    std::string_view lastName,
    std::string_view firstName,
    std::string_view middleInitial,
    std::string_view loginName,
    std::string_view hashedPassword
)
{
    coro_addUser(*static_cast<boost::mysql::connect_params*>(vDBConnectionParams),
        lastName, firstName, middleInitial, loginName, hashedPassword);
}

void DBInterface::addUserTaskWithUserID()
{
}

void DBInterface::addUserTaskWithLoginName()
{
}
