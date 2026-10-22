// Project Header Files
#include "chronoBoostConversions.h"
#include "commonTestValues.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <vector>

NoteModel::NoteModel()
: ModelDBInterface("Note", "idUserNotes"), m_userID{0}
{
}

NoteModel::NoteModel(const NoteDbQueryValues &dbTranslator)
: NoteModel()
{
    m_primaryKey = dbTranslator.id_user_notes;
    m_userID = dbTranslator.user_id;
    m_content = dbTranslator.content;
    m_createdTimeStamp = common::toChronoTimePoint(dbTranslator.note_creation);
    m_lastUpdateTimeStamp = common::toChronoTimePoint(dbTranslator.last_modifed);
    m_deleted = dbTranslator.deleted;
    m_lastModifiedByUser = dbTranslator.last_modified_by_user;
}

void NoteModel::setUserId(std::size_t userId)
{
    m_modified = true;
    m_userID = userId;
}

void NoteModel::setContent(std::string content)
{
    m_modified = true;
    m_content = content;
}

bool NoteModel::diffNote(NoteModel &other)
{
    // Ignore user preferences
    return (m_primaryKey == other.m_primaryKey && m_userID == other.m_userID && m_content == other.m_content &&
        m_createdTimeStamp == other.m_createdTimeStamp && m_lastUpdateTimeStamp == other.m_lastUpdateTimeStamp);
}

void NoteModel::initRequiredFields()
{
    m_missingRequiredFieldsTests.push_back({std::bind(&NoteModel::isMissingUserID, this), "User ID"});
    m_missingRequiredFieldsTests.push_back({std::bind(&NoteModel::isMissingContent, this), "Content"});
}

/*
 * To make the code more maintainable each field / column in a table will have
 * its own line in insert and update statements. 
 */
std::string NoteModel::formatInsertStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "INSERT INTO user_notes(");
    boost::mysql::format_sql_to(fctx, "user_id, ");
    boost::mysql::format_sql_to(fctx, "content, ");
    boost::mysql::format_sql_to(fctx, "deleted, ");
    boost::mysql::format_sql_to(fctx, "note_creation, ");
    boost::mysql::format_sql_to(fctx, "last_modified_by_user ");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_userID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_content);
    boost::mysql::format_sql_to(fctx, "{}, ", m_deleted);
    boost::mysql::format_sql_to(fctx, "{}, ", m_createdTimeStamp.transform(common::toBoostDateTime));
    boost::mysql::format_sql_to(fctx, "{} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_userID);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string NoteModel::formatUpdateStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_notes SET user_notes.content = {}, ", m_content);
    boost::mysql::format_sql_to(fctx, "user_notes.last_modified_by_user = {} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_userID);
    boost::mysql::format_sql_to(fctx, "WHERE user_notes.id_user_notes = {} ", m_primaryKey);
    boost::mysql::format_sql_to(fctx, "AND user_notes.user_id = {}", m_userID);

    return (std::move(fctx).get().value());
}

std::string NoteModel::formatDeleteStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_notes SET user_notes.deleted = 1, ");
    boost::mysql::format_sql_to(fctx, "user_notes.last_modified_by_user = {} ", m_lastModifiedByUser > 0 ? m_lastModifiedByUser : m_userID);
    boost::mysql::format_sql_to(fctx, "WHERE user_notes.user_id = {} ", m_userID);
    boost::mysql::format_sql_to(fctx, "AND user_notes.id_user_notes = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}

void NoteModel::selfTestResetAllValues() noexcept
{
    ModelDBInterface::selfTestResetAllValues();

    m_userID = 0;
    m_content.clear();
    m_createdTimeStamp.reset();
    m_lastUpdateTimeStamp.reset();
}

TestStatus NoteModel::testAllInsertFailures()
{
    selfTestResetAllValues();

    if (testCommonInsertFailurePath() != TESTPASSED)
    {
        return TESTFAILED;
    }

    std::vector<std::string> expectedErrors =
    {
        "User ID", "Content", " missing required values"
    };

    setNoteId(0);
    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setUserId(27);

    if (testInsertionFailureMessages(expectedErrors) != TESTPASSED)
    {
        return TESTFAILED;
    }
    expectedErrors.erase(expectedErrors.begin());
    setContent("Testing negative note insertion path");

    expectedErrors.clear();
    clearErrorMessages();

    if (m_verboseOutput)
    {
        std::cout << std::format("{}::{} before successful insert this = \n", getModelName(), __func__) << *this << "\n";
    }

    if (!insert())
    {
        std::cout << "In  NoteModel::testAllInsertFailures() Expected successful insert failed\n" << m_errorMessages << "\n";
        return TESTFAILED;
    }

    return TESTPASSED;
}

TestStatus NoteModel::testEqualityOperator() noexcept
{
    NoteModel other;

    other.setNoteId(m_primaryKey);
    if (*this == other)
    {
        return TESTFAILED;
    }

    other = *this;

    return (*this == other)? TESTPASSED : TESTFAILED;
}

void NoteModel::testOutput() noexcept
{
    std::cout << "Test Output: " << *this << "\n";
}

std::vector<AttributeTestFunction> NoteModel::initAttributeAccessTests() noexcept
{
    selfTestResetAllValues();

    std::vector<AttributeTestFunction> attributeAccessTests;
    attributeAccessTests.push_back({std::bind(&NoteModel::testNoteIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&NoteModel::testUserIdAccesss, this)});
    attributeAccessTests.push_back({std::bind(&NoteModel::testContentAccess, this)});
    attributeAccessTests.push_back({std::bind(&NoteModel::testDateAddedAccess, this)});
    attributeAccessTests.push_back({std::bind(&NoteModel::testLastUpdateAccess, this)});
    attributeAccessTests.push_back({std::bind(&NoteModel::testLastModifiedByUserAccess, this)});

    return attributeAccessTests;
}

std::vector<ExceptionTestElement> NoteModel::initExceptionTests() noexcept
{
    std::vector<ExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&NoteModel::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&NoteModel::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&NoteModel::testExceptionHide, this), "testExceptionHide"});

    return exceptionTests;
}

TestStatus NoteModel::testExceptionInsert() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setContent("Testing insertion exception");
    setUserId(27);
    setCreatedTimeStamp(timeStamp);
    setLastModifiedTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::insert", std::bind(&NoteModel::insert, this));
}

TestStatus NoteModel::testExceptionUpdate() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setNoteId(1);
    setContent("Testing insertion exception");
    setUserId(27);
    setCreatedTimeStamp(timeStamp);
    setLastModifiedTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::update", std::bind(&NoteModel::update, this));
}

TestStatus NoteModel::testExceptionHide() noexcept
{
   selfTestResetAllValues();
   std::size_t testUserId = 27;

    std::chrono::system_clock::time_point timeStamp = common::TestTimeStampValue;
    setNoteId(1);
    setContent("Testing insertion exception");
    setUserId(testUserId);
    setCreatedTimeStamp(timeStamp);
    setLastModifiedTimeStamp(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::hide", std::bind(&NoteModel::hide, this, std::placeholders::_1), testUserId);
}

TestStatus NoteModel::testNoteIdAccesss() noexcept
{
    return testPrimaryKeyAccessFunctions(27,
        std::bind(&NoteModel::setNoteId, this, std::placeholders::_1),
        std::bind(&NoteModel::getNoteId, this));
}

TestStatus NoteModel::testUserIdAccesss() noexcept
{
    return testForeignKeyFields(31, &m_userID, "User ID",
        std::bind(&NoteModel::setUserId, this, std::placeholders::_1),
        std::bind(&NoteModel::getUserId, this));
}

TestStatus NoteModel::testContentAccess() noexcept
{
    return testAccessorFunctions<std::string>("Test note content access", &m_content, "Content",
        std::bind(&NoteModel::setContent, this, std::placeholders::_1),
        std::bind(&NoteModel::getContent, this));
}

TestStatus NoteModel::testDateAddedAccess() noexcept
{
    return testTimeStampAccessorFunctions(common::TestTimeStampValue, &m_createdTimeStamp, "Date Added",
        std::bind(&NoteModel::setCreatedTimeStamp, this, std::placeholders::_1),
        std::bind(&NoteModel::getCreatedTSValue, this));
}

TestStatus NoteModel::testLastUpdateAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = common::TestTimeStampValue;
    return testTimeStampAccessorFunctions(testValue, &m_lastUpdateTimeStamp, "Last Modified Time Stamp",
        std::bind(&NoteModel::setLastModifiedTimeStamp, this, std::placeholders::_1),
        std::bind(&NoteModel::getLastModifiedValue, this));
}

TestStatus NoteModel::testLastModifiedByUserAccess() noexcept
{
    std::size_t testUserId = 1;

    return testLastModifiedByAccess(testUserId);
}

