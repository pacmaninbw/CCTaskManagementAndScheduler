#ifndef TESTORGANIZATIONMODEL_H_
#define TESTORGANIZATIONMODEL_H_

class UserModel;

// Project Header Files
#include "commonTestValues.h"
#include "TestDBInterfaceCore.h"
#include "OrganizationModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class TestOrganizationModel : public TestDBInterfaceCore
{
public:
    TestOrganizationModel();
    ~TestOrganizationModel() = default;
    virtual TestStatus runAllTests() override;

private:
    OrganizationModel_shp organizationFactory(
        std::string orgName, std::string email, std::string phoneNumber, std::shared_ptr<UserModel> primaryContact,
        std::shared_ptr<UserModel> secondaryContact, 
        std::optional<std::chrono::system_clock::time_point> created,
        std::optional<std::chrono::system_clock::time_point> lastModified,
        std::string addressl1 = "", std::string addressl2 = "", 
        std::string city = "", std::string zipCode = "", std::string state = "", std::string nation = "",
        std::shared_ptr<UserModel> lastUpdatedBy = nullptr
    ) noexcept;

    TestStatus testPositivePathInsertions();
    TestStatus testPositivePathUpdates();
    TestStatus testPositivePathGetAllOrganizations() const;
    TestStatus testPositivePathGetOrganizationById() const;
    TestStatus testPositivePathFindOrganizationsByName() const;
    TestStatus testPositivePathFindOrganizationsByPrimaryContactID() const;
    TestStatus testPositivePathFindOrganizationsByPrimaryContactName() const;
    TestStatus testPositivePathGetAllOrganizationsAddedBetween() const;
    TestStatus testPositivePathGetAnyOrganizationsAddedOnDate() const;
    TestStatus testPositivePathGetAnyOrganizationsModifiedOnDate() const;
    TestStatus testPositivePathDeleteOrganization() const;
    TestStatus testPositivePathSetParentOrganization() const;
    TestStatus testNegativePathSetParentOrganization() const;
    TestStatus testNegativePathAlreadyInDataBase() const;
    TestStatus testnegativePathNotModified() const;
    
    std::size_t m_ordIdInDatabase = 0;
    std::shared_ptr<UserModel> m_userOne;
    std::shared_ptr<UserModel> m_userTwo;
    std::vector<OrganizationModel_shp> m_testData;
};

#endif // TESTORGANIZATIONMODEL_H_