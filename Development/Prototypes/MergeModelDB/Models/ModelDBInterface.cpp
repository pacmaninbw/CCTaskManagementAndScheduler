#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "ModelDBInterface.h"
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

ModelDBInterface::ModelDBInterface(std::string_view modelNameIn)
: CoreDBInterface()
{
    primaryKey = 0;
    modelName = modelNameIn;
    modified = false;
    delimiter = ';';  
}

bool ModelDBInterface::save()
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

bool ModelDBInterface::insert()
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

    prepareForRunQueryAsync();

    try
    {
        NSBM::results localResult = runQueryAsync(formatInsertStatement());
        primaryKey = localResult.last_insert_id();
        modified = false;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.insert : {}", modelName, e.what()));
        return false;
    }

    return true;
}

bool ModelDBInterface::update()
{
    prepareForRunQueryAsync();

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

        NSBM::results localResult = runQueryAsync(formatUpdateStatement());
        modified = false;
            
        return true;
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.update : {}", modelName, e.what()));
        return false;
    }
}

bool ModelDBInterface::retrieve()
{
    prepareForRunQueryAsync();

    try
    {
        NSBM::results localResult = runQueryAsync(formatSelectStatement());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In {}.retrieve() : {}", modelName, e.what()));
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
            appendErrorMessage(std::format("Missing {} required {}!\n", modelName, testAndReport.fieldName));
        }
    }
}

bool ModelDBInterface::processResult(NSBM::results& results)
{
    if (results.rows().empty())
    {
        appendErrorMessage(std::format("{} not found!", modelName));
        return false;
    }

    if (results.rows().size() > 1)
    {
        appendErrorMessage(std::format("Too many {}s found to process!", modelName));
        return false;
    }

    NSBM::row_view rv = results.rows().at(0);
    processResultRow(rv);

    return true;
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


