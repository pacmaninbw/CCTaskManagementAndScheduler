#ifndef TESTUSERDBINTERFACE_H_
#define TESTUSERDBINTERFACE_H_

// Project Header Files
#include "TestDBInterfaceCore.h"
#include "UserQueryProcessor.h"
#include "UserModel.h"

// StandardS C++ Header Files
#include <functional>
#include <string>
#include <vector>

class TestUserDBInterface : public TestDBInterfaceCore
{
public:
    TestUserDBInterface(std::string userFileName);
    ~TestUserDBInterface() = default;
    virtual TestStatus runPositivePathTests() override;

private:
    bool testGetUserByLoginName(UserModel_shp insertedUser);
    bool testGetUserByLoginAndPassword(UserModel_shp insertedUser);
    bool testGetUserByFullName(UserModel_shp insertedUser);
    bool testGetUserByEmail(UserModel_shp insertedUser);
    bool testUpdateUserPassword(UserModel_shp insertedUser);
    bool loadTestUsersFromFile();
    bool testGetAllUsers();
    bool testGetAllUsersAddedOnDate() noexcept;
    bool testGetAllUsersDeletedOnDate() noexcept;
    bool testGetAllUsersFrom() noexcept;
    bool compareListReturnedToUserProfileTestData(UserModelList testData, std::string funcName) noexcept;
    bool runPositivePathListQueries() noexcept ;
    TestStatus testnegativePathNotModified();
    TestStatus testNegativePathAlreadyInDataBase();
    void deleteSomeUsers();

    std::string m_dataFileName;
    std::vector<std::function<bool(UserModel_shp)>> m_positiveTestFuncs;
    UserModelList m_userProfileTestData;
    UserModelList m_deletedUsers;
};

#endif // TESTUSERDBINTERFACE_H_

