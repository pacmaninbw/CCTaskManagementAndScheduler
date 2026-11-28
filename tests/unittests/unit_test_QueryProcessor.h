#ifndef UNIT_TEST_ORGANIZATIONQUERYPROCESSOR_H_
#define UNIT_TEST_ORGANIZATIONQUERYPROCESSOR_H_

// Project Header Files
#include "OrganizationQueryProcessor.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_OrganizationQueryProcessor : public OrganizationQueryProcessor
{
public:

};

TEST_CASE("Unit Test OrganizationQueryProcessor::setNoteId") {
    unit_test_OrganizationQueryProcessor unitTestOrgQueryProcessor;

    std::size_t testValue = 1;
//    unitTestOrgQueryProcessor.setNoteId(testValue);
//    CHECK(unitTestOrgQueryProcessor.m_primaryKey == testValue);
}

#endif // UNIT_TEST_ORGANIZATIONQUERYPROCESSOR_H_

