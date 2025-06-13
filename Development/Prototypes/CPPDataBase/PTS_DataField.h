#ifndef PTS_DATAFIELD_H_
#define PTS_DATAFIELD_H_

/*
 * PTS_DataField: 
 * This class will be used by the models to store each of the data values. 
 * This class will be used by the model to database interface classes to provide
 * the database data. 
 */
#include <chrono>
#include <iostream>
#include <string>
#include <variant>

using DataValueType = std::variant<std::monostate, std::size_t, std::chrono::time_point<std::chrono::system_clock>, std::chrono::year_month_day, 
        unsigned int, int, double, float, std::string, bool>;

class PTS_DataField
{
public:
    enum class PTS_DB_FieldType {Key, Date, DateTime, TimeStamp, Time, VarChar45, VarChar256, VarChar1024, TinyText, Text, TinyBlob, Blob,
        Boolean, UnsignedInt, Int, Size_T, Double, Float
    };

    /*
     * Currently the closest we can get to a default constructor. Field type and column
     * name must be set. 
     */
    PTS_DataField(PTS_DataField::PTS_DB_FieldType cType, std::string cName, bool isRequired=false)
        : columnType{cType}, dbColumnName{cName}, required{isRequired}, modified{false}
        {
            dataValue = std::monostate{};
        };
    PTS_DataField(PTS_DataField::PTS_DB_FieldType cType, std::string cName, DataValueType inValue, bool isRequired=false)
        : columnType{cType}, dbColumnName{cName}, dataValue{inValue}, required{isRequired}, modified{false}
        {};
    ~PTS_DataField() = default;
/*
 * Data access methods.
 */
    std::string toString();
    void setValue(DataValueType inValue);
    void dbSetValue(DataValueType inValue) { dataValue = inValue; };    // Don't set modified.
    DataValueType getValue() { return dataValue; };
    bool hasValue() const;
    bool wasModified() const { return modified; };
    bool isRequired() const { return required; };
    int getIntValue() const;
    std::size_t getSize_tValue() const;
    std::size_t getKeyValue() const { return getSize_tValue(); };
    double getDoubleValue() const;
    float getFloatValue() const;
    std::chrono::year_month_day getDateValue() const;
    std::chrono::time_point<std::chrono::system_clock> getTimeValue() const;
    std::string getStringValue() const;
    bool getBoolValue() const;
    unsigned int getUnsignedIntValue() const;

/*
 * Field information methods.
 */
    std::string getColumnName() const { return dbColumnName; };
    PTS_DataField::PTS_DB_FieldType getFieldType() const { return columnType; };
    std::string fieldInfo();
    const std::string typeToName() const;

private:
    PTS_DB_FieldType columnType;
    std::string dbColumnName;
    DataValueType dataValue;
    bool required;
    bool modified;
};

#endif  // PTS_DATAFIELD_H_

