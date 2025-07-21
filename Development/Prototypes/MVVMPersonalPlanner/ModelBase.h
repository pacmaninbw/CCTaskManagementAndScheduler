#ifndef MODELBASE_H_
#define MODELBASE_H_

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class ModelBase
{
public:
    ModelBase(const std::string modelName, const std::string tabName, const std::string primaryKeyName, std::size_t primaryKeyIn=0);
    virtual ~ModelBase() = default;
    std::string dateToString(std::chrono::year_month_day taskDate);
    std::chrono::year_month_day stringToDate(std::string dateString);
    bool isInDataBase() const;
    std::string getTableName() const { return tableName; };
    void onInsertionClearDirtyBits();
    bool atleastOneFieldModified() const;
    bool allRequiredFieldsHaveData() const;
    bool diffAllFields(ModelBase other);
    PTS_DataField_vector getAllFieldsWithValue();
    std::string reportMissingRequiredFields() const;
    const std::string getModelName() { return modelClassName; };

/*
 * Operators
 */
    bool operator==(ModelBase other)
    {
        bool areTheSame = true;

        if (modelClassName != other.modelClassName)
        {
            areTheSame = false;
            std::clog << "Model names differ: " << modelClassName << " " << other.modelClassName << "\n";
        }

        return areTheSame;
    };
    friend std::ostream& operator<<(std::ostream& os, const ModelBase& obj)
    {
        os << "Model Class:\t" << obj.modelClassName << "\n";
        return os;
    };

protected:
    std::string createDateString(int month, int day, int year);
    std::chrono::year_month_day getTodaysDate();

    const std::string modelClassName;
    const std::string tableName;
    PTS_DataField_map dataFields;
};

using ModelShp = std::shared_ptr<ModelBase>;
using ModelList = std::vector<ModelShp>;

#endif  // MODELBASE_H_


