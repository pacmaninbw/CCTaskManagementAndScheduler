#include "ModelBase.h"

ModelBase::ModelBase(std::size_t fieldCount, std::size_t primaryKeyIn)
: numberOfFields{fieldCount},
  fieldsHaveBeenUpdated{0},
  updatedFieldsMask{0},
  primaryKey{primaryKeyIn}
{
    for (std::size_t i = 0; i < fieldCount; ++i)
    {
        updatedFieldsMask |= 1 << i;
    }
}

ModelBase::~ModelBase()
{

}