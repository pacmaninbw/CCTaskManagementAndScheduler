// Project Header Files
#include "modelTemplate.h"

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

TemplateModel::TemplateModel()
: ModelDBInterface("TemplateModel")
{
    userID = 0;
    creationDate = std::chrono::system_clock::now();
    lastUpdate = creationDate;
}

void TemplateModel::setNoteId(std::size_t noteId)
{
    modified = true;
    primaryKey = noteId;
}


bool TemplateModel::diffUser(TemplateModel &other)
{
    // Ignore user preferences
    return (primaryKey == other.primaryKey && userID == other.userID);
}

void TemplateModel::initRequiredFields()
{
    missingRequiredFieldsTests.push_back({std::bind(&TemplateModel::isMissingUserID, this), "User ID"});
}

std::string TemplateModel::formatInsertStatement()
{
    initFormatOptions();

    std::string insertStatement = boost::mysql::format_sql(format_opts.value(),
        "INSERT INTO UserNotes (UserID, NotationDateTime, Content, LastUpdate) VALUES ({0}, {1}, {2}, {3})",
        userID, stdChronoTimePointToBoostDateTime(creationDate),
        content, stdChronoTimePointToBoostDateTime(lastUpdate));

    return insertStatement;
}

std::string TemplateModel::formatUpdateStatement()
{
    initFormatOptions();

    std::string updateStatement = boost::mysql::format_sql(format_opts.value(),
        "UPDATE UserProfile SET"
            " UserNotes.UserID = {0},"
            " UserNotes.LastUpdate = {3}" 
        " WHERE UserNotes.idUserNotes = {4}",
            userID, stdChronoTimePointToBoostDateTime(creationDate),
            content, stdChronoTimePointToBoostDateTime(lastUpdate));
        
    return updateStatement;
}

std::string TemplateModel::formatSelectStatement()
{
    initFormatOptions();

    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, baseQuery);
    boost::mysql::format_sql_to(fctx, " WHERE UserID = {}", primaryKey);

    return std::move(fctx).get().value();
}


void TemplateModel::processResultRow(boost::mysql::row_view rv)
{
    primaryKey = rv.at(NoteIdIdx).as_uint64();
    userID = rv.at(UserIdIdx).as_uint64();
    creationDate = boostMysqlDateTimeToChronoTimePoint(rv.at(NotationDateTimeIdx).as_datetime());
    content = rv.at(ContentIdx).as_string();
    lastUpdate = boostMysqlDateTimeToChronoTimePoint(rv.at(LastUpdateIdx).as_datetime());
}

bool TemplateModel::selectByNoteID(std::size_t noteID)
{
    prepareForRunQueryAsync();

    try
    {
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, baseQuery);
        boost::mysql::format_sql_to(fctx, " WHERE idUserNotes = {}", noteID);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TemplateModel::selectByUserID : {}", e.what()));
        return false;
    }
}
