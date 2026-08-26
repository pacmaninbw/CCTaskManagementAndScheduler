// Project Header Files
#include "commonUtilities.h"
#include "OrganizationSelfTest.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

OrganizationSelfTest::OrganizationSelfTest()
{

}

OrganizationSelfTest::~OrganizationSelfTest()
{

}

TestStatus OrganizationSelfTest::runSelfTest() noexcept
{
    m_selfTest = true;
    TestStatus selfTestStatus = TESTPASSED;

    std::cout << "\nRunning " << m_modelName << " Self Test\n";

    if (testExceptionHandling()!= TESTPASSED)
    {
        std::cerr  << m_modelName << "::runSelfTest: Exception handling FAILED!\n";
        selfTestStatus = TESTFAILED;
    }
    
    if (testSave() == TESTFAILED)
    {
        selfTestStatus = TESTFAILED;
    }

    if (testAttributeAccessFunctions() == TESTFAILED)
    {
        std::cerr << m_modelName << "::runSelfTest: One or more get or set functions FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testEqualityOperator() == TESTFAILED)
    {
        std::cerr << std::format("Equality Operator Test: Comparing 2 {}s FAILED!\n", m_modelName);
        selfTestStatus = TESTFAILED;
    }

    testOutput();

    if (testAllInsertFailures() != TESTPASSED)
    {
        std::cerr << "Test of all insertion failures FAILED!\n";
        selfTestStatus = TESTFAILED;
    }

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Insertion Failure Test PASSED!\n";
    }

    if (testCommonUpdateFailurePath() != TESTPASSED)
    {
        selfTestStatus = TESTFAILED;
    }
    else
    {
        std::cout << "Common Update Failure Test PASSED!\n";
    }

    m_selfTest = false;
    
    if (selfTestStatus == TESTPASSED)
    {
        std::cout <<  std::format("{} Self Test {}\n", m_modelName, "PASSED");
    }
    else
    {
        std::cerr <<  std::format("{} Self Test {}\n", m_modelName, "FAILED");
    }

    return selfTestStatus;
}

void OrganizationSelfTest::selfTestResetAllValues() noexcept
{
    ModelSelfTest::selfTestResetAllValues();

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

std::vector<AttributeTestFunction> OrganizationSelfTest::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();
    std::vector<AttributeTestFunction> attributeAccessTests;

    return attributeAccessTests;
}

std::vector<ExceptionTestElement> OrganizationSelfTest::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&OrganizationSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&OrganizationSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&OrganizationSelfTest::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}

TestStatus OrganizationSelfTest::testExceptionInsert() noexcept
{
    return TESTFAILED;
}

TestStatus OrganizationSelfTest::testExceptionUpdate() noexcept
{
    return TESTFAILED;
}

TestStatus OrganizationSelfTest::testExceptionHide() noexcept
{
    return TESTFAILED;
}

TestStatus OrganizationSelfTest::testAllInsertFailures()
{
    return TESTFAILED;
}

TestStatus OrganizationSelfTest::testEqualityOperator() noexcept
{
    OrganizationModel other;

    other.setOrganizationId(m_primaryKey);
    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;
    
    return (*this == other)? TESTPASSED: TESTFAILED;
}

void OrganizationSelfTest::testOutput() noexcept
{
}
