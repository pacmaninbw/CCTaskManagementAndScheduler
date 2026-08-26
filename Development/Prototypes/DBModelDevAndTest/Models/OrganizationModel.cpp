// Project Header Files
#include "OrganizationModel.h"
#include "chronoBoostConversions.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <optional>
#include <ranges>
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
    m_primaryContactUser = databaseValues.primary_contact_user;
    m_secondaryContactUser = databaseValues.secondary_contact_user;
    m_addressLine1 = databaseValues.address_line_1;
    m_addressLine2 = databaseValues.address_line_2;
    m_city = databaseValues.city;
    m_stateOrProvince = databaseValues.state_or_province;
    m_postalCode = databaseValues.postal_code;
    m_nation = databaseValues.nation;
}

OrganizationModel::~OrganizationModel()
{

}

void OrganizationModel::setOrganizationId(std::size_t orgId) noexcept
{
    m_modified = true;
    m_primaryKey = orgId;
}

void OrganizationModel::setOrganizationName(std::string& organizationName) noexcept
{
    m_modified = true;
    m_organizationName = organizationName;
}

void OrganizationModel::setEmailAddress(std::string& emailAddress) noexcept
{
    m_modified = true;
    m_email = emailAddress;
}

void OrganizationModel::setPhoneNumber(std::string& phoneNumber) noexcept
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

void OrganizationModel::setAddressLine1(std::string& streetAddress) noexcept
{
    m_modified = true;
    m_addressLine1 = streetAddress;
}

void OrganizationModel::setAddressLine2(std::string& streetAddress) noexcept
{
    m_modified = true;
    m_addressLine2 = streetAddress;
}

void OrganizationModel::setCity(std::string& city) noexcept
{
    m_modified = true;
    m_city = city;
}

void OrganizationModel::setPostalCode(std::string& postalCode) noexcept
{
    m_modified = true;
    m_postalCode = postalCode;
}

void OrganizationModel::setStateOrProvince(std::string& stateOrProvince) noexcept
{
    m_modified = true;
    m_stateOrProvince = stateOrProvince;
}

void OrganizationModel::setNation(std::string& nation) noexcept
{
    m_modified = true;
    m_nation = nation;
}

void OrganizationModel::setCreationTimeStamp(std::chrono::system_clock::time_point created) noexcept
{
    m_modified = true;
    m_created = created;
}

void OrganizationModel::setLastModified(std::chrono::system_clock::time_point lastModified) noexcept
{
    m_modified = true;
    m_lastModified = lastModified;
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
    m_missingRequiredFieldsTests.push_back({std::bind(&OrganizationModel::isMissingEmail, this), "Organization Email Address"});
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
        m_created == other.m_created
    );
}

std::string OrganizationModel::formatInsertStatement()
{
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
    boost::mysql::format_sql_to(fctx, "nation");
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
    boost::mysql::format_sql_to(fctx, "{}", m_nation);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string OrganizationModel::formatUpdateStatement()
{
    return std::string();
}

std::string OrganizationModel::formatDeleteStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_notes SET organization_profile.deleted = 1 ");
    boost::mysql::format_sql_to(fctx, "WHERE organization_profile.id_organization = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}
