#ifndef MODELBASE_H_
#define MODELBASE_H_

#include "PTS_DataField.h"
#include <string>
#include <unordered_map>

class ModelBase
{
public:
    ModelBase(std::string primaryKeyName, std::size_t primaryKeyIn=0);
    virtual ~ModelBase();
    void setPrimaryKey(std::size_t keyValue);
    std::size_t getPrimaryKey() const;
    void addDataField(std::string& fieldName, PTS_DataField::PTS_DB_FieldType fieldType, bool required=false);
    [[nodiscard]] bool setFieldValue(std::string fieldName, DataValueType dataValue);
    std::string getFieldValueString(std::string fieldName);
    DataValueType getFieldValue(std::string fieldName);
    bool fieldHasValue(std::string fieldName);

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
    std::string primaryKeyFieldName;
    std::unordered_map<std::string, PTS_DataField*> dataFields;
};

#endif  // MODELBASE_H_


