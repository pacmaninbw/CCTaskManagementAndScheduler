// Project Header Files
#include "chronoBoostConversions.h"
#include "commonTestValues.h"
#include "UserGoalModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <vector>

UserGoalModel::UserGoalModel()
: ModelDBInterface("UserGoalModel", "idUserGoals")
{
    m_userID = 0;
}

UserGoalModel::UserGoalModel(const GoalDbQueryValues &databaseValues)
: UserGoalModel()
{
    m_primaryKey = databaseValues.id_user_goals;
    m_userID = databaseValues.user_id;
    m_description = databaseValues.description;
    m_priority = databaseValues.priority;
    m_parentID = databaseValues.parent_goal;
    m_createdTimeStamp = common::toChronoTimePoint(databaseValues.creation_timestamp);
    m_lastUpdateTimeStamp = common::toChronoTimePoint(databaseValues.last_modified_time_stamp);
    m_deleted = databaseValues.deleted;
    m_lastModifiedByUser = databaseValues.last_modified_by_user;
}


void UserGoalModel::setUserId(std::size_t userId)
{
    m_modified = true;
    m_userID = userId;
}

void UserGoalModel::setDescription(std::string newDescription)
{
    m_modified = true;
    m_description = newDescription;
}

void UserGoalModel::setPriority(unsigned int newPriority)
{
    m_modified = true;
    m_priority = newPriority;
}

void UserGoalModel::setParentID(std::size_t newParentID)
{
    m_modified = true;
    m_parentID = newParentID;
}

bool UserGoalModel::diffGoal(UserGoalModel &other)
{
    return (m_primaryKey == other.m_primaryKey && m_userID == other.m_userID &&
        m_description == other.m_description);
}

void UserGoalModel::initRequiredFields()
{
    m_missingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingUserID, this), "User ID"});
    m_missingRequiredFieldsTests.push_back({std::bind(&UserGoalModel::isMissingDescription, this), "Description"});
}

/*
 * To make the code more maintainable each field / column in a table will have
 * its own line in insert and update statements. 
 */
std::string UserGoalModel::formatInsertStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());

    boost::mysql::format_sql_to(fctx, "INSERT INTO user_goals (");
    boost::mysql::format_sql_to(fctx, "user_id, ");
    boost::mysql::format_sql_to(fctx, "description, ");
    boost::mysql::format_sql_to(fctx, "priority, ");
    boost::mysql::format_sql_to(fctx, "parent_goal, ");
    boost::mysql::format_sql_to(fctx, "deleted, ");
    boost::mysql::format_sql_to(fctx, "last_modified_by_user ");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_userID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_description);
    boost::mysql::format_sql_to(fctx, "{}, ", m_priority);
    boost::mysql::format_sql_to(fctx, "{}, ", m_parentID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_deleted);
    boost::mysql::format_sql_to(fctx, "{} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_userID);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string UserGoalModel::formatUpdateStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());

    boost::mysql::format_sql_to(fctx, "UPDATE user_goals SET ");
    boost::mysql::format_sql_to(fctx, "user_goals.description = {}, ", m_description);
    boost::mysql::format_sql_to(fctx, "user_goals.priority = {}, ", m_priority);
    boost::mysql::format_sql_to(fctx, "user_goals.parent_goal = {}, ", m_parentID);
    boost::mysql::format_sql_to(fctx, "user_goals.last_modified_by_user = {} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_userID);
    boost::mysql::format_sql_to(fctx, "WHERE user_goals.user_id = {} ", m_userID);
    boost::mysql::format_sql_to(fctx, "AND user_goals.id_user_goals = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}

std::string UserGoalModel::formatDeleteStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());

    boost::mysql::format_sql_to(fctx, "UPDATE user_goals SET user_goals.deleted = 1, ");
    boost::mysql::format_sql_to(fctx, "user_goals.last_modified_by_user = {} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_userID);
    boost::mysql::format_sql_to(fctx, "WHERE user_goals.user_id = {} ", m_userID);
    boost::mysql::format_sql_to(fctx, "AND user_goals.id_user_goals = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}

void UserGoalModel::selfTestResetAllValues() noexcept
{
    ModelDBInterface::selfTestResetAllValues();

    m_userID = 0;
    m_description.clear();;
    m_priority.reset();
    m_parentID.reset();
    m_createdTimeStamp.reset();
    m_lastUpdateTimeStamp.reset();
}

TestStatus UserGoalModel::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Description", " missing required values"
    };

    setGoalId(0);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setUserId(51);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setDescription("Testing negative note insertion path");

    expectedErrors.clear();
    clearErrorMessages();

    setCreatedTimeStamp(common::TestTimeStampValue);

    if (m_verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", getModelName(), __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In  UserGoalModel::testAllInsertFailures() Expected successful insert failed\n" << m_errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus UserGoalModel::testEqualityOperator() noexcept
{
    UserGoalModel other;

    other.setGoalId(m_primaryKey);
    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;

    return (*this == other)? TESTPASSED : TESTFAILED;
}

void UserGoalModel::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

std::vector<AttributeTestFunction> UserGoalModel::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<AttributeTestFunction> attributeAccessTests;
    attributeAccessTests.push_back({std::bind(&UserGoalModel::testGoalIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalModel::testUserIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalModel::testDescriptionAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalModel::testCreationDateAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalModel::testParentIdAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalModel::testPriorityAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalModel::testLastUpdateAccess, this)});
    attributeAccessTests.push_back({std::bind(&UserGoalModel::testLastModifiedByUserAccess, this)});

    return attributeAccessTests;
}

TestStatus UserGoalModel::testGoalIdAccesss() noexcept
{
    return testPrimaryKeyAccessFunctions(57,
        std::bind(&UserGoalModel::setGoalId, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getGoalId, this));
}

TestStatus UserGoalModel::testUserIdAccesss() noexcept
{
    return testForeignKeyFields(23, &m_userID, "User ID",
        std::bind(&UserGoalModel::setUserId, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getUserId, this));
}

TestStatus UserGoalModel::testDescriptionAccess() noexcept
{
    return testAccessorFunctions<std::string>("Test note content access", &m_description, "Content",
        std::bind(&UserGoalModel::setDescription, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getDescription, this));
}

TestStatus UserGoalModel::testCreationDateAccess() noexcept
{
    return testTimeStampAccessorFunctions(common::TestTimeStampValue, &m_createdTimeStamp, "Date Added",
        std::bind(&UserGoalModel::setCreatedTimeStamp, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getCreatedTSValue, this));
}

TestStatus UserGoalModel::testParentIdAccess() noexcept
{
    return testOptionalAccessorFunctions<std::size_t>(1, &m_parentID, "Parent ID",
        std::bind(&UserGoalModel::setParentID, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getParentId, this));
}

TestStatus UserGoalModel::testPriorityAccess() noexcept
{
    return testOptionalAccessorFunctions<unsigned int>(1, &m_priority, "Priority",
        std::bind(&UserGoalModel::setPriority, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getPriority, this));
}

TestStatus UserGoalModel::testLastUpdateAccess() noexcept
{
    return testTimeStampAccessorFunctions(common::TestTimeStampValue, &m_lastUpdateTimeStamp, "Last Modified Time Stamp",
        std::bind(&UserGoalModel::setLastModifiedTimeStamp, this, std::placeholders::_1),
        std::bind(&UserGoalModel::getLastModifiedValue, this));
}

TestStatus UserGoalModel::testLastModifiedByUserAccess() noexcept
{
    std::size_t testUserId = 1;

    return testLastModifiedByAccess(testUserId);
}

std::vector<ExceptionTestElement> UserGoalModel::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&UserGoalModel::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&UserGoalModel::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&UserGoalModel::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}

TestStatus UserGoalModel::testExceptionInsert() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setDescription("Testing insertion exception");
    setUserId(27);
    setCreatedTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("UserGoalModel::insert", std::bind(&UserGoalModel::insert, this));
}

TestStatus UserGoalModel::testExceptionUpdate() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setGoalId(37);
    setDescription("Testing insertion exception");
    setUserId(23);
    setCreatedTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("UserGoalModel::update", std::bind(&UserGoalModel::update, this));
}

TestStatus UserGoalModel::testExceptionHide() noexcept
{
    selfTestResetAllValues();
    std::size_t testUserId = 1;

    setGoalId(1);
    setUserId(testUserId);

    return testExceptionAndSuccessNArgs("UserGoalModel::hide", std::bind(&UserGoalModel::hide, this, std::placeholders::_1), testUserId);
}

