#include "PTS_DataField.h"

std::string PTS_DataField::toString()
{
    std::string returnValue;

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

    case PTS_DataField::PTS_DB_FieldType::VarChar45 :
    case PTS_DataField::PTS_DB_FieldType::VarChar1024 :
    case PTS_DataField::PTS_DB_FieldType::TinyText :
    case PTS_DataField::PTS_DB_FieldType::Text :
    case PTS_DataField::PTS_DB_FieldType::TinyBlob :
    case PTS_DataField::PTS_DB_FieldType::Blob :
        returnValue = std::get<std::string>(dataValue);
        break;
    
/*    enum class PTS_DB_FieldType {Key, Date, DateTime, TimeStamp, VarChar45, VarChar1024, TinyText, Text, TinyBlob, Blob,
        Boolean, UnsignedInt, Int, Size_T, Double, Float
    };*/

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

    case PTS_DataField::PTS_DB_FieldType::Float :
        returnValue = std::to_string(std::get<float>(dataValue));
        break;
    }

    return returnValue;
}

void PTS_DataField::setValue(DataValueType inValue)
{
    dataValue = inValue;
    modified = true;
}

bool PTS_DataField::hasValue()
{
    return !std::holds_alternative<std::monostate>(dataValue);
}
