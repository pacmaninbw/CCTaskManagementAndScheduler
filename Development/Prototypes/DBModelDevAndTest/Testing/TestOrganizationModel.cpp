// Project Header Files
#include "chronoBoostConversions.h"
#include "commonUtilities.h"
#include "OrganizationModel.h"
#include "OrganizationQueryProcessor.h"
#include "TestOrganizationModel.h"
#include "UserQueryProcessor.h"

// Standard C++ Header Files
#include <algorithm>
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
    m_positiviePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testPositivePathSetParentOrganization, this));

    m_negativePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testNegativePathSetParentOrganization, this));
    m_negativePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testNegativePathAlreadyInDataBase, this));
    m_negativePathTestFuncsNoArgs.push_back(std::bind(&TestOrganizationModel::testnegativePathNotModified, this));
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

TestStatus TestOrganizationModel::testPositivePathInsertions()
{
    std::optional<std::chrono::system_clock::time_point> optTimeStamp = common::TestTimeStampValue;
    OrganizationModel_shp newOrganization = organizationFactory(
        "TestOrg1", "TestOrg1@gmail.com", "(800) 555-1212", m_userOne, m_userTwo, optTimeStamp, optTimeStamp
    );

    if (!newOrganization->insert())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__) << *newOrganization << " :\n";
        std::cerr << newOrganization->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    // Test the maximum number of fields allowed
    OrganizationModel_shp allFieldsFilled = organizationFactory(
        "AllFieldsFilled", "AllFieldsFilled@gmail.com", "(800) 555-1313", m_userOne, m_userTwo, optTimeStamp, optTimeStamp,
        "9999 Any Street", "Suite 101", "Los Angeles", "90049", "California", "USA"
    );
    if (!allFieldsFilled->insert())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__) << *allFieldsFilled << " :\n";
        std::cerr << allFieldsFilled->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    // Test the absolute minimum number of fields required
    optTimeStamp.reset();
    OrganizationModel_shp noTimeStamp = organizationFactory(
        "NoTimeStampOrg", "NoTimeStampOrg@gmail.com", "(800) 555-1414", m_userOne, m_userTwo, optTimeStamp, optTimeStamp
    );
    if (!noTimeStamp->insert())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__) << *noTimeStamp << " :\n";
        std::cerr << noTimeStamp->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathUpdates()
{
    std::optional<std::chrono::system_clock::time_point> optTimeStamp = common::TestTimeStampValue;

    OrganizationModel_shp testModify = organizationFactory(
        "TestModify", "TestModify@gmail.com", "(800) 555-1515", m_userOne, m_userTwo, optTimeStamp, optTimeStamp
    );

    if (!testModify->insert())
    {
        std::cerr << "Insertion failed for Organization in Update Test: " << *testModify << " :\n";
        std::cerr << testModify->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    testModify->setAddressLine1("1919 Modify Way");
    testModify->setCity("Los Angeles");
    testModify->setPostalCode("90049");
    testModify->setStateOrProvince("California");
    testModify->setNation("USA");

    if (!testModify->update())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__) << *testModify << " :\n";
        std::cerr << testModify->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathGetAllOrganizations()
{
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationList allOrganizations = organizationQueryProcessor.getAllOrganizations();

    if (allOrganizations.empty())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__);
        std::cerr << organizationQueryProcessor.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
        std::cout << "Found " << allOrganizations.size() << " Organizations\n";
        for (auto organization: allOrganizations)
        {
            std::cout << *organization << "\n";
        }
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathGetOrganizationById()
{
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationModel_shp firstOrganization = organizationQueryProcessor.getOrganizationById(1);

    if (firstOrganization == nullptr)
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__);
        std::cerr << organizationQueryProcessor.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
        std::cout << "Found " << *firstOrganization << "\n";
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathFindOrganizationsByName()
{
    std::string searchName("TestOrg1");
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationList firstOrganization = organizationQueryProcessor.findOrganizationsByName(searchName);

    if (firstOrganization.empty())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__);
        std::cerr << organizationQueryProcessor.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
        std::cout << "Found " << firstOrganization.size() << " Organizations\n";
        for (auto organization: firstOrganization)
        {
            std::cout << *organization << "\n";
        }
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathFindOrganizationsByPrimaryContactID()
{
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationList orgList = organizationQueryProcessor.findOrganizationsByPrimaryContactID(m_userOne->getUserID());

    if (orgList.empty())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__);
        std::cerr << organizationQueryProcessor.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
        std::cout << "Found " << orgList.size() << " Organizations\n";
        for (auto organization: orgList)
        {
            std::cout << *organization << "\n";
        }
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathFindOrganizationsByPrimaryContactName()
{
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationList orgList = organizationQueryProcessor.findOrganizationsByPrimaryContactName(
        m_userOne->getFirstName(), m_userOne->getLastName(), m_userOne->getMiddleInitial());

    if (orgList.empty())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__);
        std::cerr << organizationQueryProcessor.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
        std::cout << "Found " << orgList.size() << " Organizations\n";
        for (auto organization: orgList)
        {
            std::cout << *organization << "\n";
        }
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathGetAllOrganizationsAddedBetween()
{
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationList orgList = organizationQueryProcessor.getAllOrganizationsAddedBetween(common::TestDateRangeStartValue, common::TestDateRangeEndValue);

    if (orgList.empty())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__);
        std::cerr << organizationQueryProcessor.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
        std::cout << "Found " << orgList.size() << " Organizations\n";
        for (auto organization: orgList)
        {
            std::cout << *organization << "\n";
        }
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathGetAnyOrganizationsAddedOnDate()
{
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationList orgList = organizationQueryProcessor.getAnyOrganizationsAddedOnDate(common::TestDateValue);

    if (orgList.empty())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__);
        std::cerr << organizationQueryProcessor.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
        std::cout << "Found " << orgList.size() << " Organizations\n";
        for (auto organization: orgList)
        {
            std::cout << *organization << "\n";
        }
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathGetAnyOrganizationsModifiedOnDate()
{
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationList orgList = organizationQueryProcessor.getAnyOrganizationsModifiedOnDate(common::TestDateValue);

    if (orgList.empty())
    {
        std::cerr << std::format("TestOrganizationModel::{} FAILED!\n", __func__);
        std::cerr << organizationQueryProcessor.getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << std::format("TestOrganizationModel::{} PASSED!\n", __func__);
        std::cout << "Found " << orgList.size() << " Organizations\n";
        for (auto organization: orgList)
        {
            std::cout << *organization << "\n";
        }
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathDeleteOrganization()
{
    std::string testFuncName("Delete Organization");

    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationList allActiveOrganizations = organizationQueryProcessor.getAllActiveOrganizations();
    if (allActiveOrganizations.empty())
    {
        std::cerr << testFuncName << ": No active organizations found!! FAILED!!" << std::endl;
        return TESTFAILED;
    }

    std::size_t itemToHideIndex = allActiveOrganizations.size() > 3? allActiveOrganizations.size() - 2 : allActiveOrganizations.size() - 1;
    OrganizationModel_shp organizationToDelete = allActiveOrganizations[itemToHideIndex];
    if (!organizationToDelete->hide(m_userOne->getUserID()))
    {
        std::cerr << std::format("itemToHide->hide({}) FAILED!", m_userOne->getUserID()) << organizationToDelete->getAllErrorMessages() << "\n";
        return TESTFAILED;
    }

    if (!organizationToDelete->isDeleted())
    {
        std::cerr << std::format("{}: Organization ({}) was not marked as deleted {} FAILED\n", testFuncName,
            organizationToDelete->getOrganizationId(), organizationToDelete->getAllErrorMessages());
        return TESTFAILED;
    }

    OrganizationList alteredList = organizationQueryProcessor.getAllActiveOrganizations();
    if (!(alteredList.size() < allActiveOrganizations.size()))
    {
        std::cerr << std::format("{} ({}) did not decrease the size of the Organization list. TEST FAILED\n",
            testFuncName, organizationToDelete->getOrganizationId());
        return TESTFAILED;
    }

    auto foundDeletedOrganization = [&](OrganizationModel_shp item){ return item->getOrganizationId() == organizationToDelete->getOrganizationId(); };

    if (std::find_if(alteredList.begin(), alteredList.end(), foundDeletedOrganization) != alteredList.end())
    {
        std::cerr << "The wrong Organization was deleted. TEST FAILED\n";
        return TESTFAILED;
    }

    OrganizationList deletedList = organizationQueryProcessor.getAllDeletedOrganizations();
    if (deletedList.empty())
    {
        std::cerr << testFuncName << ": No Deleted organizations found!! FAILED!!" << std::endl;
        return TESTFAILED;
    }

    if (std::find_if(deletedList.begin(), deletedList.end(), foundDeletedOrganization) == deletedList.end())
    {
        std::cerr << testFuncName << ": Deleted organization not found in deleted organization list FAILED!!" << std::endl;
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << "Original Organization list size: " << allActiveOrganizations.size() << " Altered organization list size: " << alteredList.size() << "\n";
        std::cout << std::format("Organization ({}) marked Deleted. TEST PASSED\n", organizationToDelete->getOrganizationId());
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testPositivePathSetParentOrganization()
{
    OrganizationQueryProcessor source;
    OrganizationModel_shp parentTrap = source.getOrganizationById(2);

    if (!parentTrap->parentOrganizationHasValue())
    {
        parentTrap->setParentOrganization(parentTrap->getOrganizationId() - 1);
        if (!parentTrap->parentOrganizationHasValue())
        {
            std::cerr << "setParentOrganization() FAILED!!\n";
            return TESTFAILED;
        }
    }

    parentTrap->setParentOrganization(0);
    if (parentTrap->parentOrganizationHasValue())
    {
        std::cerr << "setParentOrganization(0) FAILED!!\n";
        return TESTFAILED;
    }

    if (programOptions.verboseOutput)
    {
        std::cout << "TestOrganizationModel::" << __func__ << " Test PASSED\n";
    }

    return TESTPASSED;
}

TestStatus TestOrganizationModel::testNegativePathSetParentOrganization()
{
    try {
        OrganizationQueryProcessor source;
        OrganizationModel_shp errorGenerator = source.getOrganizationById(1);
        errorGenerator->setParentOrganization(errorGenerator->getOrganizationId());
    }
    catch (std::out_of_range &expectedError)
    {
        if (programOptions.verboseOutput)
        {
            std::cout << std::format("setParentOrganization() threw expected error: {} TEST PASSED\n", expectedError.what());
        }
        return TESTPASSED;
    };

    return TESTFAILED;
}

TestStatus TestOrganizationModel::testNegativePathAlreadyInDataBase()
{
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationModel_shp alreadyInDatabase = organizationQueryProcessor.getOrganizationById(2);

    if (!alreadyInDatabase->isInDataBase())
    {
        std::cerr << std::format("TestOrganizationModel::{} Organization 1 not found in database!!\n", __func__);
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors = {"already in Database"};
    return testInsertionFailureMessages(alreadyInDatabase, expectedErrors);
}

TestStatus TestOrganizationModel::testnegativePathNotModified()
{
    OrganizationQueryProcessor organizationQueryProcessor;
    OrganizationModel_shp OrgNotModified = organizationQueryProcessor.getOrganizationById(1);
    if (!OrgNotModified->isInDataBase())
    {
        std::cerr << "Organization 1 not found in database!! FAILED!\n";
        return TESTFAILED;
    }

    OrgNotModified->setOrganizationId(0); // Force it to check modified rather than Already in DB.
    OrgNotModified->clearModified();
    std::vector<std::string> expectedErrors = {"not modified!"};
    return testInsertionFailureMessages(OrgNotModified, expectedErrors);
}

