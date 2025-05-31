#ifndef MODELBASE_H_
#define MODELBASE_H_

#include <cstddef>

class ModelBase
{
public:
    ModelBase(std::size_t fieldCount, std::size_t primaryKeyIn=0);
    virtual ~ModelBase();
    virtual void setPrimaryKey(std::size_t keyValue) { primaryKey = keyValue; };
    virtual std::size_t getPrimaryKey() const { return primaryKey; };

protected:
    std::size_t numberOfFields;
    std::size_t fieldsHaveBeenUpdated;
    std::size_t updatedFieldsMask;
    std::size_t primaryKey;
};

#endif  // MODELBASE_H_


