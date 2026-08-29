#ifndef USERQUERYPROCESSOR_H_
#define USERQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "UserModel.h"

// Standard C++ Header Files

using UserModelList = std::vector<UserModel_shp>;
using StaticQueryUser = boost::mysql::static_results<boost::mysql::pfr_by_name<UserDbQueryValues>>;

class UserQueryProcessor : public QueryProcessor<UserModel, UserDbQueryValues>
{
public:
    UserQueryProcessor();
    virtual ~UserQueryProcessor() = default;
    UserModelList getAllUsers() noexcept;
    UserModel_shp getUserByID(std::size_t userId) noexcept;
    UserModel_shp getUserByLoginName(const std::string_view& loginName) noexcept;
    UserModel_shp getUserByEmail(const std::string_view& emailAddress) noexcept;
    UserModel_shp getUserByLoginAndPassword(const std::string_view& loginName, const std::string_view& password) noexcept;
    UserModel_shp getUserByFullName(const std::string_view& lastName, const std::string_view& firstName,
        const std::string_view& middleI) noexcept;


private:
    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllUsers() noexcept;
    TestStatus testExceptionGetUserById() noexcept;
    TestStatus testExceptionGetUserByLoginName() noexcept;
    TestStatus testExceptionGetUserByEmail() noexcept;
    TestStatus testExceptionGetUserByLoginAndPassword() noexcept;
    TestStatus testExceptionGetUserByFullName() noexcept;
};

#endif // USERQUERYPROCESSOR_H_

