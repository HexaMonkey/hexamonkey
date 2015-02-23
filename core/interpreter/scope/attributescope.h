#ifndef ATTRIBUTESCOPE_H
#define ATTRIBUTESCOPE_H

#include "core/interpreter/scope/scope.h"

class Object;

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
