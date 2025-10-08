#ifndef MODELTEMPLACE_H_
#define MODELTEMPLACE_H_

// Project Header Files
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

class TemplateModel : public ModelDBInterface
{
public:

    TemplateModel();
    ~TemplateModel() = default;

// get access methods
    std::size_t getNoteId() const { return primaryKey; };

// set access methods
    void setNoteId(std::size_t noteId);
/*
 * Select with arguments
 */
    bool selectByNoteID(std::size_t noteID);

/*
 * Required fields.
 */
    bool isMissingUserID()  { return userID == 0; };
    void initRequiredFields() override;

    bool operator==(TemplateModel& other)
    {
        return diffUser(other);
    };
    bool operator==(std::shared_ptr<TemplateModel> other)
    {
        return diffUser(*other);
    }

    friend std::ostream& operator<<(std::ostream& os, const TemplateModel& note)
    {
        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << std::format(outFmtStr, "Note ID", note.primaryKey);

        return os;
    };

private:
    bool diffUser(TemplateModel& other);
    void processResultRow(NSBM::row_view rv) override;
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatSelectStatement() override;

    
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
    NSBM::constant_string_view baseQuery = 
        "SELECT idUserNotes, UserID, NotationDateTime, Content, LastUpdate FROM UserNotes ";

    const std::size_t NoteIdIdx = 0;
    const std::size_t UserIdIdx = 1;
    const std::size_t NotationDateTimeIdx = 2;
    const std::size_t ContentIdx = 3;
    const std::size_t LastUpdateIdx = 4;
};

using TemplateModel_shp = std::shared_ptr<TemplateModel>;

#endif // MODELTEMPLACE_H_
