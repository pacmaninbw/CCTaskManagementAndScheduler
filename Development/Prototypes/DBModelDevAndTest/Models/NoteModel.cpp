// Project Header Files
#include "NoteModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <vector>

NoteModel::NoteModel()
: ModelDBInterface("Note", "idUserNotes"), m_UserID{0}
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
    m_PrimaryKey = noteId;
    m_UserID = userId;
    m_Content = content;
    m_CreationDate = created;
    m_LastUpdate = lastModification;
    m_Deleted = hidden;
}

void NoteModel::setNoteId(std::size_t noteId)
{
    m_Modified = true;
    m_PrimaryKey = noteId;
}

void NoteModel::setUserId(std::size_t userId)
{
    m_Modified = true;
    m_UserID = userId;
}

void NoteModel::setContent(std::string content)
{
    m_Modified = true;
    m_Content = content;
}

void NoteModel::setDateAdded(std::chrono::system_clock::time_point created)
{
    m_Modified = true;
    m_CreationDate = created;
}

void NoteModel::setLastModified(std::chrono::system_clock::time_point lastModification)
{
    m_Modified = true;
    m_LastUpdate = lastModification;
}

bool NoteModel::diffNote(NoteModel &other)
{
    // Ignore user preferences
    return (m_PrimaryKey == other.m_PrimaryKey && m_UserID == other.m_UserID && m_Content == other.m_Content &&
        m_CreationDate == other.m_CreationDate && m_LastUpdate == other.m_LastUpdate);
}

void NoteModel::initRequiredFields()
{
    m_MissingRequiredFieldsTests.push_back({std::bind(&NoteModel::isMissingUserID, this), "User ID"});
    m_MissingRequiredFieldsTests.push_back({std::bind(&NoteModel::isMissingContent, this), "Content"});
}

std::string NoteModel::formatInsertStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(), "CALL AddUserNote({0}, {1})", m_UserID, m_Content);
}

std::string NoteModel::formatUpdateStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),
        "CALL UpdateNoteAllFields({0}, {1}, {2})", m_UserID, m_PrimaryKey, m_Content);
}

std::string NoteModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(), "CALL HideNote({}, {})", m_UserID, m_PrimaryKey);
}


