#ifndef NOTEMODEL_H_
#define NOTEMODEL_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class NoteModel : public ModelDBInterface
{
public:

    NoteModel();
    ~NoteModel() = default;

    std::size_t getNoteId() const { return primaryKey; };
    std::size_t getUserId() const { return userID; };
    std::string getContent() const { return content; };
    std::chrono::system_clock::time_point getDateAdded() const { return creationDate; };
    std::chrono::system_clock::time_point getLastModified() const { return lastUpdate; };

    void setNoteId(std::size_t noteId);
    void setUserId(std::size_t userId);
    void setContent(std::string contentStr);
    void setDateAdded(std::chrono::system_clock::time_point created);
    void setLastModified(std::chrono::system_clock::time_point lastModification);
/*
 * Select with arguments
 */
    bool selectByNoteID(std::size_t noteID);

/*
 * Required fields.
 */
    bool isMissingUserID()  { return userID == 0; };;
    bool isMissingContent() { return (content.empty() || content.size() < 10); };
    bool isMissingCreationDate() { return creationDate.time_since_epoch() != std::chrono::system_clock::duration::zero(); };
    bool isMissingLastUpdate() { return lastUpdate.time_since_epoch() != std::chrono::system_clock::duration::zero(); };
    void initRequiredFields() override;

    bool operator==(NoteModel& other)
    {
        return diffUser(other);
    };
    bool operator==(std::shared_ptr<NoteModel> other)
    {
        return diffUser(*other);
    }

    friend std::ostream& operator<<(std::ostream& os, const NoteModel& note)
    {
        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << std::format(outFmtStr, "Note ID", note.primaryKey);
        os << std::format(outFmtStr, "User ID", note.userID);
        os << std::format(outFmtStr, "Content", note.content);
        os << std::format(outFmtStr, "Created", note.creationDate);
        os << std::format(outFmtStr, "Last Update", note.lastUpdate);

        return os;
    };

private:
    bool diffUser(NoteModel& other);
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatSelectStatement() override;

    void processResultRow(boost::mysql::row_view rv) override;
    
    std::size_t userID;
    std::string content;
    std::chrono::system_clock::time_point creationDate;
    std::chrono::system_clock::time_point lastUpdate;


private:
/*
 * The indexes below are based on the following select statement, maintain this order.
 * baseQuery could be SELECT * FROM UserProfile, but this way the order of the columns
 * returned are known.
 */
    boost::mysql::constant_string_view baseQuery = 
        "SELECT idUserNotes, UserID, NotationDateTime, Content, LastUpdate FROM UserNotes ";

    const std::size_t NoteIdIdx = 0;
    const std::size_t UserIdIdx = 1;
    const std::size_t NotationDateTimeIdx = 2;
    const std::size_t ContentIdx = 3;
    const std::size_t LastUpdateIdx = 4;
};

using NoteModel_shp = std::shared_ptr<NoteModel>;

#endif // NOTEMODEL_H_
