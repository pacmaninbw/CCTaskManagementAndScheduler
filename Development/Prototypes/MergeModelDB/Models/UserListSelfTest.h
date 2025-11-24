#ifndef USERLISTSELFTEST_H_
#define USERLISTSELFTEST_H_

// Project Header Files
#include "ListSelfTest.h"
#include "TestStatus.h"
#include "UserList.h"

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

/*
 */
class UserListSelfTest : public ListSelfTest<UserList>
{
public:
    UserListSelfTest();
    virtual ~UserListSelfTest() = default;

    virtual TestStatus runSelfTest() noexcept override;

protected:
    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllUsers() noexcept;
};

#endif // USERLISTSELFTEST_H_


