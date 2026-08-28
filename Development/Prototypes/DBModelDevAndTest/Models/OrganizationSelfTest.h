#ifndef ORGANIZATIONSELFTEST_H_
#define ORGANIZATIONSELFTEST_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelSelfTest.h"
#include "OrganizationModel.h"

// Standard C++ Header Files
#include <functional>
#include <memory>
#include <vector>

class OrganizationSelfTest : public ModelSelfTest<OrganizationModel>
{
public:
    OrganizationSelfTest();
    ~OrganizationSelfTest();
    virtual TestStatus runSelfTest() noexcept override;

private:

// Unit test functions
    virtual void selfTestResetAllValues() noexcept override;
    virtual std::vector<AttributeTestFunction> initAttributeAccessTests() noexcept override;
    TestStatus testOrganizationIdAccess() noexcept;
    TestStatus testOrganizationNameAccess() noexcept;
    TestStatus testEmailAccess() noexcept;
    TestStatus testPhoneAccess() noexcept;
    TestStatus testPrimaryContactAccess() noexcept;
    TestStatus testSecondaryContactAccess() noexcept;
    TestStatus testAddressLine1Access() noexcept;
    TestStatus testAddressLine2Access() noexcept;
    TestStatus testCityAccess() noexcept;
    TestStatus testPostalCodeAccess() noexcept;
    TestStatus testStateOrProvinceAccess() noexcept;
    TestStatus testNationAccess() noexcept;
    TestStatus testCreatedDateAccess() noexcept;
    TestStatus testLastModifiedDateAccess() noexcept;

    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept override;
    virtual TestStatus testExceptionInsert() noexcept override;
    virtual TestStatus testExceptionUpdate() noexcept override;
    virtual TestStatus testExceptionHide() noexcept override;
    void initValidTestValues() noexcept;

    virtual TestStatus testAllInsertFailures() override;
    virtual TestStatus testEqualityOperator() noexcept override;
    virtual void testOutput() noexcept override;
    void convertOrganizationModelTo(OrganizationModel_shp original, OrganizationDbQueryValues& testInput) noexcept;
    TestStatus testDataBaseToModelConversion() noexcept;
};


using OrganizationSelfTest_shp = std::shared_ptr<OrganizationSelfTest>;

#endif  // ORGANIZATIONSELFTEST_H_
