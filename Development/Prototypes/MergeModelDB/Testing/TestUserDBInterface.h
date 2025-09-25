#ifndef TESTUSERDBINTERFACE_H_
#define TESTUSERDBINTERFACE_H_

#include <functional>
#include <string>
#include "TestDBInterfaceCore.h"
#include "UserList.h"
#include "UserModel.h"
#include <vector>

class TestUserDBInterface : public TestDBInterfaceCore
{
public:
    TestUserDBInterface(std::string userFileName);
    ~TestUserDBInterface() = default;
    virtual TestDBInterfaceCore::TestStatus runPositivePathTests() override;

private:
    bool testGetUserByLoginName(UserModel_shp insertedUser);
    bool testGetUserByLoginAndPassword(UserModel_shp insertedUser);
    bool testGetUserByFullName(UserModel_shp insertedUser);
    bool testUpdateUserPassword(UserModel_shp insertedUser);
    bool loadTestUsersFromFile(UserListValues& userProfileTestData);
    bool testGetAllUsers(UserListValues userProfileTestData);
    TestDBInterfaceCore::TestStatus negativePathMissingRequiredFields();
    void addFirstUser(UserListValues& TestUsers);
    TestDBInterfaceCore::TestStatus testnegativePathNotModified();
    TestDBInterfaceCore::TestStatus testNegativePathAlreadyInDataBase();

    std::string dataFileName;
    bool verboseOutput;
    std::vector<std::function<bool(UserModel_shp)>>positiveTestFuncs;
    std::vector<std::function<bool(void)>> negativePathTestFuncs;
};

#endif // TESTUSERDBINTERFACE_H_

