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
/*
 * Field access methods. 
 */
    void setPrimaryKey(std::size_t keyValue);
    std::size_t getPrimaryKey() const;
    void addDataField(const std::string& fieldName, PTS_DataField::PTS_DB_FieldType fieldType, bool required=false);
    bool setFieldValue(const std::string& fieldName, DataValueType dataValue);
    void dbSetFieldValue(const std::string& fieldName, DataValueType dataValue);
    std::string getFieldValueString(const std::string& fieldName);
    DataValueType getFieldValue(const std::string& fieldName) const;
    bool fieldHasValue(const std::string& fieldName) const;
    bool fieldWasModified(const std::string& fieldName) const;
/*
 * Object / Class access methods. 
 */
    bool atleastOneFieldModified() const;
    bool allRequiredFieldsHaveData() const;
    friend std::ostream& operator<<(std::ostream& os, const ModelBase& obj)
    {
        os << "Primary Key field name :" << obj.primaryKeyFieldName << "\n";
        for (const auto& [key, value] : obj.dataFields)
        {
            PTS_DataField* currentField = value;
            os << currentField->fieldInfo();
        }
        return os;
    };

protected:
    std::string createDateString(int month, int day, int year);
    std::string dateToString(std::chrono::year_month_day taskDate);
    std::chrono::year_month_day stringToDate(std::string dateString);
    std::chrono::year_month_day getTodaysDate();
    PTS_DataField* findFieldInDataFields(const std::string& fieldName) const;

    const std::string modelClassName;
    const std::string primaryKeyFieldName;
    std::unordered_map<std::string, PTS_DataField*> dataFields;
};

#endif  // MODELBASE_H_


