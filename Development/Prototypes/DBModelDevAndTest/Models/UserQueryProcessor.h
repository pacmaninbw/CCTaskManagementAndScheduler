#ifndef USERQUERYPROCESSOR_H_
#define USERQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "UserModel.h"

// Standard C++ Header Files

using UserModelList = std::vector<UserModel_shp>;

class UserQueryProcessor : public QueryProcessor<UserModel>
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
    virtual UserModel_shp processResultRow(boost::mysql::row_view& queryRow) override;
    virtual void fillRequiredIndexes() override;

    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllUsers() noexcept;
    TestStatus testExceptionGetUserById() noexcept;
    TestStatus testExceptionGetUserByLoginName() noexcept;
    TestStatus testExceptionGetUserByEmail() noexcept;
    TestStatus testExceptionGetUserByLoginAndPassword() noexcept;
    TestStatus testExceptionGetUserByFullName() noexcept;

    std::size_t m_userIdIdx = IndexNotSet;
    std::size_t m_organizationIdx = IndexNotSet;
    std::size_t m_lastNameIdx = IndexNotSet;
    std::size_t m_firstNameIdx = IndexNotSet;
    std::size_t m_middleInitialIdx = IndexNotSet;
    std::size_t m_emailAddressIdx = IndexNotSet;
    std::size_t m_loginNameIdx = IndexNotSet;
    std::size_t m_passwordIdx = IndexNotSet;
    std::size_t m_userAddedIdx = IndexNotSet;
    std::size_t m_lastLoginIdx = IndexNotSet;
    std::size_t m_preferencesIdx = IndexNotSet;
    std::size_t m_hiddenIdx = IndexNotSet;
};

#endif // USERQUERYPROCESSOR_H_

