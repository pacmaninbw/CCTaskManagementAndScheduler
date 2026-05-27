// Project Header Files
#include "NoteModel.h"

// Standard C++ Header Files
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <vector>

NoteModel::NoteModel()
: ModelDBInterface("Note", "idUserNotes"), userID{0}
{
}

NoteModel::NoteModel
(
    std::size_t nId,
    std::size_t uId,
    std::string newContent,
    std::chrono::system_clock::time_point created,
    std::chrono::system_clock::time_point lastModification,
    bool hidden
)
: NoteModel()
{
    primaryKey = nId;
    userID = uId;
    content = newContent;
    creationDate = created;
    lastUpdate = lastModification;
    deleted = hidden;
}

void NoteModel::setNoteId(std::size_t noteId)
{
    modified = true;
    primaryKey = noteId;
}

void NoteModel::setUserId(std::size_t userId)
{
    modified = true;
    userID = userId;
}

void NoteModel::setContent(std::string contentStr)
{
    modified = true;
    content = contentStr;
}

void NoteModel::setDateAdded(std::chrono::system_clock::time_point created)
{
    modified = true;
    creationDate = created;
}

void NoteModel::setLastModified(std::chrono::system_clock::time_point lastModification)
{
    modified = true;
    lastUpdate = lastModification;
}

bool NoteModel::diffNote(NoteModel &other)
{
    // Ignore user preferences
    return (primaryKey == other.primaryKey && userID == other.userID && content == other.content &&
        creationDate == other.creationDate && lastUpdate == other.lastUpdate);
}

void NoteModel::initRequiredFields()
{
    missingRequiredFieldsTests.push_back({std::bind(&NoteModel::isMissingUserID, this), "User ID"});
    missingRequiredFieldsTests.push_back({std::bind(&NoteModel::isMissingContent, this), "Content"});
}

std::string NoteModel::formatInsertStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(), "CALL AddUserNote({0}, {1})", userID, content);
}

std::string NoteModel::formatUpdateStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(),
        "CALL UpdateNoteAllFields({0}, {1}, {2})", userID, primaryKey, content);
}

std::string NoteModel::formatDeleteStatement()
{
    initFormatOptions();

    return boost::mysql::format_sql(format_opts.value(), "CALL HideNote({}, {})", userID, primaryKey);
}


