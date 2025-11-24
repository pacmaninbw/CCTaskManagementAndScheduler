
// Project Header Files
#include "UserListSelfTest.h"
#include "TestStatus.h"

// Standard C++ Header Files
#include <concepts>
#include <exception>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

UserListSelfTest::UserListSelfTest()
{
    verboseOutput = true;
    inSelfTest = true;
    queryGenerator.setSelfTest(true);
    errorMessages.clear();
}

TestStatus UserListSelfTest::runSelfTest() noexcept
{
    selfTestResetAllValues();

    std::cout << "\nRunning " << itemUnderTest << " Self Test\n";

    return testListExceptionHandling();
}

std::vector<ListExceptionTestElement> UserListSelfTest::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserListSelfTest::testExceptionsGetAllUsers, this), "selectByUserID"});

    return exceptionTests;
}

TestStatus UserListSelfTest::testExceptionsGetAllUsers() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("UserListSelfTest::testExceptionsGetAllUsers()", std::bind(&UserList::getAllUsers, this));
}
