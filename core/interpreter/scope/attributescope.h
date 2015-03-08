#ifndef ATTRIBUTESCOPE_H
#define ATTRIBUTESCOPE_H

#include "core/interpreter/scope/scope.h"

class Object;

/**
 * @brief Scope accessing the attributes of an \link Object object\endlink
 */
class AttributeScope : public Scope
{
public:
    AttributeScope(Object& object);

protected:
    virtual Variable doGet(const Variant &key, bool modifiable) override;

private:
    Object& _object;

};

#endif // ATTRIBUTESCOPE_H
