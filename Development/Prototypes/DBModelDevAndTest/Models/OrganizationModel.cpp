// Project Header Files
#include "chronoBoostConversions.h"
#include "commonTestValues.h"
#include "OrganizationModel.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

OrganizationModel::OrganizationModel()
: ModelDBInterface("Organization", "id_organization")
{

}

OrganizationModel::OrganizationModel(const OrganizationDbQueryValues &databaseValues)
: OrganizationModel()
{
    m_primaryKey = databaseValues.id_organization;
    m_organizationName = databaseValues.organization_name;
    m_email = databaseValues.email_address;
    m_phoneNumber = databaseValues.phone_number;
    m_primaryContactUser = databaseValues.primary_contact_user.value_or(0);
    m_secondaryContactUser = databaseValues.secondary_contact_user.value_or(0);
    m_addressLine1 = databaseValues.address_line_1;
    m_addressLine2 = databaseValues.address_line_2;
    m_city = databaseValues.city;
    m_stateOrProvince = databaseValues.state_or_province;
    m_postalCode = databaseValues.postal_code;
    m_nation = databaseValues.nation;
    m_parentOrganization = databaseValues.parent_organization;
    m_createdTimeStamp= common::toChronoTimePoint(databaseValues.created_timestamp);
    m_lastUpdateTimeStamp = common::toChronoTimePoint(databaseValues.last_modified_time_stamp);
    m_lastModifiedByUser = databaseValues.last_modified_by_user.value_or(0);
}

void OrganizationModel::setOrganizationName(std::string organizationName) noexcept
{
    m_modified = true;
    m_organizationName = organizationName;
}

void OrganizationModel::setEmailAddress(std::string emailAddress) noexcept
{
    m_modified = true;
    m_email = emailAddress;
}

void OrganizationModel::setPhoneNumber(std::string phoneNumber) noexcept
{
    m_modified = true;
    m_phoneNumber = phoneNumber;
}

void OrganizationModel::setPrimaryContactUserId(std::size_t userId) noexcept
{
    m_modified = true;
    m_primaryContactUser = userId;
}

void OrganizationModel::setSecondaryContactUserId(std::size_t userId) noexcept
{
    m_modified = true;
    m_secondaryContactUser = userId;
}

void OrganizationModel::setAddressLine1(std::string streetAddress) noexcept
{
    m_modified = true;
    m_addressLine1 = streetAddress;
}

void OrganizationModel::setAddressLine2(std::string streetAddress) noexcept
{
    m_modified = true;
    m_addressLine2 = streetAddress;
}

void OrganizationModel::setCity(std::string city) noexcept
{
    m_modified = true;
    m_city = city;
}

void OrganizationModel::setPostalCode(std::string postalCode) noexcept
{
    m_modified = true;
    m_postalCode = postalCode;
}

void OrganizationModel::setStateOrProvince(std::string stateOrProvince) noexcept
{
    m_modified = true;
    m_stateOrProvince = stateOrProvince;
}

void OrganizationModel::setNation(std::string nation) noexcept
{
    m_modified = true;
    m_nation = nation;
}

void OrganizationModel::setParentOrganization(std::size_t parentID)
{
    if (m_primaryKey > 0 && parentID == m_primaryKey)
    {
        appendErrorMessage(std::format("In OrganizationModel.{} : {}", __func__, " Parent OrganizationModel ID can't be same as current OrganizationModel id"));
        throw std::out_of_range("Parent OrganizationModel ID can't be same as current Task id");
    }

    // Can be zero to allow movement from parent organization.
    if (parentID > 0)
    {
        m_modified = true;
        m_parentOrganization = parentID;
    }
    else if (parentID == 0 && m_parentOrganization.has_value())
    {
        m_modified = true;
        m_parentOrganization.reset();
    }
}

bool OrganizationModel::isMissingOrganizationName() const noexcept
{
    return m_organizationName.empty();
}

bool OrganizationModel::isMissingEmail() const noexcept
{
    return m_email.empty();
}

bool OrganizationModel::isMissingPrimaryContact() const noexcept
{
    return m_primaryContactUser == 0;
}

bool OrganizationModel::isMissingSecondaryContact() const noexcept
{
    return m_secondaryContactUser == 0;
}

bool OrganizationModel::isMissingPhoneNumber() const noexcept
{
    return m_phoneNumber.empty();
}

void OrganizationModel::initRequiredFields() noexcept
{
    m_missingRequiredFieldsTests.push_back({std::bind(&OrganizationModel::isMissingOrganizationName, this), "Organization Name"});
    m_missingRequiredFieldsTests.push_back({std::bind(&OrganizationModel::isMissingEmail, this), "Email Address"});
    m_missingRequiredFieldsTests.push_back({std::bind(&OrganizationModel::isMissingPrimaryContact, this), "Primary Contact"});
    m_missingRequiredFieldsTests.push_back({std::bind(&OrganizationModel::isMissingSecondaryContact, this), "Secondary Contact"});
    m_missingRequiredFieldsTests.push_back({std::bind(&OrganizationModel::isMissingPhoneNumber, this), "Phone Number"});
}

bool OrganizationModel::diffOrganization(OrganizationModel &other) const noexcept
{

    return (m_primaryKey == other.m_primaryKey &&
        m_organizationName == other.m_organizationName &&
        m_email == other.m_email &&
        m_phoneNumber == other.m_phoneNumber &&
        m_primaryContactUser == other.m_primaryContactUser &&
        m_secondaryContactUser == other.m_secondaryContactUser &&
        m_createdTimeStamp == other.m_createdTimeStamp
    );
}

std::string OrganizationModel::formatInsertStatement()
{
    // For the OrganizationModel only the last_modified_by_user field is optional.
    std::optional<std::size_t> lastModifiedByUser;
    if (m_lastModifiedByUser > 0)
    {
        lastModifiedByUser = m_lastModifiedByUser;
    }

    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "INSERT INTO organization_profile(");
    boost::mysql::format_sql_to(fctx, "organization_name, ");
    boost::mysql::format_sql_to(fctx, "email_address, ");
    boost::mysql::format_sql_to(fctx, "phone_number, ");
    boost::mysql::format_sql_to(fctx, "primary_contact_user, ");
    boost::mysql::format_sql_to(fctx, "secondary_contact_user, ");
    boost::mysql::format_sql_to(fctx, "address_line_1, ");
    boost::mysql::format_sql_to(fctx, "address_line_2, ");
    boost::mysql::format_sql_to(fctx, "city, ");
    boost::mysql::format_sql_to(fctx, "state_or_province, ");
    boost::mysql::format_sql_to(fctx, "postal_code, ");
    boost::mysql::format_sql_to(fctx, "nation, ");
    boost::mysql::format_sql_to(fctx, "parent_organization, ");
    boost::mysql::format_sql_to(fctx, "created_timestamp, ");
    boost::mysql::format_sql_to(fctx, "last_modified_time_stamp, ");
    boost::mysql::format_sql_to(fctx, "last_modified_by_user ");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_organizationName);
    boost::mysql::format_sql_to(fctx, "{}, ", m_email);
    boost::mysql::format_sql_to(fctx, "{}, ", m_phoneNumber);
    boost::mysql::format_sql_to(fctx, "{}, ", m_primaryContactUser);
    boost::mysql::format_sql_to(fctx, "{}, ", m_secondaryContactUser);
    boost::mysql::format_sql_to(fctx, "{}, ", m_addressLine1);
    boost::mysql::format_sql_to(fctx, "{}, ", m_addressLine2);
    boost::mysql::format_sql_to(fctx, "{}, ", m_city);
    boost::mysql::format_sql_to(fctx, "{}, ", m_stateOrProvince);
    boost::mysql::format_sql_to(fctx, "{}, ", m_postalCode);
    boost::mysql::format_sql_to(fctx, "{}, ", m_nation);
    boost::mysql::format_sql_to(fctx, "{}, ", m_parentOrganization);
    boost::mysql::format_sql_to(fctx, "{}, ", m_createdTimeStamp.transform(common::toBoostDateTime));
    boost::mysql::format_sql_to(fctx, "{}, ", m_lastUpdateTimeStamp.transform(common::toBoostDateTime));
    boost::mysql::format_sql_to(fctx, "{}", lastModifiedByUser);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string OrganizationModel::formatUpdateStatement()
{
    // For the OrganizationModel only the last_modified_by_user field is optional.
    std::optional<std::size_t> lastModifiedByUser;
    if (m_lastModifiedByUser > 0)
    {
        lastModifiedByUser = m_lastModifiedByUser;
    }

    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE organization_profile SET ");
    boost::mysql::format_sql_to(fctx, "organization_profile.organization_name = {}, ", m_organizationName);
    boost::mysql::format_sql_to(fctx, "organization_profile.email_address = {}, ", m_email);
    boost::mysql::format_sql_to(fctx, "organization_profile.phone_number = {}, ", m_phoneNumber);
    boost::mysql::format_sql_to(fctx, "organization_profile.primary_contact_user = {}, ", m_primaryContactUser);
    boost::mysql::format_sql_to(fctx, "organization_profile.secondary_contact_user = {}, ", m_secondaryContactUser);
    boost::mysql::format_sql_to(fctx, "organization_profile.address_line_1 = {}, ", m_addressLine1);
    boost::mysql::format_sql_to(fctx, "organization_profile.address_line_2 = {}, ", m_addressLine2);
    boost::mysql::format_sql_to(fctx, "organization_profile.city = {}, ", m_city);
    boost::mysql::format_sql_to(fctx, "organization_profile.postal_code = {}, ", m_postalCode);
    boost::mysql::format_sql_to(fctx, "organization_profile.state_or_province = {}, ", m_stateOrProvince);
    boost::mysql::format_sql_to(fctx, "organization_profile.nation = {}, ", m_nation);
    boost::mysql::format_sql_to(fctx, "organization_profile.parent_organization = {}, ", m_parentOrganization);
    boost::mysql::format_sql_to(fctx, "organization_profile.deleted = {}, ", m_deleted);
    boost::mysql::format_sql_to(fctx, "organization_profile.created_timestamp = {}, ", m_createdTimeStamp.transform(common::toBoostDateTime));
    boost::mysql::format_sql_to(fctx, "organization_profile.last_modified_by_user = {} ", lastModifiedByUser);
    boost::mysql::format_sql_to(fctx, "WHERE organization_profile.id_organization = {} ", m_primaryKey);

    return (std::move(fctx).get().value());
}

std::string OrganizationModel::formatDeleteStatement()
{
    // For the OrganizationModel only the last_modified_by_user field is optional.
    std::optional<std::size_t> lastModifiedByUser = m_lastModifiedByUser > 0 ? m_lastModifiedByUser : 1;

    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE organization_profile SET organization_profile.deleted = 1, ");
    boost::mysql::format_sql_to(fctx, "organization_profile.last_modified_by_user = {} ", lastModifiedByUser);
    boost::mysql::format_sql_to(fctx, "WHERE organization_profile.id_organization = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}

void OrganizationModel::selfTestResetAllValues() noexcept
{
    ModelDBInterface::selfTestResetAllValues();

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

TestStatus OrganizationModel::testAllInsertFailures()
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

    for (const auto& setField: fieldSettings)
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
        std::cout << std::format("{}::{} before successful insert this = \n", getModelName(), __func__) << *this << "\n";
    }

    setCreatedTimeStamp(common::TestTimeStampValue);
    if (!insert())
    {
        std::cout << "In  OrganizationModel::testAllInsertFailures() Expected successful insert failed\n" << m_errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus OrganizationModel::testEqualityOperator() noexcept
{
    OrganizationModel other;

    other.setOrganizationId(m_primaryKey);
    if (*this == other)
    {
        return TESTFAILED;
    }

    if (!m_forceError)
    {
        other = *this;
    }
    
    return (*this == other)? TESTPASSED: TESTFAILED;
}

void OrganizationModel::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

std::vector<AttributeTestFunction> OrganizationModel::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();
    std::vector<AttributeTestFunction> attributeAccessTests;

    attributeAccessTests.push_back({std::bind(&OrganizationModel::testOrganizationIdAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testOrganizationNameAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testEmailAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testPhoneAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testPrimaryContactAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testSecondaryContactAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testAddressLine1Access, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testAddressLine2Access, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testCityAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testPostalCodeAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testStateOrProvinceAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testNationAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testParentOrganizationAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testCreatedDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testLastModifiedDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&OrganizationModel::testLastModifiedByUserAccess, this)});

    return attributeAccessTests;
}

TestStatus OrganizationModel::testOrganizationIdAccess() noexcept
{
    std::size_t testPrimaryKey = 1;

    return testPrimaryKeyAccessFunctions(testPrimaryKey,
        std::bind(&OrganizationModel::setOrganizationId, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getOrganizationId, this));
}

TestStatus OrganizationModel::testOrganizationNameAccess() noexcept
{
    std::string testName("Any organization name up to 255 characters, Inc.");
    return testAccessorFunctions<std::string>(testName, &m_organizationName, "Organization Name",
        std::bind(&OrganizationModel::setOrganizationName, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getOrganizationName, this));
}

TestStatus OrganizationModel::testEmailAccess() noexcept
{
    std::string testEmail("Any_organization_email_up_to255-characters@gmail.com");
    return testAccessorFunctions<std::string>(testEmail, &m_email, "Organization Email",
        std::bind(&OrganizationModel::setEmailAddress, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getEmailAddress, this));
}

TestStatus OrganizationModel::testPhoneAccess() noexcept
{
    std::string testPhoneNumber("Any_organization_email_up_to255-characters@gmail.com");
    return testAccessorFunctions<std::string>(testPhoneNumber, &m_phoneNumber, "Organization Phone Number",
        std::bind(&OrganizationModel::setPhoneNumber, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getPhoneNumber, this));
}

TestStatus OrganizationModel::testPrimaryContactAccess() noexcept
{
    std::size_t primaryContactUserId = 1;
    return testForeignKeyFields(primaryContactUserId, &m_primaryContactUser, "Primary Contact ID",
        std::bind(&OrganizationModel::setPrimaryContactUserId, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getPrimaryContactUserId, this));
}

TestStatus OrganizationModel::testSecondaryContactAccess() noexcept
{
    std::size_t secondaryContactUserId = 1;
    return testForeignKeyFields(secondaryContactUserId, &m_secondaryContactUser, "Secondary Contact ID",
        std::bind(&OrganizationModel::setSecondaryContactUserId, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getSecondaryContactUserId, this));
}

TestStatus OrganizationModel::testAddressLine1Access() noexcept
{
    std::string testValue("Any street address up to 255 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_addressLine1, "Address Line 1",
        std::bind(&OrganizationModel::setAddressLine1, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getAddressLine1, this));
}

TestStatus OrganizationModel::testAddressLine2Access() noexcept
{
    std::string testValue("Any additional street address up to 255 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_addressLine2, "Address Line 2",
        std::bind(&OrganizationModel::setAddressLine2, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getAddressLine2, this));
 }

TestStatus OrganizationModel::testCityAccess() noexcept
{
    std::string testValue("Any city name up to 127 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_city, "City",
        std::bind(&OrganizationModel::setCity, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getCity, this));
}

TestStatus OrganizationModel::testPostalCodeAccess() noexcept
{
    std::string testValue("Any postal code up to 31 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_postalCode, "Postal Code",
        std::bind(&OrganizationModel::setPostalCode, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getPostalCode, this));
}

TestStatus OrganizationModel::testStateOrProvinceAccess() noexcept
{
    std::string testValue("Any state or province up to 255 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_stateOrProvince, "State or Province",
        std::bind(&OrganizationModel::setStateOrProvince, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getStateOrProvince, this));
}

TestStatus OrganizationModel::testNationAccess() noexcept
{
    std::string testValue("Any nation up to 63 characters.");
    return testOptionalAccessorFunctions<std::string>(testValue, &m_nation, "Nation",
        std::bind(&OrganizationModel::setNation, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getNation, this));
}

TestStatus OrganizationModel::testParentOrganizationAccess() noexcept
{
    std::size_t testValue = 23;
    return testOptionalAccessorFunctions<std::size_t>(testValue, &m_parentOrganization, "Parent Organization",
        std::bind(&OrganizationModel::setParentOrganization, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getParentOrganization, this));
}


TestStatus OrganizationModel::testCreatedDateAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_createdTimeStamp, "Creation TimeStamp",
        std::bind(&OrganizationModel::setCreatedTimeStamp, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getCreatedTSValue, this));
}

TestStatus OrganizationModel::testLastModifiedDateAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_lastUpdateTimeStamp, "Last Modified TimeStamp",
        std::bind(&OrganizationModel::setLastModifiedTimeStamp, this, std::placeholders::_1),
        std::bind(&OrganizationModel::getLastModifiedValue, this));
}

TestStatus OrganizationModel::testLastModifiedByUserAccess() noexcept
{
    std::size_t testUserId = 1;

    return testLastModifiedByAccess(testUserId);
}

std::vector<ExceptionTestElement> OrganizationModel::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&OrganizationModel::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&OrganizationModel::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&OrganizationModel::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}

TestStatus OrganizationModel::testExceptionInsert() noexcept
{
    selfTestResetAllValues();

    initValidTestValues();

    return testExceptionAndSuccessNArgs("OrganizationModel::insert", std::bind(&OrganizationModel::insert, this));
}

TestStatus OrganizationModel::testExceptionUpdate() noexcept
{
    selfTestResetAllValues();

    initValidTestValues();
    setOrganizationId(1);

    return testExceptionAndSuccessNArgs("OrganizationModel::update", std::bind(&OrganizationModel::update, this));
}

TestStatus OrganizationModel::testExceptionHide() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;

    setOrganizationId(testUserId);

    return testExceptionAndSuccessNArgs("OrganizationModel::hide", std::bind(&OrganizationModel::hide, this, std::placeholders::_1), testUserId);
}

void OrganizationModel::initValidTestValues() noexcept
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
    setCreatedTimeStamp(timeStamp);
    setLastModifiedTimeStamp(timeStamp);
}


