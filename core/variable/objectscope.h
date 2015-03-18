#ifndef OBJECTSCOPEIMPLEMENTATION_H
#define OBJECTSCOPEIMPLEMENTATION_H

#include "core/variable/variable.h"

class Object;

class ObjectScope : public VariableImplementation
{
public:
    ObjectScope(Object& object);

    Variable doGetField(const Variant &key, bool modifiable);

    void doSetValue(const Variant &value);
    Variant doGetValue();

private:
    Object& _object;
};

#endif // OBJECTSCOPEIMPLEMENTATION_H
