// Project Header Files
#include "commonTestValues.h"
#include "commonUtilities.h"
#include "UserModel.h"
#include "chronoBoostConversions.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

UserModel::UserModel()
: ModelDBInterface("User", "UserID")
{
    m_preferences.includePriorityInSchedule = true;
    m_preferences.includeMinorPriorityInSchedule = true;
    m_preferences.userLetterForMajorPriority = true;
    m_preferences.separateMajorAndMinorWithDot = false;
    m_preferences.startTime = "8:30 AM";
    m_preferences.endTime = "5:00 PM";
}

UserModel::UserModel(const UserDbQueryValues &databaseValues)
: ModelDBInterface("User", "UserID")
{
    m_primaryKey = databaseValues.user_id;
    m_lastName = databaseValues.last_name;
    m_firstName = databaseValues.first_name;
    m_middleInitial = databaseValues.middle_initial.value_or("");
    m_email = databaseValues.email_address;
    m_loginName = databaseValues.user_login;
    m_password = databaseValues.hashed_password;
    parsePrefenceText(databaseValues.preferences);
    m_createdTimeStamp = common::toChronoTimePoint(databaseValues.created_timestamp);
    m_lastLogin = databaseValues.last_login.transform(common::toChronoTimePoint);
    m_organizationId = databaseValues.id_organization;
    m_lastModifiedByUser = databaseValues.last_modified_by_user;
}

void UserModel::autoGenerateLoginAndPassword() noexcept
{
    if (m_loginName.empty() && m_password.empty())
    {
        createLoginBasedOnUserName(m_lastName, m_firstName, m_middleInitial);
    }
}

void UserModel::createLoginBasedOnUserName(
    const std::string &lastName, const std::string &firstName, const std::string &middleInitial)  noexcept
{
    std::string tempLoginName(lastName);
    tempLoginName += firstName;
    if (middleInitial.size())
    {
        tempLoginName += middleInitial[0];
    }

    setLoginName(tempLoginName);
    setPassword(tempLoginName);
}

void UserModel::setLastName(const std::string &lastName) noexcept
{
    m_modified = true;
    m_lastName = lastName;
}

void UserModel::setFirstName(const std::string &firstName) noexcept
{
    m_modified = true;
    m_firstName = firstName;
}

void UserModel::setMiddleInitial(const std::string &middleinit) noexcept
{
    m_modified = true;
    m_middleInitial = middleinit;
}

void UserModel::setEmail(const std::string &email) noexcept
{
    m_modified = true;
    m_email = email;
}

void UserModel::setLoginName(const std::string &loginName) noexcept
{
    m_modified = true;
    m_loginName = loginName;
}

void UserModel::setPassword(const std::string &password) noexcept
{
    m_modified = true;
    m_password = password;
}

void UserModel::setStartTime(const std::string &startTime) noexcept
{
    m_modified = true;
    m_preferences.startTime = startTime;
}

void UserModel::setEndTime(const std::string &endTime) noexcept
{
    m_modified = true;
    m_preferences.endTime = endTime;
}

void UserModel::setPriorityInSchedule(bool inSchedule) noexcept
{
    m_modified = true;
    m_preferences.includePriorityInSchedule = inSchedule;
}

void UserModel::setMinorPriorityInSchedule(bool inSchedule) noexcept
{
    m_modified = true;
    m_preferences.includeMinorPriorityInSchedule = inSchedule;
}

void UserModel::setUsingLettersForMaorPriority(bool usingLetters) noexcept
{
    m_modified = true;
    m_preferences.userLetterForMajorPriority = usingLetters;
}

void UserModel::setSeparatingPriorityWithDot(bool separate) noexcept
{
    m_modified = true;
    m_preferences.separateMajorAndMinorWithDot = separate;
}

void UserModel::setUserID(std::size_t UserID) noexcept
{
    m_modified = true;
    m_primaryKey = UserID;
}

void UserModel::setLastLogin(std::chrono::system_clock::time_point dateAndTime) noexcept
{
    m_modified = true;
    m_lastLogin = dateAndTime;
}

void UserModel::setOrganizationID(std::size_t organizationId) noexcept
{
    m_modified = true;
    m_organizationId = organizationId;
}

bool UserModel::isMissingLastName() const noexcept
{
    return (m_lastName.empty() || m_lastName.length() < minNameLenght);
}

bool UserModel::isMissingFirstName() const noexcept
{
    return (m_firstName.empty() || m_firstName.length() < minNameLenght);
}

bool UserModel::isMissingLoginName()const noexcept
{
    return (m_loginName.empty() || m_loginName.length() < (2 * minNameLenght));
}

bool UserModel::isMissingPassword() const noexcept
{
    return (m_password.empty() || m_password.length() < minPasswordLenght);;
}

bool UserModel::isMissingEmail() const noexcept
{
    return (m_email.empty() || m_email.length() < minEmailLength);
}

bool UserModel::diffUser(UserModel &other) const noexcept
{
    // Ignore user preferences
    return (m_primaryKey == other.m_primaryKey && m_loginName == other.m_loginName && m_password == other.m_password &&
        m_lastName == other.m_lastName && m_firstName == other.m_firstName &&m_middleInitial == other.m_middleInitial);
}

void UserModel::initRequiredFields() noexcept
{
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingLastName, this), "Last Name"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingFirstName, this), "First Name"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingEmail, this), "Email"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingLoginName, this), "Login Name"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserModel::isMissingPassword, this), "Password"});
}

/*
 * To make the code more maintainable each field / column in a table will have
 * its own line in insert and update statements. 
 */
std::string UserModel::formatInsertStatement()
{
    if (m_lastModifiedByUser == 0)
    {
        m_lastModifiedByUser = 1;
    }
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "INSERT INTO user_profile(");
    boost::mysql::format_sql_to(fctx, "id_organization, ");
    boost::mysql::format_sql_to(fctx, "last_name, ");
    boost::mysql::format_sql_to(fctx, "first_name, " );
    boost::mysql::format_sql_to(fctx, "middle_initial, ");
    boost::mysql::format_sql_to(fctx, "email_address, ");
    boost::mysql::format_sql_to(fctx, "user_login, ");
    boost::mysql::format_sql_to(fctx, "hashed_password, ");
    boost::mysql::format_sql_to(fctx, "preferences, ");
    boost::mysql::format_sql_to(fctx, "last_modified_by_user ");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_organizationId);
    boost::mysql::format_sql_to(fctx, "{}, ", m_lastName);
    boost::mysql::format_sql_to(fctx, "{}, ", m_firstName);
    boost::mysql::format_sql_to(fctx, "{}, ", m_middleInitial);
    boost::mysql::format_sql_to(fctx, "{}, ", m_email);
    boost::mysql::format_sql_to(fctx, "{}, ", m_loginName);
    boost::mysql::format_sql_to(fctx, "{}, ", m_password);
    boost::mysql::format_sql_to(fctx, "{}, ", buildPreferenceText());
    boost::mysql::format_sql_to(fctx, "{} ", m_lastModifiedByUser);
    boost::mysql::format_sql_to(fctx, ")");
    
    return std::move(fctx).get().value();
}

std::string UserModel::formatUpdateStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_profile SET ");
    boost::mysql::format_sql_to(fctx, "user_profile.id_organization = {}, ", m_organizationId);
    boost::mysql::format_sql_to(fctx, "user_profile.last_name = {}, ", m_lastName);
    boost::mysql::format_sql_to(fctx, "user_profile.first_name = {}, ", m_firstName);
    boost::mysql::format_sql_to(fctx, "user_profile.middle_initial = {}, ", m_middleInitial);
    boost::mysql::format_sql_to(fctx, "user_profile.email_address = {}, ", m_email);
    boost::mysql::format_sql_to(fctx, "user_profile.user_login = {}, ", m_loginName);
    boost::mysql::format_sql_to(fctx, "user_profile.hashed_password = {}, ", m_password);
    boost::mysql::format_sql_to(fctx, "user_profile.preferences = {}, ", buildPreferenceText());
    boost::mysql::format_sql_to(fctx, "user_profile.last_login = {}, ", m_lastLogin.transform(common::toBoostDateTime));
    boost::mysql::format_sql_to(fctx, "user_profile.last_modified_by_user = {} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_primaryKey);
    boost::mysql::format_sql_to(fctx, "WHERE user_profile.user_id = {}", m_primaryKey);
        
    return std::move(fctx).get().value();
}

std::string UserModel::formatDeleteStatement()
{
    return boost::mysql::format_sql(getFormatOptions(),
        "UPDATE user_profile SET user_profile.deleted = 1, user_profile.last_modified_by_user = {} WHERE user_profile.user_id = {}",
         m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_primaryKey, m_primaryKey);
}

std::string UserModel::buildPreferenceText() noexcept
{
    std::vector<std::string> preferences;

    preferences.push_back(getStartTime());
    preferences.push_back(getEndTime());
    preferences.push_back(std::to_string(static_cast<int>(isPriorityInSchedule())));
    preferences.push_back(std::to_string(static_cast<int>(isMinorPriorityInSchedule())));
    preferences.push_back(std::to_string(static_cast<int>(isUsingLettersForMaorPriority())));
    preferences.push_back(std::to_string(static_cast<int>(isSeparatingPriorityWithDot())));

    auto joined_view = preferences | std::views::join_with(m_delimiter);
    return std::ranges::to<std::string>(joined_view);
}

void UserModel::parsePrefenceText(std::string preferences) noexcept
{
    // If user data was altered using SQL in a seperate process then preferences may not
    // have values or enough values.
    if (preferences.empty())
    {
        return;
    }
    std::vector<std::string> subfields = preferences
        | std::views::split(m_delimiter) 
        | std::ranges::to<std::vector<std::string>>();

    if (subfields.size() <= PrefUsingDotIdx)
    {
        return;
    }

    setStartTime(subfields[PrefDayStartIdx]);
    setEndTime(subfields[PrefDayEndIdx]);
    setPriorityInSchedule(std::stoi(subfields[PrefMajorPriorityIdx]));
    setMinorPriorityInSchedule(std::stoi(subfields[PrefMinorPriorityIdx]));
    setUsingLettersForMaorPriority(std::stoi(subfields[PrefUsingLetterIdx]));
    setSeparatingPriorityWithDot(std::stoi(subfields[PrefUsingDotIdx]));

    // Since we are using the setter functions we have to clear the modified flag.
    clearModified();
}

TestStatus UserModel::testEqualityOperator() noexcept
{
    UserModel other;

    other.setUserID(m_primaryKey);
    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;
    
    return (*this == other)? TESTPASSED: TESTFAILED;
}

TestStatus UserModel::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "Last Name", "First Name", "Email", "Login Name", "Password", "User is missing required values"
    };

    setUserID(0);   // Force a modification so that missing fields can be tested.

    std::vector<std::function<void(std::string)>> fieldSettings;
    fieldSettings.push_back(std::bind(&UserModel::setLastName, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&UserModel::setFirstName, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&UserModel::setEmail, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&UserModel::setLoginName, this, std::placeholders::_1));
    fieldSettings.push_back(std::bind(&UserModel::setPassword, this, std::placeholders::_1));

    for (auto setField: fieldSettings)
    {
        if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
        {
            return TESTFAILED;
        }
        expectedErrors.erase(expectedErrors.begin());
        setField("teststringvalue");
    }

    expectedErrors.clear();

    if (m_verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", getModelName(), __func__) << *this << "\n";
    }

    setCreatedTimeStamp(common::TestTimeStampValue);
    if (!insert())
    {
        std::cout << "In  UserModel::testAllInsertFailures() Expected successful insert failed\n" << m_errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

void UserModel::selfTestResetAllValues() noexcept
{
    ModelDBInterface::selfTestResetAllValues();

    m_lastName.clear();
    m_firstName.clear();
    m_middleInitial.clear();
    m_email.clear();
    m_loginName.clear();
    m_password.clear();
    m_createdTimeStamp.reset();
    m_lastLogin.reset();
}

std::vector<AttributeTestFunction> UserModel::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();
    std::vector<AttributeTestFunction> attributeAccessTests;

    attributeAccessTests.push_back({std::bind(&UserModel::testUserIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testLastNameAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testFirstNameAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testMiddleInitialAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testEmailAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testOrganizationAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testLoginNameAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testPassWordAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testCreatedDateAcfcess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testLastLoginAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testStartTimeAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testEndTimeAccesss, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testLastModifiedAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testLastModifiedByUserAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testIncludePriorityInScheduleAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testIncludeMinorPriorityInScheduleAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testUseLetterForMajorPriorityAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserModel::testSeparateMajorAndMinorWithDotAccess, this)});

    return attributeAccessTests;
}

TestStatus UserModel::testUserIdAccesss() noexcept
{
    std::size_t testPrimaryKey = 31;

    return testPrimaryKeyAccessFunctions(testPrimaryKey,
        std::bind(&UserModel::setUserID, this, std::placeholders::_1),
        std::bind(&UserModel::getUserID, this));
}

TestStatus UserModel::testLastNameAccess() noexcept
{
    std::string testValue("AnyLastName");
    return testAccessorFunctions<std::string>(testValue, &m_lastName, "Last Name",
        std::bind(&UserModel::setLastName, this, std::placeholders::_1),
        std::bind(&UserModel::getLastName, this));
}

TestStatus UserModel::testFirstNameAccess() noexcept
{
    std::string testValue("AnyFirstName");
    return testAccessorFunctions<std::string>(testValue, &m_firstName, "First Name",
        std::bind(&UserModel::setFirstName, this, std::placeholders::_1),
        std::bind(&UserModel::getFirstName, this));
}

TestStatus UserModel::testMiddleInitialAccess() noexcept
{
    std::string testValue("A");
    return testAccessorFunctions<std::string>(testValue, &m_middleInitial, "Middle Initial",
        std::bind(&UserModel::setMiddleInitial, this, std::placeholders::_1),
        std::bind(&UserModel::getMiddleInitial, this));
}

TestStatus UserModel::testLoginNameAccess() noexcept
{
    std::string testValue("AnyLoginName");
    return testAccessorFunctions<std::string>(testValue, &m_loginName, "LoginName",
        std::bind(&UserModel::setLoginName, this, std::placeholders::_1),
        std::bind(&UserModel::getLoginName, this));
}

TestStatus UserModel::testPassWordAccess() noexcept
{
    std::string testValue("AnyPassword");
    return testAccessorFunctions<std::string>(testValue, &m_password, "Password",
        std::bind(&UserModel::setPassword, this, std::placeholders::_1),
        std::bind(&UserModel::getPassword, this));
}

TestStatus UserModel::testCreatedDateAcfcess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_createdTimeStamp, "Creation TimeStamp",
        std::bind(&UserModel::setCreatedTimeStamp, this, std::placeholders::_1),
        std::bind(&UserModel::getCreatedTSValue, this));
}

TestStatus UserModel::testLastLoginAccess() noexcept
{
    std::optional<std::chrono::system_clock::time_point> testValue = common::TestTimeStampValue;
    return testOptionalAccessorFunctions<std::chrono::system_clock::time_point>(testValue, &m_lastLogin, "Last Login TimeStamp",
        std::bind(&UserModel::setLastLogin, this, std::placeholders::_1),
        std::bind(&UserModel::getLastLogin, this));
}

TestStatus UserModel::testEmailAccess() noexcept
{
    std::string testValue("AnyEmail");
    return testAccessorFunctions<std::string>(testValue, &m_email, "Email",
        std::bind(&UserModel::setEmail, this, std::placeholders::_1),
        std::bind(&UserModel::getEmail, this));
}

TestStatus UserModel::testOrganizationAccess() noexcept
{
    std::size_t testValue = 1;
    return testOptionalAccessorFunctions<std::size_t>(testValue, &m_organizationId, "Organization ID", 
        std::bind(&UserModel::setOrganizationID, this, std::placeholders::_1),
        std::bind(&UserModel::getOrganizationID, this));
}


TestStatus UserModel::testStartTimeAccess() noexcept
{
    std::string testValue("4:30 AM");
    return testAccessorFunctions<std::string>(testValue, &m_preferences.startTime, "Start Time Preference",
        std::bind(&UserModel::setStartTime, this, std::placeholders::_1),
        std::bind(&UserModel::getStartTime, this));
}

TestStatus UserModel::testEndTimeAccesss() noexcept
{
    std::string testValue("4:15 PM");
    return testAccessorFunctions<std::string>(testValue, &m_preferences.endTime, "End Time Preference",
        std::bind(&UserModel::setEndTime, this, std::placeholders::_1),
        std::bind(&UserModel::getEndTime, this));
}

TestStatus UserModel::testLastModifiedAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_lastUpdateTimeStamp, "Last Modified TimeStamp",
        std::bind(&UserModel::setLastModifiedTimeStamp, this, std::placeholders::_1),
        std::bind(&UserModel::getLastModifiedValue, this));
}

TestStatus UserModel::testLastModifiedByUserAccess() noexcept
{
    std::size_t testUserId = 1;

    return testLastModifiedByAccess(testUserId);
}

TestStatus UserModel::testIncludePriorityInScheduleAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &m_preferences.includePriorityInSchedule, "Include Priority In Schedule",
        std::bind(&UserModel::setPriorityInSchedule, this, std::placeholders::_1),
        std::bind(&UserModel::isPriorityInSchedule, this));
}

TestStatus UserModel::testIncludeMinorPriorityInScheduleAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &m_preferences.includeMinorPriorityInSchedule, "Include Minor Priority In Schedule",
        std::bind(&UserModel::setMinorPriorityInSchedule, this, std::placeholders::_1),
        std::bind(&UserModel::isMinorPriorityInSchedule, this));
}

TestStatus UserModel::testUseLetterForMajorPriorityAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &m_preferences.userLetterForMajorPriority, "Use Letter For Major Priority Access",
        std::bind(&UserModel::setUsingLettersForMaorPriority, this, std::placeholders::_1),
        std::bind(&UserModel::isUsingLettersForMaorPriority, this));
}

TestStatus UserModel::testSeparateMajorAndMinorWithDotAccess() noexcept
{
    bool testValue = true;
    return testAccessorFunctions<bool>(testValue, &m_preferences.separateMajorAndMinorWithDot, "Separate Major And Minor With Dot Access",
        std::bind(&UserModel::setSeparatingPriorityWithDot, this, std::placeholders::_1),
        std::bind(&UserModel::isSeparatingPriorityWithDot, this));
}

std::vector<ExceptionTestElement> UserModel::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserModel::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&UserModel::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&UserModel::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}


TestStatus UserModel::testExceptionInsert() noexcept
{
    selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setLastName("LastName");
    setFirstName("FirstName");
    setMiddleInitial("M");
    setEmail("FirstName.LastName@LastName.com");
    autoGenerateLoginAndPassword();
    setCreatedTimeStamp(timeStamp);
    setLastLogin(timeStamp);

    return testExceptionAndSuccessNArgs("UserModel::insert", std::bind(&UserModel::insert, this));
}

TestStatus UserModel::testExceptionUpdate() noexcept
{
    selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setUserID(1);
    setLastName("LastName");
    setFirstName("FirstName");
    setMiddleInitial("M");
    setEmail("FirstName.LastName@LastName.com");
    autoGenerateLoginAndPassword();
    setCreatedTimeStamp(timeStamp);
    setLastLogin(timeStamp);

    return testExceptionAndSuccessNArgs("UserModel::update", std::bind(&UserModel::update, this));
}

TestStatus UserModel::testExceptionHide() noexcept
{
    selfTestResetAllValues();

    std::size_t testUserId = 1;

    setUserID(testUserId);

    return testExceptionAndSuccessNArgs("UserModel::hide", std::bind(&UserModel::hide, this, std::placeholders::_1), testUserId);
}
