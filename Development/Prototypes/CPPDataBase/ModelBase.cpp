#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include "ModelBase.h"
#include "PTS_DataField.h"
#include <string>
#include <unordered_map>
#include <vector>


ModelBase::ModelBase(const std::string modelName, const std::string tabName, std::string primaryKeyName, std::size_t primaryKeyIn)
: modelClassName{modelName}, tableName{tabName}, primaryKeyFieldName{primaryKeyName}
{
    PTS_DataField_shp primaryKey = std::make_shared<PTS_DataField>(PTS_DataField(PTS_DataField::PTS_DB_FieldType::Key, primaryKeyName, true));
    if (primaryKeyIn)
    {
        primaryKey->setValue(primaryKeyIn);
    }
    dataFields.insert({primaryKeyName, primaryKey});
}

std::string ModelBase::dateToString(std::chrono::year_month_day taskDate)
{
    std::stringstream ss;
    ss << taskDate;
    return ss.str();
}

std::chrono::year_month_day ModelBase::stringToDate(std::string dateString)
{
    std::chrono::year_month_day dateValue = getTodaysDate();

    // First try the ISO standard date.
    std::istringstream ss(dateString);
    ss >> std::chrono::parse("%Y-%m-%d", dateValue);
    if (!ss.fail())
    {
        return dateValue;
    }

    // The ISO standard didn't work, try some local dates
    std::locale usEnglish("en_US.UTF-8");
    std::vector<std::string> legalFormats = {
        {"%B %d, %Y"},
        {"%m/%d/%Y"},
        {"%m-%d-%Y"}
    };

    ss.imbue(usEnglish);
    for (auto legalFormat: legalFormats)
    {
        ss >> std::chrono::parse(legalFormat, dateValue);
        if (!ss.fail())
        {
            return dateValue;
        }
    }

    return dateValue;
}

void ModelBase::addDataField(const std::string& fieldName, PTS_DataField::PTS_DB_FieldType fieldType, bool required)
{
    PTS_DataField_shp dataField = std::make_shared<PTS_DataField>(PTS_DataField(fieldType, fieldName, required));
    dataFields.insert({fieldName, dataField});
}

bool ModelBase::isInDataBase() const
{
    PTS_DataField_shp pkField = findFieldInDataFields(primaryKeyFieldName);
    return pkField? pkField->hasValue() : false;
}

void ModelBase::onInsertionClearDirtyBits()
{
    for (const auto& [key, value] : dataFields)
    {
        value->clearDirtyBit();
    }
}

bool ModelBase::setFieldValue(const std::string& fieldName, DataValueType dataValue)
{
    PTS_DataField_shp fieldToUpdate = findFieldInDataFields(fieldName);
    if (fieldToUpdate)
    {
        fieldToUpdate->setValue(dataValue);
        return true;
    }

    return false;
}

/*
 * Does not set the modified flag.
 */
void ModelBase::initFieldValueNotChanged(const std::string &fieldName, DataValueType dataValue)
{
    PTS_DataField_shp fieldToUpdate = findFieldInDataFields(fieldName);
    if (fieldToUpdate)
    {
        fieldToUpdate->dbSetValue(dataValue);
    }
}

std::string ModelBase::getFieldValueString(const std::string& fieldName)
{
    std::string valueString("No Value Set");

    PTS_DataField_shp fieldToFind = findFieldInDataFields(fieldName);
    if (fieldToFind)
    {
        valueString = fieldToFind->toString();
    }

    return valueString;
}

DataValueType ModelBase::getFieldValue(const std::string& fieldName) const
{
    DataValueType dataValue;

    PTS_DataField_shp fieldToFind = findFieldInDataFields(fieldName);
    if (fieldToFind)
    {
        if (fieldToFind->hasValue())
        {
            return fieldToFind->getValue();
        }
    }

    return dataValue;
}

bool ModelBase::fieldHasValue(const std::string& fieldName) const
{
    PTS_DataField_shp fieldToFind = findFieldInDataFields(fieldName);
    if (fieldToFind)
    {
        return fieldToFind->hasValue();
    }

    return false;
}

bool ModelBase::fieldWasModified(const std::string &fieldName) const
{
    PTS_DataField_shp fieldToFind = findFieldInDataFields(fieldName);
    if (fieldToFind)
    {
        return fieldToFind->wasModified();
    }

    return false;
}

void ModelBase::setPrimaryKey(std::size_t keyValue)
{
    initFieldValueNotChanged(primaryKeyFieldName, keyValue);
}

std::size_t ModelBase::getPrimaryKey() const
{
    std::size_t primaryKey = 0;

    PTS_DataField_shp  primaryKeyField = findFieldInDataFields(primaryKeyFieldName);
    if (primaryKeyField)
    {
        if (primaryKeyField->hasValue())
        {
            DataValueType dataValue = primaryKeyField->getValue();
            primaryKey = std::get<std::size_t>(dataValue);
        }
    }

    return primaryKey;
}

bool ModelBase::atleastOneFieldModified() const
{
    for (const auto& [key, value] : dataFields)
    {
        if (value->wasModified())
        {
            return true;
        }
    }

    return false;
}

bool ModelBase::allRequiredFieldsHaveData() const
{
    for (const auto& [key, value] : dataFields)
    {
        /*
         * If this is a new object that hasn't been entered into the database yet
         * then the primary key won't have a value. Whether to insert a new record
         * or update an existing record is determined in the database interface. 
         */
        if (value->isRequired() && key != primaryKeyFieldName)
        {
            if (!value->hasValue())
            {
                return false;
            }
        }
    }

    return true;
}

std::string ModelBase::reportMissingRequiredFields() const
{
    std::string agregateErrorMessage;

    for (const auto& [key, value] : dataFields)
    {
        if (value->isRequired() && key != primaryKeyFieldName)
        {
            if (!value->hasValue())
            {
                agregateErrorMessage += "The required field " + value->getColumnName() +
                    " has not been set!\n";
            }
        }
    }

    return agregateErrorMessage;
}

/*
 * Protected methods.
 */
std::string ModelBase::createDateString(int month, int day, int year)
{
    std::string dateString = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);

    return dateString;
}

std::chrono::year_month_day ModelBase::getTodaysDate()
{
    std::chrono::time_point<std::chrono::system_clock> today = std::chrono::system_clock::now();
    return std::chrono::floor<std::chrono::days>(today);
}

PTS_DataField_shp ModelBase::findFieldInDataFields(const std::string &fieldName) const
{
    auto fieldToFind = dataFields.find(fieldName);
    if (fieldToFind != dataFields.end())
    {
        return fieldToFind->second;
    }
    else
    {
        std::string eMessage("Interal Error in " + modelClassName + ": ");
        eMessage += "Field not found in dataFields: " + fieldName;
        std::out_of_range oor(eMessage);
        throw oor;
    }

    return nullptr;
}

PTS_DataField_vector ModelBase::getAllFieldsWithValue()
{
    std::vector<PTS_DataField_shp> fields;

    for (const auto& [key, value] : dataFields)
    {
        PTS_DataField_shp currentField = value;
        if (currentField->hasValue())
        {
            fields.push_back(currentField);
        }
    }

    return fields;
}

bool ModelBase::diffAllFields(ModelBase other)
{
    bool areTheSame = true;

    for (const auto& [key, value] : dataFields)
    {
        const PTS_DataField_shp otherValue = other.findFieldInDataFields(key);
        if (*value != *otherValue)
        {
            areTheSame = false;
            std::clog << "Fields differ: " << key << "\n\t" << *value << "\n\t" << *otherValue << "\n";
        }
    }

    return areTheSame;
}

