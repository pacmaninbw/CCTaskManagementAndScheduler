#ifndef ORGANIZATIONQUERYPROCESSOR_H_
#define ORGANIZATIONQUERYPROCESSOR_H_

// Project Header Files
#include "OrganizationModel.h"
#include "QueryProcessor.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>


// Standard C++ Header Files
#include <chrono>
#include <string>
#include <vector>

using OrganizationList = std::vector<OrganizationModel_shp>;
using StaticQueryOrganization = boost::mysql::static_results<boost::mysql::pfr_by_name<OrganizationDbQueryValues>>;

class OrganizationQueryProcessor : public QueryProcessor<OrganizationModel, OrganizationDbQueryValues>
{
public:
    OrganizationQueryProcessor();
    ~OrganizationQueryProcessor();
    OrganizationList getAllOrganizations() noexcept;
    OrganizationList findOrganizationsByName(std::string orgName) noexcept;
    OrganizationList findOrganizationsByPrimaryContactID(std::size_t userId) noexcept;
    OrganizationList findOrganizationsByPrimaryContactName(std::string firstName, std::string lastName, std::string middleInitial) noexcept;
    OrganizationList getAllOrganizationsAddedBetween(std::chrono::year_month_day startRage, std::chrono::year_month_day endRange) noexcept;
    OrganizationList getAnyOrganizationsAddedOnDate(std::chrono::year_month_day searchDate) noexcept;
    OrganizationList getAnyOrganizationsModifiedOnDate(std::chrono::year_month_day searchDate) noexcept;
    OrganizationModel_shp getOrganizationById(std::size_t id_organization) noexcept;

private:
    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionGetAllOrganizations() noexcept;
    TestStatus testExceptionFindOrganizationsByName() noexcept;
    TestStatus testExceptionFindOrganizationsByPrimaryContactID() noexcept;
    TestStatus testExceptionFindOrganizationsByPrimaryContactName() noexcept;
    TestStatus testExceptionGetAllOrganizationsAddedBetween() noexcept;
    TestStatus testExceptionGetAnyOrganizationsAddedOnDate() noexcept;
    TestStatus testExceptionGetAnyOrganizationsModifiedOnDate() noexcept;
    TestStatus testExceptionGetOrganizationById() noexcept;
};

#endif // ORGANIZATIONQUERYPROCESSOR_H_