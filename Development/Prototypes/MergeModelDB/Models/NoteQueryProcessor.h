#ifndef NOTEQUERYPROCESSOR_H_
#define NOTEQUERYPROCESSOR_H_

// Project Header Files
//#include "QueryProcessor.h"
#include "CoreDBInterface.h"
#include "NoteModel.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>


// Standard C++ Header Files
#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using NoteList = std::vector<NoteModel_shp>;

struct ColumnNameToIndexmapping
{
    std::string columnName;
    std::optional<std::size_t> columnIndex;
    ColumnNameToIndexmapping(std::string name) : columnName{name}{};
};

class NoteQueryProcessor : public CoreDBInterface
{
public:
    NoteQueryProcessor();
    virtual ~NoteQueryProcessor() = default;
    NoteList getAllNotesForUser(std::size_t userId) noexcept;
    NoteList getNotesForUserSimlarToContent(std::size_t userId, std::string likeContent) noexcept;
    NoteList getAllNotesForUserCreatedInDatgeRange(std::size_t userId,
        std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept;
    NoteList getAllNotesForUserEditedInDatgeRange(std::size_t userId,
        std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) noexcept;
    NoteList getDashboardNoteTable(std::size_t userId, std::chrono::year_month_day searchDate) noexcept;

private:
    std::string formatSelectByUserId(std::size_t userId);
    std::string formatSelectByUserIdAndSimilarContent(std::size_t userId, std::string similarContent);
    std::string formatSelectByUserIdAndCreationDateRange(std::size_t userId, std::chrono::year_month_day startDay,
        std::chrono::year_month_day endDay);
    std::string formatSelectByUserIdAndUpdateDateRange(std::size_t userId, std::chrono::year_month_day startDay,
        std::chrono::year_month_day endDay);
    std::string formatGetNotesFromUserForDate(std::size_t userId, std::chrono::year_month_day searchDate);
    void mapColumnNameToIndex(boost::mysql::resultset_view& noteQueryresultSet);
    void assignValueToIndex(std::string columnName, std::size_t& columnIndex);
    void fillRequiredIndexes();
    NoteList processResults(boost::mysql::results& noteQueryResults);
    NoteList processResultSet(boost::mysql::resultset_view& noteQueryresultSet);
    NoteModel_shp processResultRow(boost::mysql::row_view& noteQueryRow);

    std::vector<ColumnNameToIndexmapping> columnToIndexMap;
    std::vector<std::string> requiredColumns;

    boost::mysql::constant_string_view baseQuery = "SELECT * FROM UserNotes ";

    std::size_t noteIDX = 0xffff;
    std::size_t userIDX = 0xffff;
    std::size_t contentIDX = 0xffff;
    std::size_t hiddenIDX = 0xffff;
    std::size_t createdIDX = 0xffff;
    std::size_t lastmodIDX = 0xffff;


#if  0
    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllNotes() noexcept;
    TestStatus testExceptionsGetNotesForUserLikeContent() noexcept;
    TestStatus testExceptionsGetNotesForUserCreatedDateRange() noexcept;
    TestStatus testExceptionsGetNotesForUserEditedDateRange() noexcept;
    TestStatus testExceptionsGetDashboardNoteTable() noexcept;
#endif
};

#endif // NOTEQUERYPROCESSOR_H_

