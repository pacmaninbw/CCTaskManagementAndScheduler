// Project Header Files
#include "ModelDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <exception>
//#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

ModelDBInterface::ModelDBInterface(std::string modelNameIn, std::string primaryKeyNameIn)
: CoreDBInterface()
{
    m_primaryKey = 0;
    m_modelName = modelNameIn;
    m_primaryKeyName = primaryKeyNameIn;
    m_modified = false;
    m_delimiter = ';';  
    m_deleted = false;
}

bool ModelDBInterface::save() noexcept
{
    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified, no changes to save", m_modelName));
        return false;
    }

    if (isInDataBase())
    {
        return update();
    }
    else
    {
        return insert();
    }
}

bool ModelDBInterface::insert() noexcept
{
    clearErrorMessages();

    if (isInDataBase())
    {
        appendErrorMessage(std::format("{} already in Database, use Update!", m_modelName));
        return false;
    }

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", m_modelName));
        return false;
    }

    if (!hasRequiredValues())
    {
        appendErrorMessage(std::format("{} is missing required values!", m_modelName));
        reportMissingFields();
        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatInsertStatement());
        m_primaryKey =  getPrimaryKeyValue(localResult);
        m_modified = false;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.insert : {}", m_modelName, e.what()));
        return false;
    }

    return true;
}

bool ModelDBInterface::update() noexcept
{
    clearErrorMessages();

    if (!isInDataBase())
    {
        appendErrorMessage(std::format("{} not in Database, use Insert!", m_modelName));
        return false;
    }

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", m_modelName));
        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatUpdateStatement());
        m_modified = false;
            
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.update : {}", m_modelName, e.what()));
        return false;
    }
}

bool ModelDBInterface::hide([[maybe_unused]]std::size_t userRequestingDelete) noexcept
{
    clearErrorMessages();

    if (!isInDataBase())
    {
        appendErrorMessage(std::format("{} not in Database, nothing to delete!", m_modelName));

        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatDeleteStatement());

        m_deleted = true;
        
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.hide() : {}", m_modelName, e.what()));
        return false;
    }
}


bool ModelDBInterface::hasRequiredValues()
{
    initRequiredFields();
    for (auto fieldTest: m_missingRequiredFieldsTests)
    {
        if (fieldTest.errorCondition())
        {
            return false;
        }
    }
    
    return true;
}

/*
 * Assumes that ModelDBInterface::hasRequiredValues() was called previously and that
 * initRequiredFields() has been called.
 */
void ModelDBInterface::reportMissingFields() noexcept
{
    for (auto testAndReport: m_missingRequiredFieldsTests)
    {
        if (testAndReport.errorCondition())
        {
            appendErrorMessage(std::format("Missing {} required {}!", m_modelName, testAndReport.fieldName));
        }
    }
}

std::vector<std::string> ModelDBInterface::explodeTextField(std::string const& textField) noexcept
{
    std::vector<std::string> subFields;
    std::istringstream iss(textField);

    for (std::string token; std::getline(iss, token, m_delimiter); )
    {
        subFields.push_back(std::move(token));
    }
    return subFields;
}

std::string ModelDBInterface::implodeTextField(std::vector<std::string> &fields) noexcept
{
    std::string textField;

    for (auto field: fields)
    {
        textField.append(field);
        textField += m_delimiter;
    }

    return textField;
}

std::size_t ModelDBInterface::getPrimaryKeyValue(boost::mysql::results &dbResultSet)
{
    // If this is self test then we don't actually connect to the database, the code that
    // tests insert statements still needs a value returned.
    if (m_selfTest)
    {
        return 1;
    }

    // Old method: boost::mysql executes last_insert_id and returns the value, this
    // this doesn't work with stored procedures, the stored procedure returns
    // a value in the result set with the primary key name. 
    std::size_t pKeyValue = dbResultSet.last_insert_id();

    if (pKeyValue == 0)
    {
        boost::mysql::resultset_view resultview0 = dbResultSet[0];
        if (resultview0.rows().num_columns() > 0)
        {
            std::vector<std::string> columnNames;
            for (auto metaIter: resultview0.meta())
            {
                columnNames.push_back(metaIter.column_name());
            }

            if (columnNames[0] == m_primaryKeyName)
            {
                boost::mysql::field_view fv = resultview0.rows().at(0).at(0);
                if (fv.kind() == boost::mysql::field_kind::uint64)
                {
                    pKeyValue = fv.as_uint64();
                }
            }
        }
    }

    return pKeyValue;
}

