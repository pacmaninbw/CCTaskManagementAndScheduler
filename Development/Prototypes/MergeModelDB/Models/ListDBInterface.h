#ifndef LISTDBINTERFACECORE_H_
#define LISTDBINTERFACECORE_H_

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <concepts>
#include "CoreDBInterface.h"
#include <iostream>
#include <memory>
#include "ModelDBInterface.h"
#include <string>
#include <string_view>
#include <vector>

/*
 * Templated Class to handle model select queries with multiple results.
 * 
 * The model itself is used to create the select statement. The first query will
 * only return the primary key of each instance of the model that matches the
 * search parameters of the query. Once the list of primary keys is established
 * the model will be used to select the object by primary key for the full data.
 * 
 * This is done primarily to keep the table related information for a model in
 * the model. A second reason for this design is that the is a maximum size that
 * a boost::mysql::results class can reach; to reduce the possiblity of reaching
 * that maximum queries that can return a large amount of data will be reduced
 * to returning only the primary keys of the data.
 * 
 * This design may be revisited if there is too much a performance degradation.
 */
template<typename ListType>
requires std::is_base_of<ModelDBInterface, ListType>::value
class ListDBInterface : public CoreDBInterface
{
public:
    ListDBInterface()
    : CoreDBInterface()
    {
        std::string tempListType(queryGenerator.getModelName());
        tempListType += "List";
        listTypeName = tempListType;
    }
    virtual ~ListDBInterface() = default;

    std::string_view getListTypeName() const noexcept { return listTypeName; };
    
    bool runFirstQuery()
    {
        prepareForRunQueryAsync();

        try
        {
            primaryKeyResults.clear();
            NSBM::results localResult = runQueryAsync(firstFormattedQuery);
            primaryKeyResults = processFirstQueryResults(localResult);

            return primaryKeyResults.size() > 0;
        }

        catch(const std::exception& e)
        {
            appendErrorMessage(std::format("In {}List.runFirstQuery() : {}",
                queryGenerator.getModelName(), e.what()));
            return false;
        }
    };

protected:
    void setFirstQuery(std::string formattedQueryStatement) noexcept
    {
        firstFormattedQuery = formattedQueryStatement;
    }
    
    virtual std::vector<std::size_t> processFirstQueryResults(NSBM::results& results)
    {
        if (results.rows().empty())
        {
            appendErrorMessage(std::format("No {}s found!", queryGenerator.getModelName()));
            return primaryKeyResults;
        }

        for (auto row: results.rows())
        {
            primaryKeyResults.push_back(row.at(0).as_uint64());
    
        }
        return primaryKeyResults;
    }

    ListType queryGenerator;
    std::string_view listTypeName;
    std::string firstFormattedQuery;
    std::vector<std::size_t> primaryKeyResults;
    std::vector<std::shared_ptr<ListType>> returnType;
};

#endif // LISTDBINTERFACECORE_H_


