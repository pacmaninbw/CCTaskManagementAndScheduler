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
        unsigned int, int, double, std::string, bool>;

class PTS_DataField
{
public:
    enum class PTS_DB_FieldType {Key, Date, DateTime, TimeStamp, VarChar45, VarChar256, VarChar1024, TinyText, Text,
        Boolean, UnsignedInt, Int, Size_T, Double
    };
    PTS_DataField(DataValueType inValue);   // Only used for parameters for SELECT statement
    PTS_DataField(PTS_DataField::PTS_DB_FieldType cType, std::string cName, bool isRequired=false);
    PTS_DataField(PTS_DataField::PTS_DB_FieldType cType, std::string cName, DataValueType inValue, bool isRequired=false);
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
    void clearDirtyBit() { modified = false; };

/*
 * Field information methods.
 */
    std::string getColumnName() const { return dbColumnName; };
    PTS_DataField::PTS_DB_FieldType getFieldType() const { return columnType; };
    std::string fieldInfo();
    const std::string typeToName() const;
    bool isStringType() const { return stringType; };

/*
 * Operators
 */
    bool operator==(const PTS_DataField& other) const
    {
        bool areTheSame = (columnType == other.columnType && dbColumnName == other.dbColumnName &&
            dataValue == other.dataValue);

        if (!areTheSame)
        {
            std::clog << "Fields differ: " << dbColumnName << "\n";// << other;
        }

        return areTheSame;
    };
    friend std::ostream& operator<<(std::ostream& os, PTS_DataField& field)
    {
        os << field.fieldInfo() << "\n";
        os << "Column Name: ";
        os << field.dbColumnName << "\tColumn Type: ";
        os << field.typeToName() + "\tRequired: ";
        os << (field.required? "True" : "False");
        os << "\tModified: ";
        os << (field.modified? "True" : "False");
        os << "\tHas value: ";
        os << (field.hasValue()? "True" : "False");

        if (field.hasValue())
        {
            try
            {
                os <<"\t" + field.toString();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }

        return os;
    };


private:
    PTS_DB_FieldType columnType;
    std::string dbColumnName;
    DataValueType dataValue;
    bool required;
    bool modified;
    bool stringType;
};

#endif  // PTS_DATAFIELD_H_

