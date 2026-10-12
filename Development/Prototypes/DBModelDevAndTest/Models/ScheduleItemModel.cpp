// Project Header Files
#include "commonTestValues.h"
#include "ScheduleItemModel.h"
#include "chronoBoostConversions.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <string>
#include <vector>

ScheduleItemModel::ScheduleItemModel()
: ModelDBInterface("ScheduleItem", "idUserScheduleItem")
{
    m_userID = 0;
    m_title = "";
    m_personal = false;
}

ScheduleItemModel::ScheduleItemModel(const ScheduleItemDbQueryValues &databaseValues)
: ScheduleItemModel()
{
    m_primaryKey = databaseValues.id_user_schedule_item;
    m_userID = databaseValues.user_id;
    m_title = databaseValues.title;
    m_startTime = common::toChronoTimePoint(databaseValues.start_date_time);
    m_endTime = common::toChronoTimePoint(databaseValues.end_date_time);
    m_personal = static_cast<bool>(databaseValues.personal);
    m_location = databaseValues.location.value_or("");
    m_createdTimeStamp = common::toChronoTimePoint(databaseValues.created_timestamp);
    m_lastUpdateTimeStamp = common::toChronoTimePoint(databaseValues.last_modified_time_stamp);
    m_deleted = databaseValues.deleted;
    m_lastModifiedByUser = databaseValues.last_modified_by_user;
}

void ScheduleItemModel::setUserID(std::size_t userId)
{
    m_modified = true;
    m_userID = userId;
}

void ScheduleItemModel::setTitle(std::string title)
{
    m_modified = true;
    m_title = title;
}

void ScheduleItemModel::setStartDateAndTime(std::chrono::system_clock::time_point startTime)
{
    m_modified = true;
    m_startTime = startTime;
}

void ScheduleItemModel::setEndDateAndTime(std::chrono::system_clock::time_point endTime)
{
    m_modified = true;
    m_endTime = endTime;
}

void ScheduleItemModel::setPersonal(bool personal)
{
    m_modified = true;
    m_personal = personal;
}

void ScheduleItemModel::setLocation(std::string location)
{
    m_modified = true;
    m_location = location;
}

void ScheduleItemModel::setScheduleItemID(std::size_t eventId)
{
    m_modified = true;
    m_primaryKey = eventId;
}

bool ScheduleItemModel::diffScheduleItem(ScheduleItemModel &other)
{
    std::chrono::system_clock::time_point localStartTime = m_startTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point otherStartTime = other.m_startTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point localEndTime = m_endTime.value_or(std::chrono::system_clock::now());
    std::chrono::system_clock::time_point otherEndTime = other.m_endTime.value_or(std::chrono::system_clock::now());

    return (m_primaryKey == other.m_primaryKey &&
        m_userID == other.m_userID &&
        m_title == other.m_title &&
        localStartTime == otherStartTime &&
        localEndTime == otherEndTime &&
        m_personal == other.m_personal
    );
}

/*
 * To make the code more maintainable each field / column in a table will have
 * its own line in insert and update statements. 
 */
std::string ScheduleItemModel::formatInsertStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "INSERT INTO user_schedule_item  (");
    boost::mysql::format_sql_to(fctx, "user_id, ");
    boost::mysql::format_sql_to(fctx, "start_date_time, ");
    boost::mysql::format_sql_to(fctx, "end_date_time, ");
    boost::mysql::format_sql_to(fctx, "title, ");
    boost::mysql::format_sql_to(fctx, "personal, ");
    boost::mysql::format_sql_to(fctx, "location, ");
    boost::mysql::format_sql_to(fctx, "deleted, ");
    boost::mysql::format_sql_to(fctx, "last_modified_by_user ");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_userID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_startTime.transform(common::toBoostDateTime));
    boost::mysql::format_sql_to(fctx, "{}, ", m_endTime.transform(common::toBoostDateTime));
    boost::mysql::format_sql_to(fctx, "{}, ", m_title);
    boost::mysql::format_sql_to(fctx, "{}, ", static_cast<unsigned int>(m_personal?1:0));
    boost::mysql::format_sql_to(fctx, "{}, ", m_location);
    boost::mysql::format_sql_to(fctx, "{}, ", m_deleted);
    boost::mysql::format_sql_to(fctx, "{} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_userID);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string ScheduleItemModel::formatUpdateStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_schedule_item SET ");
    boost::mysql::format_sql_to(fctx, "user_schedule_item.start_date_time = {}, ", m_startTime.transform(common::toBoostDateTime));
    boost::mysql::format_sql_to(fctx, "user_schedule_item.end_date_time = {}, ", m_endTime.transform(common::toBoostDateTime));
    boost::mysql::format_sql_to(fctx, "user_schedule_item.title = {}, ", m_title);
    boost::mysql::format_sql_to(fctx, "user_schedule_item.personal = {}, ", static_cast<unsigned int>(m_personal?1:0));
    boost::mysql::format_sql_to(fctx, "user_schedule_item.location = {}, ", m_location);
    boost::mysql::format_sql_to(fctx, "user_schedule_item.last_modified_by_user = {} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_userID);
    boost::mysql::format_sql_to(fctx, "WHERE user_schedule_item.id_user_schedule_item = {} ", m_primaryKey);
    boost::mysql::format_sql_to(fctx, "AND user_schedule_item.user_id = {}", m_userID);

    return (std::move(fctx).get().value());
}

std::string ScheduleItemModel::formatDeleteStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_schedule_item SET user_schedule_item.deleted = 1, ");
    boost::mysql::format_sql_to(fctx, "user_schedule_item.last_modified_by_user = {} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_userID);
    boost::mysql::format_sql_to(fctx, "WHERE user_schedule_item.user_id = {} ", m_userID);
    boost::mysql::format_sql_to(fctx, "AND user_schedule_item.id_user_schedule_item = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}

void ScheduleItemModel::initRequiredFields()
{
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingUserID, this), "User ID"});
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingTitle, this), "Title"});
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingStartTime, this), "Start Time"});
    m_missingRequiredFieldsTests.push_back({std::bind(&ScheduleItemModel::isMissingEndTime, this), "End Time"});
}

void ScheduleItemModel::selfTestResetAllValues() noexcept
{
    ModelDBInterface::selfTestResetAllValues();

    m_userID = 0;
    m_title.clear();
    m_startTime.reset();
    m_endTime.reset();
    m_personal = false;
    m_location.reset();
    m_createdTimeStamp.reset();
    m_lastUpdateTimeStamp.reset();
}

TestStatus ScheduleItemModel::testAllInsertFailures()
{

    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Title", "Start Time", "End Time", " missing required values"
    };

    setScheduleItemID(0);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());

    setUserID(1);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());

    setTitle("Test missing required fields: Set title");
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());

    setStartDateAndTime(common::TestTimeStampValue);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());

    setEndDateAndTime(common::TestTimeStampValue);

    expectedErrors.clear();
    clearErrorMessages();

    setCreatedTimeStamp(common::TestTimeStampValue);

    if (m_verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", getModelName(), __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In ScheduleItemModel::testAllInsertFailuresExpected successful insert failed\n" << m_errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;}

TestStatus ScheduleItemModel::testEqualityOperator() noexcept
{
    try
    {
        ScheduleItemModel other;

        other.setScheduleItemID(m_primaryKey);

        if (*this == other)
        {
            return TESTFAILED;
        }

        other = *this;

        return (*this == other)? TESTPASSED : TESTFAILED;
    }
    catch(std::exception &e)
    {
        std::cerr << std::format("ScheduleItemModel::testEqualityOperator:: Caught Exception:: {}\n", e.what());
        return TESTFAILED;
    }
}

void ScheduleItemModel::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

std::vector<ExceptionTestElement> ScheduleItemModel::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&ScheduleItemModel::testExceptionInsert, this), "testExceeptionInsert"});
    exceptionTests.push_back({std::bind(&ScheduleItemModel::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&ScheduleItemModel::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}

TestStatus ScheduleItemModel::testExceptionInsert() noexcept
{
    selfTestResetAllValues();
    m_forceException = true;

    setUserID(1);
    setTitle("Testing Exception handling for Schedule Item Insert");
    setStartDateAndTime(common::TestTimeStampValue);
    setEndDateAndTime(common::TestTimeStampValue);
    setCreatedTimeStamp(common::TestTimeStampValue);

    if (testFormatExceptionCatchSuccessNArgs(
        "ScheduleItemModel::formatInsertStatement", std::bind(&ScheduleItemModel::formatInsertStatement, this)) == TESTFAILED)
    {
        std::cerr << "ScheduleItemModel::formatInsertStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("ScheduleItemModel::insert", std::bind(&ScheduleItemModel::insert, this));
}

TestStatus ScheduleItemModel::testExceptionUpdate() noexcept
{
    selfTestResetAllValues();

    setScheduleItemID(57);
    setUserID(1);
    setTitle("Testing Exception handling for Schedule Item Update");
    setStartDateAndTime(common::TestTimeStampValue);
    setEndDateAndTime(common::TestTimeStampValue);
    setCreatedTimeStamp(common::TestTimeStampValue);
    setLastModifiedTimeStamp(common::TestTimeStampValue);

    if (testFormatExceptionCatchSuccessNArgs(
        "ScheduleItemModel::formatUpdateStatement", std::bind(&ScheduleItemModel::formatUpdateStatement, this)) == TESTFAILED)
    {
        std::cerr << "ScheduleItemModel::formatUpdateStatement() returned a string in Exception Test, FAILED\n";
        return TESTFAILED;
    }

    return testExceptionAndSuccessNArgs("ScheduleItemModel::update", std::bind(&ScheduleItemModel::update, this));
}

TestStatus ScheduleItemModel::testExceptionHide() noexcept
{
    selfTestResetAllValues();
    std::size_t testUserId = 1;

    setScheduleItemID(57);
    setUserID(testUserId);
    setTitle("Testing Exception handling for Schedule Item Update");
    setStartDateAndTime(common::TestTimeStampValue);
    setEndDateAndTime(common::TestTimeStampValue);
    setCreatedTimeStamp(common::TestTimeStampValue);
    setLastModifiedTimeStamp(common::TestTimeStampValue);

    return testExceptionAndSuccessNArgs("ScheduleItemModel::hide", std::bind(&ScheduleItemModel::hide, this, std::placeholders::_1), testUserId);
}

std::vector<AttributeTestFunction> ScheduleItemModel::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<AttributeTestFunction> attributeAccessTests; 
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testScheduleItemIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testUserIDAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testTitleAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testStartTimeAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testOptionalStartTimeAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testEndTimeAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testOptionalEndTimeAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testCreationTimeStampAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testLastUpDateTimeStampAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testLocationAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testPersonalAccess, this)});
    attributeAccessTests.push_back({std::bind(&ScheduleItemModel::testLastModifiedByUserAccess, this)});

    return attributeAccessTests;
}

TestStatus ScheduleItemModel::testScheduleItemIDAccess() noexcept
{
    std::size_t testPrimaryKey = 57;

    return testPrimaryKeyAccessFunctions(testPrimaryKey,
        std::bind(&ScheduleItemModel::setScheduleItemID, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getScheduleItemID, this));
}

TestStatus ScheduleItemModel::testUserIDAccess() noexcept
{
    std::size_t testValue = 1;

    return testForeignKeyFields(testValue, &m_userID, "User ID",
        std::bind(&ScheduleItemModel::setUserID, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getUserID, this));
}

TestStatus ScheduleItemModel::testTitleAccess() noexcept
{
    std::string testValue("Testing schedule item Title string access");

    return testAccessorFunctions<std::string>(testValue, &m_title, "Title",
        std::bind(&ScheduleItemModel::setTitle, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getTitle, this));
}

TestStatus ScheduleItemModel::testStartTimeAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;

    return testTimeStampAccessorFunctions(testValue, &m_startTime, "Schedule Item Start Time",
        std::bind(&ScheduleItemModel::setStartDateAndTime, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getStartTime, this));
}

TestStatus ScheduleItemModel::testOptionalStartTimeAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;

    return testOptionalAccessorFunctions< std::chrono::system_clock::time_point>(testValue, &m_startTime, "Optional Start Time",
        std::bind(&ScheduleItemModel::setStartDateAndTime, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getOptionalStartTime, this));
}

TestStatus ScheduleItemModel::testEndTimeAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;

    return testTimeStampAccessorFunctions(testValue, &m_endTime, "Schedule Item End Time",
        std::bind(&ScheduleItemModel::setEndDateAndTime, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getEndTime, this));
}

TestStatus ScheduleItemModel::testOptionalEndTimeAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;

    return testOptionalAccessorFunctions< std::chrono::system_clock::time_point>(testValue, &m_startTime, "Optional End Time",
        std::bind(&ScheduleItemModel::setEndDateAndTime, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getOptionalEndTime, this));
}

TestStatus ScheduleItemModel::testCreationTimeStampAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;

    return testTimeStampAccessorFunctions(testValue, &m_createdTimeStamp, "Schedule Item Creation Timestamp",
        std::bind(&ScheduleItemModel::setCreatedTimeStamp, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getCreatedTSValue, this));
}

TestStatus ScheduleItemModel::testLastUpDateTimeStampAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;

    return testTimeStampAccessorFunctions(testValue, &m_lastUpdateTimeStamp, "Schedule Item Last Update Timestamp",
        std::bind(&ScheduleItemModel::setLastModifiedTimeStamp, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getLastModifiedValue, this));
}

TestStatus ScheduleItemModel::testPersonalAccess() noexcept
{
    bool testValue = true;

    return testAccessorFunctions<bool>(testValue, &m_personal, "Personal",
        std::bind(&ScheduleItemModel::setPersonal, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::isPersonal, this));
}

TestStatus ScheduleItemModel::testLastModifiedByUserAccess() noexcept
{
    std::size_t testUserId = 1;

    return testLastModifiedByAccess(testUserId);
}

TestStatus ScheduleItemModel::testLocationAccess() noexcept
{
    std::string testValue("Home Office");

    return testOptionalAccessorFunctions<std::string>(testValue, &m_location, "Location",
        std::bind(&ScheduleItemModel::setLocation, this, std::placeholders::_1),
        std::bind(&ScheduleItemModel::getLocation, this));
}

