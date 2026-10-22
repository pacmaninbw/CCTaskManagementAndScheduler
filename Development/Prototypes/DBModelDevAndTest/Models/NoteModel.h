#ifndef NOTEMODEL_H_
#define NOTEMODEL_H_

// Project Header Files
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

/*
 * Used to retrieve values from the database by the boost library. The names
 * of the variables in the struct must match the names of the columns in
 * the user goal table in the database because the boost reflection library is 
 * used to retrieve them.
 */
struct NoteDbQueryValues
{
    std::uint64_t id_user_notes;
    std::uint64_t user_id;
    std::string content;
    std::int64_t deleted;
    boost::mysql::datetime note_creation;
    boost::mysql::datetime last_modifed;
    std::size_t last_modified_by_user;
};

class NoteModel : public ModelDBInterface
{
public:

    NoteModel();
    NoteModel(const NoteDbQueryValues& dbTranslator);
    ~NoteModel() = default;

    std::size_t getNoteId() const { return m_primaryKey; };
    std::size_t getUserId() const { return m_userID; };
    std::string getContent() const { return m_content; };

    void setNoteId(std::size_t noteId) noexcept {setPrimaryKey(noteId, true); };
    void setUserId(std::size_t userId);
    void setContent(std::string content);
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

    friend std::ostream& operator<<(std::ostream& os, const NoteModel& note)
    {
        os << note.commonCoreVariablesOutput();

        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << std::format(outFmtStr, "User ID", note.m_userID);
        os << std::format(outFmtStr, "Content", note.m_content);

        return os;
    };

protected:
    bool diffNote(NoteModel& other);
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    virtual std::string formatDeleteStatement() override;

/*
 * Unit Testing (Self Testing)
 */
    virtual void selfTestResetAllValues() noexcept override;
    virtual TestStatus testAllInsertFailures() override;
    virtual TestStatus testEqualityOperator() noexcept override;
    virtual void testOutput() noexcept override;

    virtual std::vector<AttributeTestFunction> initAttributeAccessTests() noexcept override;

    virtual std::vector<ExceptionTestElement> initExceptionTests() noexcept override;
    virtual TestStatus testExceptionInsert() noexcept override;
    virtual TestStatus testExceptionUpdate() noexcept override;
    virtual TestStatus testExceptionHide() noexcept override;

    TestStatus testNoteIdAccesss() noexcept;
    TestStatus testUserIdAccesss() noexcept;
    TestStatus testContentAccess() noexcept;
    TestStatus testDateAddedAccess() noexcept;
    TestStatus testLastUpdateAccess() noexcept;
    TestStatus testLastModifiedByUserAccess() noexcept;

/*
 * Model specific attributes
 */    
    std::size_t m_userID;
    std::string m_content;
};

using NoteModel_shp = std::shared_ptr<NoteModel>;

#endif // NOTEMODEL_H_
