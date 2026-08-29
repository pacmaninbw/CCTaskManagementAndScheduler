// Project Header Files
#include "chronoBoostConversions.h"
#include "commonUtilities.h"
#include "OrganizationSelfTest.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

OrganizationSelfTest::OrganizationSelfTest()
{

}

OrganizationSelfTest::~OrganizationSelfTest()
{

}

TestStatus OrganizationSelfTest::runSelfTest() noexcept
{
    m_selfTest = true;
    TestStatus selfTestStatus = TESTPASSED;

    std::cout << "\nRunning " << m_modelName << " Self Test\n";

    if (testExceptionHandling()!= TESTPASSED)
    {
        std::cerr  << m_modelName << "::runSelfTest: Exception handling FAILED!\n";
        selfTestStatus = TESTFAILED;
    }
    
    if (testSave() == TESTFAILED)
    {
        selfTestStatus = TESTFAILED;
    }

    if (testAttributeAccessFunctions() == TESTFAILED)
    {
        std::cerr << m_modelName << "::runSelfTest: One or more get or set functions FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testEqualityOperator() == TESTFAILED)
    {
        std::cerr << std::format("Equality Operator Test: Comparing 2 {}s FAILED!\n", m_modelName);
        selfTestStatus = TESTFAILED;
    }

    testOutput();

    if (testDataBaseToModelConversion() != TESTPASSED)
    {
        std::cerr << std::format("Test new {} from Database Data FAILED!\n", m_modelName);
        selfTestStatus = TESTFAILED;
    }

    if (testAllInsertFailures() != TESTPASSED)
    {
        std::cerr << "Test of all insertion failures FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Insertion Failure Test PASSED!\n";
    }

    if (testCommonUpdateFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Update Failure Test PASSED!\n";
    }

    m_selfTest = false;
    
    if (selfTestStatus == TESTPASSED)
    {
        std::cout <<  std::format("{} Self Test {}\n", m_modelName, "PASSED");
    }
    else
    {
        std::cerr <<  std::format("{} Self Test {}\n", m_modelName, "FAILED");
    }

    return selfTestStatus;
}

void OrganizationSelfTest::selfTestResetAllValues() noexcept
{
    ModelSelfTest::selfTestResetAllValues();

    m_organizationName.clear();
    m_email.clear();
    m_phoneNumber.clear();
    m_primaryContactUser = 0;
    m_secondaryContactUser = 0;
    m_addressLine1.reset();
    m_addressLine2.reset();
    m_city.reset();
    m_stateOrProvince.reset();
    m_postalCode.reset();
    m_nation.reset();
}

std::vector<AttributeTestFunction> OrganizationSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();
    std::vector<AttributeTestFunction> attributeAccessTests;

    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testOrganizationIdAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testOrganizationNameAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testEmailAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testPhoneAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testPrimaryContactAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testSecondaryContactAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testAddressLine1Access, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testAddressLine2Access, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testCityAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testPostalCodeAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testStateOrProvinceAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testNationAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testCreatedDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationSelfTest::testLastModifiedDateAccess, this)});

    return attributeAccessTests;
}

TestStatus OrganizationSelfTest::testOrganizationIdAccess() noexcept
{
    std::size_t testPrimaryKey = 1;

    return testAccessorFunctions<std::size_t>(testPrimaryKey, &m_primaryKey, "Primary Key",
        std::bind(&OrganizationModel::setOrganizationId, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getOrganizationId, this));
}

TestStatus OrganizationSelfTest::testOrganizationNameAccess() noexcept
{
    std::string testName("Any organization name up to 255 characters, Inc.");
    return testAccessorFunctions<std::string>(testName, &m_organizationName, "Organization Name",
        std::bind(&OrganizationModel::setOrganizationName, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getOrganizationName, this));
}

TestStatus OrganizationSelfTest::testEmailAccess() noexcept
{
    std::string testEmail("Any_organization_email_up_to255-characters@gmail.com");
    return testAccessorFunctions<std::string>(testEmail, &m_email, "Organization Email",
        std::bind(&OrganizationModel::setEmailAddress, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getEmailAddress, this));
}

TestStatus OrganizationSelfTest::testPhoneAccess() noexcept
{
    std::string testPhoneNumber("Any_organization_email_up_to255-characters@gmail.com");
    return testAccessorFunctions<std::string>(testPhoneNumber, &m_phoneNumber, "Organization Phone Number",
        std::bind(&OrganizationModel::setPhoneNumber, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getPhoneNumber, this));
}

TestStatus OrganizationSelfTest::testPrimaryContactAccess() noexcept
{
    std::size_t primaryContactUserId = 1;
    return testAccessorFunctions<std::size_t>(primaryContactUserId, &m_primaryContactUser, "Primary Contact ID",
        std::bind(&OrganizationModel::setPrimaryContactUserId, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getPrimaryContactUserId, this));
}

TestStatus OrganizationSelfTest::testSecondaryContactAccess() noexcept
{
    std::size_t secondaryContactUserId = 1;
    return testAccessorFunctions<std::size_t>(secondaryContactUserId, &m_secondaryContactUser, "Secondary Contact ID",
        std::bind(&OrganizationModel::setSecondaryContactUserId, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getSecondaryContactUserId, this));
}

TestStatus OrganizationSelfTest::testAddressLine1Access() noexcept
{
    std::string testValue("Any street address up to 255 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_addressLine1, "Address Line 1",
        std::bind(&OrganizationModel::setAddressLine1, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getAddressLine1, this));
}

TestStatus OrganizationSelfTest::testAddressLine2Access() noexcept
{
    std::string testValue("Any additional street address up to 255 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_addressLine2, "Address Line 2",
        std::bind(&OrganizationModel::setAddressLine2, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getAddressLine2, this));
 }

TestStatus OrganizationSelfTest::testCityAccess() noexcept
{
    std::string testValue("Any city name up to 127 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_city, "City",
        std::bind(&OrganizationModel::setCity, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getCity, this));
}

TestStatus OrganizationSelfTest::testPostalCodeAccess() noexcept
{
    std::string testValue("Any postal code up to 31 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_postalCode, "Postal Code",
        std::bind(&OrganizationModel::setPostalCode, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getPostalCode, this));
}

TestStatus OrganizationSelfTest::testStateOrProvinceAccess() noexcept
{
    std::string testValue("Any state or province up to 255 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_stateOrProvince, "State or Province",
        std::bind(&OrganizationModel::setStateOrProvince, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getStateOrProvince, this));
}

TestStatus OrganizationSelfTest::testNationAccess() noexcept
{
    std::string testValue("Any nation up to 63 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_nation, "Nation",
        std::bind(&OrganizationModel::setNation, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getNation, this));
}

TestStatus OrganizationSelfTest::testCreatedDateAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_created, "Creation TimeStamp",
        std::bind(&OrganizationModel::setCreationTimeStamp, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getCreationTimeStamp, this));
}

TestStatus OrganizationSelfTest::testLastModifiedDateAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_created, "Last Modified TimeStamp",
        std::bind(&OrganizationModel::setLastModified, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getLastModified, this));
}

std::vector<ExceptionTestElement> OrganizationSelfTest::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&OrganizationSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&OrganizationSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&OrganizationSelfTest::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}

TestStatus OrganizationSelfTest::testExceptionInsert() noexcept
{
    selfTestResetAllValues();

    initValidTestValues();

    return testExceptionAndSuccessNArgs("OrganizationModel::insert", std::bind(&OrganizationModel::insert, this));
}

TestStatus OrganizationSelfTest::testExceptionUpdate() noexcept
{
    selfTestResetAllValues();

    initValidTestValues();
    setOrganizationId(1);

    return testExceptionAndSuccessNArgs("OrganizationModel::update", std::bind(&OrganizationModel::update, this));
}

TestStatus OrganizationSelfTest::testExceptionHide() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;

    setOrganizationId(testUserId);

    return testExceptionAndSuccessNArgs("OrganizationModel::hide", std::bind(&OrganizationModel::hide, this, std::placeholders::_1), testUserId);
}

void OrganizationSelfTest::initValidTestValues() noexcept
{
    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;

    setOrganizationName("Any Organization .Inc");
    setEmailAddress("AnyEmailAddress@gmail.com");
    setPhoneNumber("800-555-1212");
    setPrimaryContactUserId(1);
    setSecondaryContactUserId(2);
    setAddressLine1("11111 First Street");
    setAddressLine2("Suite 101");
    setCity("Home Town");
    setStateOrProvince("California");
    setPostalCode("90001");
    setNation("United States");
    setCreationTimeStamp(timeStamp);
    setLastModified(timeStamp);
}

TestStatus OrganizationSelfTest::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "Organization Name", "Email Address", "Phone Number", "Primary Contact", "Secondary Contact"
    };

    setOrganizationId(0);   // Force a modification so that missing fields can be tested.

    std::vector<std::function<void(std::string)>> fieldSettings;
    fieldSettings.push_back(std::bind(&OrganizationModel::setOrganizationName, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&OrganizationModel::setEmailAddress, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&OrganizationModel::setPhoneNumber, this, std::placeholders::_1));

    for (auto setField: fieldSettings)
    {
        if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
        {
            return TESTFAILED;
        }
        expectedErrors.erase(expectedErrors.begin());
        setField("teststringvalue");
    }

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setPrimaryContactUserId(1);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setSecondaryContactUserId(2);

    expectedErrors.clear();

    if (m_verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", m_modelName, __func__) << *this << "\n";
    }

    setCreationTimeStamp(common::TestTimeStampValue);
    if (!insert())
    {
        std::cout << "In  OrganizationSelfTest::testAllInsertFailures() Expected successful insert failed\n" << m_errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus OrganizationSelfTest::testEqualityOperator() noexcept
{
    OrganizationModel other;

    other.setOrganizationId(m_primaryKey);
    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;
    
    return (*this == other)? TESTPASSED: TESTFAILED;
}

void OrganizationSelfTest::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

void OrganizationSelfTest::convertOrganizationModelTo(OrganizationModel_shp original, OrganizationDbQueryValues &testInput) noexcept
{
    testInput.id_organization = original->getOrganizationId();
    testInput.organization_name = original->getOrganizationName();
    testInput.email_address = original->getEmailAddress();
    testInput.phone_number = original->getPhoneNumber();
    testInput.primary_contact_user = original->getPrimaryContactUserId();
    testInput.secondary_contact_user = original->getSecondaryContactUserId();
    testInput.address_line_1 = original->getAddressLine1();
    testInput.address_line_2 = original->getAddressLine2();
    testInput.city = original->getCity();
    testInput.state_or_province = original->getStateOrProvince();
    testInput.postal_code = original->getPostalCode();
    testInput.nation = original->getNation();
    testInput.created_timestamp = common::toBoostDateTime(original->getCreationTimeStamp());
    testInput.last_modified_time_stamp = common::toBoostDateTime(original->getLastModified());
}

TestStatus OrganizationSelfTest::testDataBaseToModelConversion() noexcept
{
    OrganizationModel_shp expectData = std::make_shared<OrganizationModel>();
    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;

    expectData->setOrganizationId(1);
    expectData->setOrganizationName("Any Organization .Inc");
    expectData->setEmailAddress("AnyEmailAddress@gmail.com");
    expectData->setPhoneNumber("800-555-1212");
    expectData->setPrimaryContactUserId(1);
    expectData->setSecondaryContactUserId(2);
    expectData->setAddressLine1("11111 First Street");
    expectData->setAddressLine2("Suite 101");
    expectData->setCity("Home Town");
    expectData->setStateOrProvince("California");
    expectData->setPostalCode("90001");
    expectData->setNation("United States");
    expectData->setCreationTimeStamp(timeStamp);
    expectData->setLastModified(timeStamp);

    OrganizationDbQueryValues testInput;
    convertOrganizationModelTo(expectData, testInput);

    OrganizationModel_shp testOutput = std::make_shared<OrganizationModel>(testInput);

    if (*expectData == *testOutput)
    {
        return TESTPASSED;
    }

    std::cerr << "Test Input " << *this << "\n\n Test Output " << *testOutput << std::endl;
    return TESTFAILED;
}

