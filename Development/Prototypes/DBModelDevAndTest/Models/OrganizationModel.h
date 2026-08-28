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

/*
 * Access member functions
 */
    void setOrganizationId(std::size_t orgId) noexcept;
    std::size_t getOrganizationId() const noexcept { return m_primaryKey; };
    void setOrganizationName(std::string organizationName) noexcept;
    std::string getOrganizationName() const noexcept { return m_organizationName; };
    void setEmailAddress(std::string emailAddress) noexcept;
    std::string getEmailAddress() const noexcept { return m_email; };
    void setPhoneNumber(std::string phoneNumber) noexcept;
    std::string getPhoneNumber() const noexcept { return m_phoneNumber; };
    void setPrimaryContactUserId(std::size_t userId) noexcept;
    std::size_t getPrimaryContactUserId() const noexcept { return m_primaryContactUser; };
    void setSecondaryContactUserId(std::size_t userId) noexcept;
    std::size_t getSecondaryContactUserId() const noexcept { return m_secondaryContactUser; };
    void setAddressLine1(std::string streetAddress) noexcept;
    std::string getAddressLine1() const noexcept { return m_addressLine1.value_or(""); };
    void setAddressLine2(std::string streetAddress) noexcept;
    std::string getAddressLine2() const noexcept { return m_addressLine2.value_or(""); };
    void setCity(std::string city) noexcept;
    std::string getCity() const noexcept { return m_city.value_or("") ;};
    void setPostalCode(std::string postalCode) noexcept;
    std::string getPostalCode() const noexcept { return m_postalCode.value_or(""); };
    void setStateOrProvince(std::string stateOrProvince) noexcept;
    std::string getStateOrProvince() const noexcept { return m_stateOrProvince.value_or(""); };
    void setNation(std::string nation) noexcept;
    std::string getNation() const noexcept { return m_nation.value_or(""); };
    void setCreationTimeStamp(std::chrono::system_clock::time_point created) noexcept;
    std::chrono::system_clock::time_point getCreationTimeStamp() {return m_created.value(); };
    void setLastModified(std::chrono::system_clock::time_point lastModified) noexcept;
    std::chrono::system_clock::time_point getLastModified() { return m_lastModified.value(); };

/*
 * Required fields.
 */
    bool isMissingOrganizationName() const noexcept;
    bool isMissingEmail() const noexcept;
    bool isMissingPrimaryContact() const noexcept;
    bool isMissingSecondaryContact() const noexcept;
    bool isMissingPhoneNumber() const noexcept;
    void initRequiredFields() noexcept override;

    bool operator==(OrganizationModel& other)
    {
        return diffOrganization(other);
    };



    bool operator==(OrganizationModel& other) const { return diffOrganization(other); };

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
            os << std::format(outFmtStr, "Address Line 2", orgProfile.m_addressLine2.value());
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

protected:
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
};

using OrganizationModel_shp = std::shared_ptr<OrganizationModel>;

#endif  // ORGANIZATIONMODEL_H_