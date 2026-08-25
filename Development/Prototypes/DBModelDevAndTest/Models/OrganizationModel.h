#ifndef ORGANIZATIONMODEL_H_
#define ORGANIZATIONMODEL_H_

// Project Header Files
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

struct OrganizationDbQueryValues
{
    std::uint64_t id_organization;
    std::string organization_name;
    std::string email_address;
    std::string phone_number;
    std::uint64_t primary_contact_user;
    std::uint64_t secondary_contact_user;
    std::optional<std::string> address_line_1;
    std::optional<std::string> address_line_2;
    std::optional<std::string> city;
    std::optional<std::string> postal_code;
    std::optional<std::string> state_or_province;
    std::optional<std::string> nation;
    boost::mysql::datetime created_timestamp;
    boost::mysql::datetime last_modified_time_stamp;
    std::int64_t deleted;
};

class OrganizationModel : public ModelDBInterface
{
public:
    OrganizationModel();
    OrganizationModel(const OrganizationDbQueryValues& databaseValues);
    ~OrganizationModel();

    bool operator==(OrganizationModel& other) { return diffOrganization(other); };

    friend std::ostream& operator<<(std::ostream& os, const OrganizationModel& orgProfile)
    {
        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << std::format(outFmtStr, "Organization ID", orgProfile.m_primaryKey);
        os << std::format(outFmtStr, "Organization Name", orgProfile.m_organizationName);
        os << std::format(outFmtStr, "Email Address", orgProfile.m_email);
        os << std::format(outFmtStr, "Phone Number", orgProfile.m_phoneNumber);
        os << std::format(outFmtStr, "Primary Contact User ID", orgProfile.m_primaryContactUser);
        os << std::format(outFmtStr, "Secondary Contact User ID", orgProfile.m_secondaryContactUser);
        os << std::format(outFmtStr, "Address Line 1", orgProfile.m_addressLine1.value_or(""));
        
        if (orgProfile.m_addressLine2.has_value())
        {
            os << std::format(outFmtStr, "Address Line 1", orgProfile.m_addressLine2.value());
        }

        os << std::format(outFmtStr, "City", orgProfile.m_city.value_or(""));
        os << std::format(outFmtStr, "Postal Code", orgProfile.m_postalCode.value_or(""));
        os << std::format(outFmtStr, "State or Province", orgProfile.m_stateOrProvince.value_or(""));
        os << std::format(outFmtStr, "Nation", orgProfile.m_nation.value_or(""));

        if (orgProfile.m_created.has_value())
        {
            os << std::format(outFmtStr, "Date Added", orgProfile.m_created.value());
        }

        if (orgProfile.m_lastModified.has_value())
        {
            os << std::format(outFmtStr, "Date Added", orgProfile.m_lastModified.value());
        }

        return os;
    };

private:
    bool diffOrganization(OrganizationModel& other) const noexcept;
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatDeleteStatement() override;

    std::string m_organizationName;
    std::string m_email;
    std::string m_phoneNumber;
    std::uint64_t m_primaryContactUser;
    std::uint64_t m_secondaryContactUser;
    std::optional<std::string> m_addressLine1;
    std::optional<std::string> m_addressLine2;
    std::optional<std::string> m_city;
    std::optional<std::string> m_postalCode;
    std::optional<std::string> m_stateOrProvince;
    std::optional<std::string> m_nation;
    std::optional<std::chrono::system_clock::time_point>  m_created;
    std::optional<std::chrono::system_clock::time_point>  m_lastModified;
    std::int64_t deleted;
};

#endif  // ORGANIZATIONMODEL_H_