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
