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

    std::size_t m_UserIdIdx = IndexNotSet;
    std::size_t m_OrganizationIdx = IndexNotSet;
    std::size_t m_LastNameIdx = IndexNotSet;
    std::size_t m_FirstNameIdx = IndexNotSet;
    std::size_t m_MiddleInitialIdx = IndexNotSet;
    std::size_t m_EmailAddressIdx = IndexNotSet;
    std::size_t m_LoginNameIdx = IndexNotSet;
    std::size_t m_PasswordIdx = IndexNotSet;
    std::size_t m_UserAddedIdx = IndexNotSet;
    std::size_t m_LastLoginIdx = IndexNotSet;
    std::size_t m_PreferencesIdx = IndexNotSet;
    std::size_t m_HiddenIdx = IndexNotSet;
};

#endif // USERQUERYPROCESSOR_H_

