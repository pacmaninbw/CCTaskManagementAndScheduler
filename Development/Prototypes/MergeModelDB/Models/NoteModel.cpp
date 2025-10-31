// Project Header Files
#include "commonUtilities.h"
#include "NoteModel.h"

// Standard C++ Header Files
#include <exception>
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

NoteModel::NoteModel()
: ModelDBInterface("Note")
{
    userID = 0;
    creationDate = std::chrono::system_clock::now();
    lastUpdate = creationDate;
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
    missingRequiredFieldsTests.push_back({std::bind(&NoteModel::isMissingCreationDate, this), "Date Created"});
    missingRequiredFieldsTests.push_back({std::bind(&NoteModel::isMissingLastUpdate, this), "Last Update"});
}

std::string NoteModel::formatInsertStatement()
{
    initFormatOptions();

    std::string insertStatement = boost::mysql::format_sql(format_opts.value(),
        "INSERT INTO UserNotes (UserID, NotationDateTime, Content, LastUpdate) VALUES ({0}, {1}, {2}, {3})",
        userID, stdChronoTimePointToBoostDateTime(creationDate.value()),
        content, stdChronoTimePointToBoostDateTime(lastUpdate.value()));

    return insertStatement;
}

std::string NoteModel::formatUpdateStatement()
{
    initFormatOptions();

    std::string updateStatement = boost::mysql::format_sql(format_opts.value(),
        "UPDATE UserNotes SET"
            " UserNotes.UserID = {0},"
            " UserNotes.NotationDateTime = {1},"
            " UserNotes.Content = {2},"
            " UserNotes.LastUpdate = {3}" 
        " WHERE UserNotes.idUserNotes = {4}",
            userID, stdChronoTimePointToBoostDateTime(creationDate.value()),
            content, stdChronoTimePointToBoostDateTime(lastUpdate.value()), primaryKey);
        
    return updateStatement;
}

std::string NoteModel::formatSelectStatement()
{
    initFormatOptions();

    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, baseQuery);
    boost::mysql::format_sql_to(fctx, " WHERE UserID = {}", primaryKey);

    return std::move(fctx).get().value();
}


void NoteModel::processResultRow(boost::mysql::row_view rv)
{
    primaryKey = rv.at(NoteIdIdx).as_uint64();
    userID = rv.at(UserIdIdx).as_uint64();
    creationDate = boostMysqlDateTimeToChronoTimePoint(rv.at(NotationDateTimeIdx).as_datetime());
    content = rv.at(ContentIdx).as_string();
    lastUpdate = boostMysqlDateTimeToChronoTimePoint(rv.at(LastUpdateIdx).as_datetime());
}

bool NoteModel::selectByNoteID(std::size_t noteID)
{
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, baseQuery);
        boost::mysql::format_sql_to(fctx, " WHERE idUserNotes = {}", noteID);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In NoteModel::selectByUserID : {}", e.what()));
        return false;
    }
}
