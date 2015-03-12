#ifndef TYPESCOPEIMPLEMENTATION_H
#define TYPESCOPEIMPLEMENTATION_H

#include "core/variable/variable.h"

class Parser;
class ObjectType;

/**
 * @brief Scope implementation that gives access to a \link ObjectType type\endlink's parameters
 *
 * The arguments can either be access by their index or name.
 */
class AbstractTypeScopeImplementation : public VariableImplementation
{
protected:
    virtual Variable doGetField(const Variant &key, bool modifiable) override;

    virtual ObjectType* modifiableType() = 0;
    virtual const ObjectType& constType() = 0;
};

#endif // TYPESCOPEIMPLEMENTATION_H
