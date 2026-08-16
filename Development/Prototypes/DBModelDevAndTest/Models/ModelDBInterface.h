#ifndef MODELDBINTERFACECORE_H_
#define MODELDBINTERFACECORE_H_

// Project Header Files
#include "CoreDBInterface.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <functional>
#include <memory>
#include <string>
#include <vector>

class ModelDBInterface : public CoreDBInterface
{
public:
    ModelDBInterface(std::string modelNameIn, std::string primaryKeyNameIn);
    virtual ~ModelDBInterface() = default;
    virtual bool save() noexcept;
    virtual bool insert() noexcept;
    virtual bool update() noexcept;
    virtual bool hide(std::size_t userRequestingDelete) noexcept;
    bool isInDataBase() const noexcept { return (m_primaryKey > 0); };
    bool isModified() const noexcept { return m_modified; };
    void clearModified() { m_modified = false; };
    bool hasRequiredValues();
    void reportMissingFields() noexcept;
    std::string getModelName() { return m_modelName; };
    bool isDeleted() const noexcept { return m_deleted; };
    std::size_t getPrimaryKey() const noexcept { return m_primaryKey; };

protected:
/*
 * Each model will have 1 or more required fields, the model must specify what those
 * fields are.
 */
    virtual void initRequiredFields() = 0;
/*
 * Each model must provide formating for Delete, Insert, Update and Select by
 * object ID. Additional select statements will be handled by each model as necessary.
 */
    virtual std::string formatInsertStatement() = 0;
    virtual std::string formatUpdateStatement() = 0;
    virtual std::string formatDeleteStatement() = 0;

/*
 * Get the primary key value after a record is inserted in the database.
 */
    std::size_t getPrimaryKeyValue(boost::mysql::results& dbResultSet);
/*
 * Pre database action checks.
 */
    bool preInsertCheck() noexcept;
    bool preUpdateCheck() noexcept;

 /*
  * common protected variables used by all Models.
  */
    std::size_t m_primaryKey;
    std::string m_modelName;
    std::string m_primaryKeyName;
    bool m_modified;
    bool m_deleted;
    char m_delimiter;
    struct RequireField
    {
        std::function<bool(void)> errorCondition;
        std::string fieldName;
    };
    std::vector<RequireField> m_missingRequiredFieldsTests;
};

using AnyModel_shp = std::shared_ptr<ModelDBInterface>;

#endif // MODELDBINTERFACECORE_H_


