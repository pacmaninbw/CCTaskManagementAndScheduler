// Project Header Files
#include "chronoBoostConversions.h"
#include "commonUtilities.h"
#include "OrganizationModel.h"
#include "OrganizationQueryProcessor.h"
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
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathInsertions, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathUpdates, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathGetAllOrganizations, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathGetOrganizationById, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathFindOrganizationsByName, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathFindOrganizationsByPrimaryContactID, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathFindOrganizationsByPrimaryContactName, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathGetAllOrganizationsAddedBetween, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathGetAnyOrganizationsAddedOnDate, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathGetAnyOrganizationsModifiedOnDate, this));
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathDeleteOrganization, this));

    m_negativePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testNegativePathAlreadyInDataBase, this));
    m_negativePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testnegativePathNotModified, this));
}

TestOrganizationModel::~TestOrganizationModel()
{

}

TestStatus TestOrganizationModel::runAllTests()
{
    std::cout << std::format("\nRunning {} Integration Tests\n", m_modelUnderTest);
    TestStatus positivePathPassed = TESTPASSED;
    TestStatus negativePathPassed = TESTPASSED;
    TestStatus allTestsStatus = TESTPASSED;
    std::string failReason("");

    try {
        UserQueryProcessor userQueryProcessor;
        m_userOne = userQueryProcessor.getUserByFullName("One", "User", "P");
        if (m_userOne == nullptr || !m_userOne->isInDataBase())
        {
            std::cerr << std::format("Failed to find userOne in DB! : {}\n", userQueryProcessor.getAllErrorMessages());
            return TESTFAILED;
        }

        m_userTwo = userQueryProcessor.getUserByFullName("Doe", "John", "Q");
        if (m_userTwo == nullptr || !m_userTwo->isInDataBase())
        {
            std::cerr << std::format("Failed to find userTwo in DB! : {}\n", userQueryProcessor.getAllErrorMessages());
            return TESTFAILED;
        }

        positivePathPassed = runPositivePathTests();
        negativePathPassed = runNegativePathTests();

        allTestsStatus =
            (positivePathPassed == TESTPASSED && negativePathPassed == TESTPASSED) ? TESTPASSED : TESTFAILED;
    }
    catch(std::exception& e)
    {
        failReason = std::format("Caught exception:{} ", e.what());
        allTestsStatus = TEXTEXCEPTION;
    }

    reportTestStatus(allTestsStatus, failReason);

    return allTestsStatus;
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
    std::optional<std::chrono::system_clock::time_point> optTimeStamp = common::TestTimeStampValue;

    m_testData.push_back(organizationFactory("First Test Orgnaization, no address",
        "firstTestOrganization@gmail.com", "(800) 555-1212", m_userOne, m_userTwo, optTimeStamp,
        optTimeStamp));
}

TestStatus TestOrganizationModel::testPositivePathInsertions()
{
    std::optional<std::chrono::system_clock::time_point> optTimeStamp = common::TestTimeStampValue;
    OrganizationModel_shp newOrganization = organizationFactory(
        "TestOrg1", "TestOrg1@gmail.com", "(800) 555-1212", m_userOne, m_userTwo, optTimeStamp, optTimeStamp
    );

    if (!newOrganization->insert())
    {
        std::cout << "Insertion failed for Organization: " << *newOrganization << " :\n";
        std::cout << newOrganization->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathUpdates()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testPositivePathGetAllOrganizations()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testPositivePathGetOrganizationById()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testPositivePathFindOrganizationsByName()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testPositivePathFindOrganizationsByPrimaryContactID()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testPositivePathFindOrganizationsByPrimaryContactName()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testPositivePathGetAllOrganizationsAddedBetween()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testPositivePathGetAnyOrganizationsAddedOnDate()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testPositivePathGetAnyOrganizationsModifiedOnDate()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testPositivePathDeleteOrganization()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testNegativePathAlreadyInDataBase()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}

TestStatus TestOrganizationModel::testnegativePathNotModified()
{
    std::cerr << std::format("TestOrganizationModel::{} NOT Implemented\n", __func__);
    return TESTFAILED;
}
