#include "ModelBase.h"

ModelBase::ModelBase(std::string primaryKeyName, std::size_t primaryKeyIn)
: primaryKeyFieldName{primaryKeyName}
{
    PTS_DataField* primaryKey = new PTS_DataField(PTS_DataField::PTS_DB_FieldType::Key, primaryKeyName, true);
    if (primaryKeyIn)
    {
        primaryKey->setValue(primaryKeyIn);
    }
    dataFields.insert({primaryKeyName, primaryKey});
}

ModelBase::~ModelBase()
{

}

void ModelBase::addDataField(std::string& fieldName, PTS_DataField::PTS_DB_FieldType fieldType, bool required)
{
    PTS_DataField* dataField = new PTS_DataField(fieldType, fieldName, required);
    dataFields.insert({fieldName, dataField});
}

bool ModelBase::setFieldValue(std::string fieldName, DataValueType dataValue)
{
    auto fieldToUpdate = dataFields.find(fieldName);
    if (fieldToUpdate != dataFields.end())
    {
        fieldToUpdate->second->setValue(dataValue);
        return true;
    }
    else
    {
        std::cerr << "Field not found in dataFields: " << fieldName << "\n";
        return false;
    }
}

std::string ModelBase::getFieldValueString(std::string fieldName)
{
    std::string valueString("No Value Set");

    auto fieldToFind = dataFields.find(fieldName);
    if (fieldToFind != dataFields.end())
    {
        if (fieldToFind->second->hasValue())
        {
            valueString = fieldToFind->second->toString();
        }
    }
    else
    {
        std::cerr << "Field not found in dataFields: " << fieldName << "\n";
    }

    return valueString;
}

void ModelBase::setPrimaryKey(std::size_t keyValue)
{
    if (!setFieldValue(primaryKeyFieldName, keyValue))
    {
        std::cerr << "Internal Error, primary key not found in dataFields\n";
    }
}

std::size_t ModelBase::getPrimaryKey() const
{
    std::size_t primaryKey = 0;

    auto primaryKeyField = dataFields.find(primaryKeyFieldName);
    if (primaryKeyField != dataFields.end())
    {
        if (primaryKeyField->second->hasValue())
        {
            DataValueType dataValue = primaryKeyField->second->getValue();
            primaryKey = std::get<std::size_t>(dataValue);
        }
    }
    else
    {
        std::cerr << "Internal Error, primary key not found in dataFields\n";
    }

    return primaryKey;
}

DataValueType ModelBase::getFieldValue(std::string fieldName)
{
    DataValueType dataValue;
    auto fieldToFind = dataFields.find(fieldName);
    if (fieldToFind != dataFields.end())
    {
        if (fieldToFind->second->hasValue())
        {
            dataValue = fieldToFind->second->getValue();
        }
    }
    else
    {
        std::cerr << "Field not found in dataFields: " << fieldName << "\n";
    }

    return dataValue;
}

bool ModelBase::fieldHasValue(std::string fieldName)
{
    bool hasValue = false;

    auto fieldToFind = dataFields.find(fieldName);
    if (fieldToFind != dataFields.end())
    {
        hasValue = fieldToFind->second->hasValue();
    }
    else
    {
        std::cerr << "Field not found in dataFields: " << fieldName << "\n";
    }

    return hasValue;
}
