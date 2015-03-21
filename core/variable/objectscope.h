#ifndef OBJECTSCOPEIMPLEMENTATION_H
#define OBJECTSCOPEIMPLEMENTATION_H

#include "core/variable/variable.h"

class Object;

class ObjectScope : public VariableImplementation
{
public:
    ObjectScope(Object& object);

    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;
    virtual void doSetValue(const Variant &value) override;
    virtual Variant doGetValue() override;

private:
    Object& _object;
};

#endif // OBJECTSCOPEIMPLEMENTATION_H
