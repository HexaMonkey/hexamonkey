#ifndef OBJECTSCOPEIMPLEMENTATION_H
#define OBJECTSCOPEIMPLEMENTATION_H

#include "core/variable/variable.h"

class Object;

class ObjectScopeImplementation : public VariableImplementation
{
public:
    ObjectScopeImplementation(Object& object);

    Variable doGetField(const Variant &key, bool modifiable);

    void doSetValue(const Variant &value);
    Variant doGetValue();

private:
    Object& _object;
};

#endif // OBJECTSCOPEIMPLEMENTATION_H
