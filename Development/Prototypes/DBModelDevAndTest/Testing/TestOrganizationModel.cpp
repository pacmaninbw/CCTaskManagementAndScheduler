// Project Header Files
#include "chronoBoostConversions.h"
#include "commonUtilities.h"
#include "OrganizationModel.h"
#include "TestOrganizationModel.h"
#include "UserQueryProcessor.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <string>
#include <vector>


TestOrganizationModel::TestOrganizationModel()
: TestDBInterfaceCore("Organization")
{
    UserQueryProcessor userQueryProcessor;
    m_userOne = userQueryProcessor.getUserByFullName("One", "User", "P");
    if (m_userOne == nullptr || !m_userOne->isInDataBase())
    {
        std::cerr << std::format("Failed to find userOne in DB! : {}\n", userQueryProcessor.getAllErrorMessages());
    }

    m_userTwo = userQueryProcessor.getUserByFullName("Doe", "John", "Q");
    if (m_userTwo == nullptr || !m_userTwo->isInDataBase())
    {
        std::cerr << std::format("Failed to find userTwo in DB! : {}\n", userQueryProcessor.getAllErrorMessages());
    }
}

TestOrganizationModel::~TestOrganizationModel()
{

}

OrganizationModel_shp TestOrganizationModel::organizationFactory(
    std::string orgName,
    std::string email,
    std::string phoneNumber,
    std::shared_ptr<UserModel> primaryContact,
    std::shared_ptr<UserModel> secondaryContact,
    std::optional<std::chrono::system_clock::time_point> created,
    std::optional<std::chrono::system_clock::time_point> lastModified,
    std::string addressl1,
    std::string addressl2,
    std::string city,
    std::string zipCode,
    std::string state,
    std::string nation
) noexcept
{
    OrganizationModel_shp newOrganization = std::make_shared<OrganizationModel>();

    newOrganization->setOrganizationName(orgName);
    newOrganization->setEmailAddress(email);
    newOrganization->setPhoneNumber(phoneNumber);
    newOrganization->setPrimaryContactUserId(primaryContact->getUserID());
    newOrganization->setSecondaryContactUserId(secondaryContact->getUserID());

    auto setTimeStampIfValid = [](OrganizationModel& object, auto setter, std::optional<std::chrono::system_clock::time_point> optionalTime) {
        if (optionalTime.has_value())
        {
            std::invoke(setter, object, optionalTime.value());
        }
    };

    setTimeStampIfValid(*newOrganization, &OrganizationModel::setCreationTimeStamp, created);
    setTimeStampIfValid(*newOrganization, &OrganizationModel::setLastModified, lastModified);

    // Source - https://stackoverflow.com/a/79999780
    // Posted by JaMiT
    // Retrieved 2026-09-01, License - CC BY-SA 4.0
    auto conditionalSetField = [](OrganizationModel& object, auto setter, std::string& str) {
        if (!str.empty())
        {
            std::invoke(setter, object, str);
        }
    };
    
    conditionalSetField(*newOrganization, &OrganizationModel::setAddressLine1, addressl1);
    conditionalSetField(*newOrganization, &OrganizationModel::setAddressLine2, addressl2);
    conditionalSetField(*newOrganization, &OrganizationModel::setCity, city);
    conditionalSetField(*newOrganization, &OrganizationModel::setPostalCode, zipCode);
    conditionalSetField(*newOrganization, &OrganizationModel::setStateOrProvince, state);
    conditionalSetField(*newOrganization, &OrganizationModel::setNation, nation);

    return newOrganization;
}

void TestOrganizationModel::creatTestOrganizations() noexcept
{
}

