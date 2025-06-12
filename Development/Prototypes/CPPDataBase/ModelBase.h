#ifndef MODELBASE_H_
#define MODELBASE_H_

#include <chrono>
#include <iostream>
#include "PTS_DataField.h"
#include <string>
#include <unordered_map>

class ModelBase
{
public:
    ModelBase(const std::string modelName, const std::string primaryKeyName, std::size_t primaryKeyIn=0);
    virtual ~ModelBase();
    std::string dateToString(std::chrono::year_month_day taskDate);
    std::chrono::year_month_day stringToDate(std::string dateString);
/*
 * Field access methods. 
 */
    void setPrimaryKey(std::size_t keyValue);
    std::size_t getPrimaryKey() const;
    void addDataField(const std::string& fieldName, PTS_DataField::PTS_DB_FieldType fieldType, bool required=false);
    bool setFieldValue(const std::string& fieldName, DataValueType dataValue);
    void initFieldValueNotChanged(const std::string& fieldName, DataValueType dataValue);
    std::string getFieldValueString(const std::string& fieldName);
    DataValueType getFieldValue(const std::string& fieldName) const;
    bool fieldHasValue(const std::string& fieldName) const;
    bool fieldWasModified(const std::string& fieldName) const;
    int getIntFieldValue(const std::string& fieldName) const {
        PTS_DataField* fieldToFind = findFieldInDataFields(fieldName);
        return fieldToFind? fieldToFind->getIntValue() : 0;
    };
    std::size_t getSize_tFieldValue(const std::string& fieldName) const
    {
        PTS_DataField* fieldToFind = findFieldInDataFields(fieldName);
        return fieldToFind? fieldToFind->getSize_tValue() : 0;
    };
    std::size_t getKeyFieldValue(const std::string& fieldName) const { return getSize_tFieldValue(fieldName); };
    double getDoubleFieldValue(const std::string& fieldName) const
    {
        PTS_DataField* fieldToFind = findFieldInDataFields(fieldName);
        return fieldToFind ? fieldToFind->getDoubleValue() : 0.0;
    };
    float getFloatFieldValue(const std::string& fieldName) const
    {
        PTS_DataField* fieldToFind = findFieldInDataFields(fieldName);
        return fieldToFind ? fieldToFind->getFloatValue() : 0.0;
    };
    std::chrono::year_month_day getDateFieldValue(const std::string& fieldName) const
    {
        std::chrono::year_month_day badDate;
        PTS_DataField* fieldToFind = findFieldInDataFields(fieldName);
        return fieldToFind ? fieldToFind->getDateValue() : badDate;
    };
    std::chrono::time_point<std::chrono::system_clock> getTimeFieldValue(const std::string& fieldName) const
    {
        std::chrono::time_point<std::chrono::system_clock> badTime;
        PTS_DataField* fieldToFind = findFieldInDataFields(fieldName);
        return fieldToFind ? fieldToFind->getTimeValue() : badTime;
    };
    std::string getStringFieldValue(const std::string& fieldName) const
    {
        PTS_DataField* fieldToFind = findFieldInDataFields(fieldName);
        return fieldToFind ? fieldToFind->getStringValue() : "";
    };
    bool getBoolFieldValue(const std::string& fieldName) const
    {
        PTS_DataField* fieldToFind = findFieldInDataFields(fieldName);
        return fieldToFind ? fieldToFind->getBoolValue() : false;
    };
    unsigned int getUnsignedIntFieldValue(const std::string& fieldName) const
    {
        PTS_DataField* fieldToFind = findFieldInDataFields(fieldName);
        return fieldToFind ? fieldToFind->getUnsignedIntValue() : 0;
    }
/*
 * Object / Class access methods. 
 */
    bool atleastOneFieldModified() const;
    bool allRequiredFieldsHaveData() const;
    std::string reportMissingRequiredFields() const;
    friend std::ostream& operator<<(std::ostream& os, const ModelBase& obj)
    {
        os << "Primary Key field name :" << obj.primaryKeyFieldName << "\n";
        for (const auto& [key, value] : obj.dataFields)
        {
            PTS_DataField* currentField = value;
            os << currentField->fieldInfo();
            os << "\n";
        }
        return os;
    };

protected:
    std::string createDateString(int month, int day, int year);
    std::chrono::year_month_day getTodaysDate();
    PTS_DataField* findFieldInDataFields(const std::string& fieldName) const;

    const std::string modelClassName;
    const std::string primaryKeyFieldName;
    std::unordered_map<std::string, PTS_DataField*> dataFields;
};

#endif  // MODELBASE_H_


