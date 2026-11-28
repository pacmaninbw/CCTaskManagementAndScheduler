#ifndef UNIT_TEST_USERMODEL_H_
#define UNIT_TEST_USERMODEL_H_

// Project Header Files
#include "UserModel.h"

// External Libraries
#include "doctest.h"

// Standard C++ Header Files

class unit_test_UserModel : public UserModel
{
public:
    using UserModel::m_primaryKey;

};

TEST_CASE("Unit Test UserModel::setUserId") {
    unit_test_UserModel unitTestUser;

    std::size_t testValue = 1;
    unitTestUser.setUserID(testValue);
    CHECK(unitTestUser.m_primaryKey == testValue);
}

#endif // UNIT_TEST_USERMODEL_H_

