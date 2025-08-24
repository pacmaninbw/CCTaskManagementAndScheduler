#ifndef TESTUSERDBINTERFACE_H_
#define TESTUSERDBINTERFACE_H_

#include <functional>
#include <string>
#include "UserDbInterface.h"
#include "UserModel.h"
#include <vector>

class TestUserDBInterface
{
public:
    TestUserDBInterface(std::string userFileName);
    ~TestUserDBInterface() = default;
    bool runAllTests();

private:
    bool negativePathTests();
    bool positivePathTests();
    bool testGetUserByLoginName(UserModel_shp insertedUser);
    bool testGetUserByLoginAndPassword(UserModel_shp insertedUser);
    bool testGetUserByFullName(UserModel_shp insertedUser);
    bool testUpdateUserPassword(UserModel_shp insertedUser);
    bool loadTestUsersFromFile(UserList& userProfileTestData);
    bool testGetAllUsers(UserList userProfileTestData);
    bool testMissingRequiredField(UserModel& userMissingField, std::vector<std::string>& expectedErrors);
    bool wrongErrorMessage(std::string expectedString);
    bool hasErrorMessage();
    bool insertionWasSuccessfull(std::size_t userID, std::string logMessage);
    bool negativePathMissingRequiredFields();
    void addFirstUser(UserList& TestUsers);

    UserDbInterface userDBInterface;
    std::string dataFileName;
    bool verboseOutput;
    std::vector<std::function<bool(UserModel_shp)>>positiveTestFuncs;
    std::vector<std::function<bool(void)>> negativePathTestFuncs;
};

#endif // TESTUSERDBINTERFACE_H_

