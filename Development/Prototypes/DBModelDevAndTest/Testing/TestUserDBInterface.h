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
    virtual bool databaseObjectInitAfterConstructor() override { return true; };
    bool testGetUserByLoginName(UserModel_shp insertedUser) const;
    bool testGetUserByLoginAndPassword(UserModel_shp insertedUser) const;
    bool testGetUserByFullName(UserModel_shp insertedUser) const;
    bool testGetUserByEmail(UserModel_shp insertedUser) const;
    bool testUpdateUserPassword(UserModel_shp insertedUser) const;
    bool loadTestUsersFromFile();
    bool testGetAllUsers();
    bool testGetAllUsersAddedOnDate() noexcept;
    bool testGetAllUsersDeletedOnDate() noexcept;
    bool testGetAllUsersFrom() noexcept;
    bool compareListReturnedToUserProfileTestData(UserModelList testData, std::string funcName) noexcept;
    bool runPositivePathListQueries() noexcept ;
    TestStatus testnegativePathNotModified() const;
    TestStatus testNegativePathAlreadyInDataBase() const;
    void deleteSomeUsers();

    std::string m_dataFileName;
    std::vector<std::function<bool(UserModel_shp)>> m_positiveTestFuncs;
    UserModelList m_userProfileTestData;
    UserModelList m_deletedUsers;
};

#endif // TESTUSERDBINTERFACE_H_

