#ifndef UNIT_TEST_ORGANIZATIONMODEL_H_
#define UNIT_TEST_ORGANIZATIONMODEL_H_

// Project Header Files
#include "OrganizationModel.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_OrganizationModel : public OrganizationModel
{
public:
    using OrganizationModel::m_primaryKey;

};

TEST_CASE("Unit Test OrganizationModel::setOrganizationId") {
    unit_test_OrganizationModel unitTestOrg;

    std::size_t testValue = 1;
    unitTestOrg.setOrganizationId(testValue);
    CHECK(unitTestOrg.m_primaryKey == testValue);
}

#endif // UNIT_TEST_ORGANIZATIONMODEL_H_
