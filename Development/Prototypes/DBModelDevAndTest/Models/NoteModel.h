#ifndef NOTEMODEL_H_
#define NOTEMODEL_H_

// Project Header Files
#include "CommandLineParser.h"
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

class NoteModel : public ModelDBInterface
{
public:

    NoteModel();
    NoteModel(
        std::size_t noteId,
        std::size_t userId,
        std::string content,
        std::chrono::system_clock::time_point created,
        std::chrono::system_clock::time_point lastModification,
        bool deleted = false
    );
    ~NoteModel() = default;

    std::size_t getNoteId() const { return m_primaryKey; };
    std::size_t getUserId() const { return m_userID; };
    std::string getContent() const { return m_content; };
    std::chrono::system_clock::time_point getDateAdded() const { return m_creationDate.value(); };
    std::chrono::system_clock::time_point getLastModified() const { return m_lastUpdate.value(); };

    void setNoteId(std::size_t noteId);
    void setUserId(std::size_t userId);
    void setContent(std::string content);
    void setDateAdded(std::chrono::system_clock::time_point created);
    void setLastModified(std::chrono::system_clock::time_point lastModification);
/*
 * Required fields.
 */
    bool isMissingUserID()  { return m_userID == 0; };;
    bool isMissingContent() { return (m_content.empty() || m_content.size() < 10); };
    void initRequiredFields() override;

    bool operator==(NoteModel& other)
    {
        return diffNote(other);
    };
    bool operator==(std::shared_ptr<NoteModel> other)
    {
        return diffNote(*other);
    }

    friend std::ostream& operator<<(std::ostream& os, const NoteModel& note)
    {
        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << std::format(outFmtStr, "Note ID", note.m_primaryKey);
        os << std::format(outFmtStr, "User ID", note.m_userID);
        os << std::format(outFmtStr, "Content", note.m_content);
        if (programOptions.showTimeStamps)
        {
            if (note.m_creationDate.has_value())
            {
                os << std::format(outFmtStr, "Created", note.m_creationDate.value());
            }
            if (note.m_lastUpdate.has_value())
            {
                os << std::format(outFmtStr, "Last Update", note.m_lastUpdate.value());
            }
        }

        return os;
    };

protected:
    bool diffNote(NoteModel& other);
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    virtual std::string formatDeleteStatement() override;
    
    std::size_t m_userID;
    std::string m_content;
    std::optional<std::chrono::system_clock::time_point> m_creationDate;
    std::optional<std::chrono::system_clock::time_point> m_lastUpdate;
};

using NoteModel_shp = std::shared_ptr<NoteModel>;

#endif // NOTEMODEL_H_
