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
    return TESTFAILED;
}

void OrganizationSelfTest::selfTestResetAllValues() noexcept
{
}

std::vector<AttributeTestFunction> OrganizationSelfTest::initAttributeAccessTests() noexcept
{
    return std::vector<AttributeTestFunction>();
}

std::vector<ExceptionTestElement> OrganizationSelfTest::initExceptionTests() noexcept
{
    return std::vector<ExceptionTestElement>();
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
    return TESTFAILED;
}

void OrganizationSelfTest::testOutput() noexcept
{
}
