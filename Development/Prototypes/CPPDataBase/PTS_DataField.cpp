#include <chrono>
#include "GenericDictionary.h"
#include <iostream>
#include "PTS_DataField.h"
#include <string>
#include <variant>
#include <vector>
#include <utility>

static std::vector<GenericDictionary<PTS_DataField::PTS_DB_FieldType, std::string>::DictType> translationTableDefs = {
    {PTS_DataField::PTS_DB_FieldType::Key, "Key"},
    {PTS_DataField::PTS_DB_FieldType::Date, "Date"},
    {PTS_DataField::PTS_DB_FieldType::DateTime, "DateTime"},
    {PTS_DataField::PTS_DB_FieldType::TimeStamp, "TimeStamp"},
    {PTS_DataField::PTS_DB_FieldType::VarChar45, "VarChar45"},
    {PTS_DataField::PTS_DB_FieldType::VarChar256, "VarChar256"},
    {PTS_DataField::PTS_DB_FieldType::VarChar1024, "VarChar1024"},
    {PTS_DataField::PTS_DB_FieldType::TinyText, "TinyText"},
    {PTS_DataField::PTS_DB_FieldType::Text, "Text"},
    {PTS_DataField::PTS_DB_FieldType::Boolean, "Boolean"},
    {PTS_DataField::PTS_DB_FieldType::UnsignedInt, "UnsignedInt"},
    {PTS_DataField::PTS_DB_FieldType::Int, "Int"},
    {PTS_DataField::PTS_DB_FieldType::Size_T, "Size_T"},
    {PTS_DataField::PTS_DB_FieldType::Double, "Double"}
};

static GenericDictionary<PTS_DataField::PTS_DB_FieldType, std::string> translationTable(translationTableDefs);

PTS_DataField::PTS_DataField(PTS_DataField::PTS_DB_FieldType cType, std::string cName, bool isRequired)
: columnType{cType}, dbColumnName{cName}, required{isRequired}, modified{false}
{
    dataValue = std::monostate{};

    switch (cType)
    {
        default :
            stringType = false;
            break;
        case PTS_DataField::PTS_DB_FieldType::VarChar45 :
        case PTS_DataField::PTS_DB_FieldType::VarChar256 :
        case PTS_DataField::PTS_DB_FieldType::VarChar1024 :
        case PTS_DataField::PTS_DB_FieldType::TinyText :
        case PTS_DataField::PTS_DB_FieldType::Text :
            stringType = true;
            break;
    }
};

PTS_DataField::PTS_DataField(PTS_DataField::PTS_DB_FieldType cType, std::string cName, DataValueType inValue, bool isRequired)
: PTS_DataField(cType, cName, isRequired)
{
    dataValue = inValue;
};


std::string PTS_DataField::toString()
{
    std::string returnValue;

    if (isStringType())
    {
        return std::get<std::string>(dataValue);
    }

    switch (columnType)
    {
    default:
        std::cerr << "Unknown column type in PTS_DataField::toString()" << static_cast<int>(columnType) << "\n";
        break;

    case PTS_DataField::PTS_DB_FieldType::Date :
        {
            std::stringstream ss;
            ss << std::get<std::chrono::year_month_day>(dataValue);
            returnValue = ss.str();
        }
        break;

    case PTS_DataField::PTS_DB_FieldType::DateTime :
    case PTS_DataField::PTS_DB_FieldType::TimeStamp :
        {
            std::stringstream ss;
            ss << std::format("%F %T", std::get<std::chrono::time_point<std::chrono::system_clock>>(dataValue));
            returnValue = ss.str();
        }
        break;

    case PTS_DataField::PTS_DB_FieldType::Int :
        returnValue = std::to_string(std::get<int>(dataValue));
        break;

    case PTS_DataField::PTS_DB_FieldType::Key :
    case PTS_DataField::PTS_DB_FieldType::Size_T :
        returnValue = std::to_string(std::get<std::size_t>(dataValue));
        break;

    case PTS_DataField::PTS_DB_FieldType::UnsignedInt :
        returnValue = std::to_string(std::get<unsigned int>(dataValue));
        break;

    case PTS_DataField::PTS_DB_FieldType::Double :
        returnValue = std::to_string(std::get<double>(dataValue));
        break;

    case PTS_DataField::PTS_DB_FieldType::Boolean :
        returnValue = (std::get<bool>(dataValue)) ? "1" : "0";
        break;
    }

    return returnValue;
}

void PTS_DataField::setValue(DataValueType inValue)
{
    dataValue = inValue;
    modified = true;
}

bool PTS_DataField::hasValue() const
{
    return !std::holds_alternative<std::monostate>(dataValue);
}

std::string PTS_DataField::fieldInfo()
{
    std::string info("Column Name: ");
    info += dbColumnName + "\tColumn Type: ";
    info += typeToName() + "\tRequired: ";
    info += required? "True" : "False";
    info += "\tModified: ";
    info += modified? "True" : "False";
    info += "\tHas value: ";
    info += hasValue()? "True" : "False";

    if (hasValue())
    {
        try
        {
            info += "\t" + toString();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }

    return info;
}

const std::string PTS_DataField::typeToName() const
{
    PTS_DataField::PTS_DB_FieldType target_key = columnType;
    auto typeName = translationTable.lookupName(target_key);
    return typeName.has_value()? *typeName : std::string();
}

int PTS_DataField::getIntValue() const
{
    if (hasValue() &&
        columnType == PTS_DataField::PTS_DB_FieldType::Int)
    {
        return std::get<int>(dataValue);
    }

    return 0;
}

std::size_t PTS_DataField::getSize_tValue() const
{
    if (hasValue() && (columnType == PTS_DataField::PTS_DB_FieldType::Key
        || columnType == PTS_DataField::PTS_DB_FieldType::Size_T))
    {
        return std::get<std::size_t>(dataValue);
    }

    return 0;
}

double PTS_DataField::getDoubleValue() const
{
    if (hasValue() && columnType == PTS_DataField::PTS_DB_FieldType::Double)
    {
        return std::get<double>(dataValue);
    }

    return 0.0;
}

std::chrono::year_month_day PTS_DataField::getDateValue() const
{
    if (hasValue() && columnType == PTS_DataField::PTS_DB_FieldType::Date)
    {
        return std::get<std::chrono::year_month_day>(dataValue);
    }

    return std::chrono::year_month_day();
}

std::chrono::time_point<std::chrono::system_clock> PTS_DataField::getTimeValue() const
{
    if (hasValue() && (columnType == PTS_DataField::PTS_DB_FieldType::DateTime
        || columnType == PTS_DataField::PTS_DB_FieldType::TimeStamp))
    {
        return std::get<std::chrono::time_point<std::chrono::system_clock>>(dataValue);
    }

    return std::chrono::time_point<std::chrono::system_clock>();
}

std::string PTS_DataField::getStringValue() const
{
    if (hasValue())
    {
        if (isStringType())
        {
            return std::get<std::string>(dataValue);
        }
    }

        return std::string();
}

bool PTS_DataField::getBoolValue() const
{
    if (hasValue() && columnType == PTS_DataField::PTS_DB_FieldType::Boolean)
    {
        return std::get<bool>(dataValue);
    }

    return false;
}

unsigned int PTS_DataField::getUnsignedIntValue() const
{
    if (hasValue() && columnType == PTS_DataField::PTS_DB_FieldType::UnsignedInt)
    {
        return std::get<unsigned int>(dataValue);
    }

    return 0;
}
