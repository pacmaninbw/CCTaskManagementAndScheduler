#ifndef MODELDBINTERFACECORE_H_
#define MODELDBINTERFACECORE_H_

// Project Header Files
#include "CoreDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <chrono>
#include <concepts>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class ModelDBInterface : public CoreDBInterface
{
public:
    ModelDBInterface(std::string modelNameIn);
    virtual ~ModelDBInterface() = default;
    bool save() noexcept;
    bool insert() noexcept;
    bool update() noexcept;
    bool retrieve() noexcept;    // Only select object by object ID.
    bool isInDataBase() const noexcept { return (primaryKey > 0); };
    bool isModified() const noexcept { return modified; };
    void clearModified() { modified = false; };
    bool hasRequiredValues();
    void reportMissingFields() noexcept;
    std::string getModelName() { return modelName; };

protected:
/*
 * Each model will have 1 or more required fields, the model must specify what those
 * fields are.
 */
    virtual void initRequiredFields() = 0;
/*
 * Each model must provide formating for Insert, Update and Select by object ID.
 * Additional select statements will be handled by each model as necessary.
 */
    virtual std::string formatInsertStatement() = 0;
    virtual std::string formatUpdateStatement() = 0;
    virtual std::string formatSelectStatement() = 0;
    virtual bool processResult(boost::mysql::results& results);
/*
 * Each model must provide the process by which the database information will
 * be translated into the specific model.
 */
    virtual void processResultRow(boost::mysql::row_view rv) = 0;

/*
 * for MySQL pattern match of string
 */
    std::string wrapSearchContentSQLPatternMatch(std::string searchString) noexcept;

/*
 * To process TEXT fields that contain model fields.
 */
    std::vector<std::string> explodeTextField(std::string const& textField) noexcept;
    std::string implodeTextField(std::vector<std::string>& fields) noexcept;

/*
 * Conversions from std::chrono to boost::mysql
 */
    boost::mysql::date stdchronoDateToBoostMySQLDate(const std::chrono::year_month_day& source) noexcept;
    std::chrono::year_month_day boostMysqlDateToChronoDate(const boost::mysql::date& source) noexcept;
    boost::mysql::datetime stdChronoTimePointToBoostDateTime(std::chrono::system_clock::time_point source) noexcept;
    std::chrono::system_clock::time_point boostMysqlDateTimeToChronoTimePoint(boost::mysql::datetime dbDateTime);
    std::optional<boost::mysql::date> optionalDateConversion(std::optional<std::chrono::year_month_day> optDate);
    std::optional<boost::mysql::datetime> optionalDateTimeConversion(std::optional<std::chrono::system_clock::time_point> optDateTime);

protected:
    std::size_t primaryKey;
    std::string modelName;
    bool modified;
    char delimiter;
    struct RequireField
    {
        std::function<bool(void)>errorCondition;
        std::string fieldName;
    };
    std::vector<RequireField> missingRequiredFieldsTests;
};

using AnyModel_shp = std::shared_ptr<ModelDBInterface>;

#endif // MODELDBINTERFACECORE_H_


