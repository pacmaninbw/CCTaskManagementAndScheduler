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
    enum class PTS_DB_FieldType {Key, Date, DateTime, TimeStamp, VarChar45, VarChar1024, TinyText, Text, TinyBlob, Blob,
        Boolean, UnsignedInt, Int, Size_T, Double, Float
    };

    /*
     * Currently the closes we can get to a default constructor. Field type and column
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
    DataValueType getValue() { return dataValue; };
    bool hasValue();
    bool wasModified() const { return modified; };
/*
 * Field information methods.
 */
    std::string getColumnName() const { return dbColumnName; };
    PTS_DataField::PTS_DB_FieldType getFieldType() const { return columnType; };
    std::string fieldInfo();
    std::string typeToName();

private:
    PTS_DB_FieldType columnType;
    std::string dbColumnName;
    DataValueType dataValue;
    bool required;
    bool modified;
};

#endif  // PTS_DATAFIELD_H_

