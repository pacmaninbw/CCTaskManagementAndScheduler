#ifndef MODELBASE_H_
#define MODELBASE_H_

#include <utility>

class ModelBase
{
public:
    ModelBase(std::size_t fieldCount, std::size_t primaryKeyIn=0);
    ~ModelBase();
    void setPrimaryKey(std::size_t keyValue) { primaryKey = keyValue; };
    std::size_t getPrimaryKey() const { return primaryKey; };

protected:
    std::size_t numberOfFields;
    std::size_t fieldsHaveBeenUpdated;
    std::size_t updatedFieldsMask;
    std::size_t primaryKey;
};

#endif  // MODELBASE_H_