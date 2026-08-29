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

private:

};

#endif // ORGANIZATIONQUERYPROCESSOR_H_