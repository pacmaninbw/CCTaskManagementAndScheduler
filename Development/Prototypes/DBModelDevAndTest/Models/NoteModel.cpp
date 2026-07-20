// Project Header Files
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

NoteModel::NoteModel
(
    std::size_t noteId,
    std::size_t userId,
    std::string content,
    std::chrono::system_clock::time_point created,
    std::chrono::system_clock::time_point lastModification,
    bool hidden
)
: NoteModel()
{
    m_primaryKey = noteId;
    m_userID = userId;
    m_content = content;
    m_creationDate = created;
    m_lastUpdate = lastModification;
    m_deleted = hidden;
}

void NoteModel::setNoteId(std::size_t noteId)
{
    m_modified = true;
    m_primaryKey = noteId;
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

void NoteModel::setDateAdded(std::chrono::system_clock::time_point created)
{
    m_modified = true;
    m_creationDate = created;
}

void NoteModel::setLastModified(std::chrono::system_clock::time_point lastModification)
{
    m_modified = true;
    m_lastUpdate = lastModification;
}

bool NoteModel::diffNote(NoteModel &other)
{
    // Ignore user preferences
    return (m_primaryKey == other.m_primaryKey && m_userID == other.m_userID && m_content == other.m_content &&
        m_creationDate == other.m_creationDate && m_lastUpdate == other.m_lastUpdate);
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
    boost::mysql::format_sql_to(fctx, "deleted ");
    boost::mysql::format_sql_to(fctx, ") VALUES (");
    boost::mysql::format_sql_to(fctx, "{}, ", m_userID);
    boost::mysql::format_sql_to(fctx, "{}, ", m_content);
    boost::mysql::format_sql_to(fctx, "{}", m_deleted);
    boost::mysql::format_sql_to(fctx, ")");

    return (std::move(fctx).get().value());
}

std::string NoteModel::formatUpdateStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_notes SET user_notes.content = {} ", m_content);
    boost::mysql::format_sql_to(fctx, "WHERE user_notes.id_user_notes = {} ", m_primaryKey);
    boost::mysql::format_sql_to(fctx, "AND user_notes.user_id = {}", m_userID);

    return (std::move(fctx).get().value());
}

std::string NoteModel::formatDeleteStatement()
{
    boost::mysql::format_context fctx(getFormatOptions());
    boost::mysql::format_sql_to(fctx, "UPDATE user_notes SET user_notes.deleted = 1 ");
    boost::mysql::format_sql_to(fctx, "WHERE user_notes.user_id = {} ", m_userID);
    boost::mysql::format_sql_to(fctx, "AND user_notes.id_user_notes = {}", m_primaryKey);

    return (std::move(fctx).get().value());
}


