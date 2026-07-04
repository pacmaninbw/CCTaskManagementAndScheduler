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

std::string NoteModel::formatInsertStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(m_formatOpts.value(), "CALL AddUserNote({0}, {1})", m_userID, m_content);
}

std::string NoteModel::formatUpdateStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(m_formatOpts.value(),
        "CALL UpdateNoteAllFields({0}, {1}, {2})", m_userID, m_primaryKey, m_content);
}

std::string NoteModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(m_formatOpts.value(), "CALL HideNote({}, {})", m_userID, m_primaryKey);
}


