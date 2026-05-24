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
    primaryKey = 0;
    modelName = modelNameIn;
    primaryKeyName = primaryKeyNameIn;
    modified = false;
    delimiter = ';';  
    deleted = false;
}

bool ModelDBInterface::save() noexcept
{
    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified, no changes to save", modelName));
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
    errorMessages.clear();

    if (isInDataBase())
    {
        appendErrorMessage(std::format("{} already in Database, use Update!", modelName));
        return false;
    }

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", modelName));
        return false;
    }

    if (!hasRequiredValues())
    {
        appendErrorMessage(std::format("{} is missing required values!", modelName));
        reportMissingFields();
        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatInsertStatement());
        primaryKey =  getPrimaryKeyValue(localResult);
        modified = false;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.insert : {}", modelName, e.what()));
        return false;
    }

    return true;
}

bool ModelDBInterface::update() noexcept
{
    errorMessages.clear();

    if (!isInDataBase())
    {
        appendErrorMessage(std::format("{} not in Database, use Insert!", modelName));
        return false;
    }

    if (!isModified())
    {
        appendErrorMessage(std::format("{} not modified!", modelName));
        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatUpdateStatement());
        modified = false;
            
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.update : {}", modelName, e.what()));
        return false;
    }
}

bool ModelDBInterface::hide([[maybe_unused]]std::size_t userRequestingDelete) noexcept
{
    errorMessages.clear();

    if (!isInDataBase())
    {
        appendErrorMessage(std::format("{} not in Database, nothing to delete!", modelName));

        return false;
    }

    try
    {
        boost::mysql::results localResult = runQueryAsync(formatDeleteStatement());

        deleted = true;
        
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.hide() : {}", modelName, e.what()));
        return false;
    }
}


bool ModelDBInterface::hasRequiredValues()
{
    initRequiredFields();
    for (auto fieldTest: missingRequiredFieldsTests)
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
    for (auto testAndReport: missingRequiredFieldsTests)
    {
        if (testAndReport.errorCondition())
        {
            appendErrorMessage(std::format("Missing {} required {}!", modelName, testAndReport.fieldName));
        }
    }
}

std::vector<std::string> ModelDBInterface::explodeTextField(std::string const& textField) noexcept
{
    std::vector<std::string> subFields;
    std::istringstream iss(textField);

    for (std::string token; std::getline(iss, token, delimiter); )
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
        textField += delimiter;
    }

    return textField;
}

std::size_t ModelDBInterface::getPrimaryKeyValue(boost::mysql::results &dbResultSet)
{
    // If this is self test then we don't actually connect to the database, the code that
    // tests insert statements still needs a value returned.
    if (inSelfTest)
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

            if (columnNames[0] == primaryKeyName)
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

