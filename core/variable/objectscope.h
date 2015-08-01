#ifndef OBJECTSCOPEIMPLEMENTATION_H
#define OBJECTSCOPEIMPLEMENTATION_H

#include <memory>

#include "core/variable/variable.h"
#include "core/parser.h"

class Object;

class ObjectScope : public VariableImplementation
{
public:
    ObjectScope(Object& object);
    ObjectScope(Object& object, const std::shared_ptr<Parser>& parser);

    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;
    virtual void doSetValue(const Variant &value) override;
    virtual Variant doGetValue() override;

private:
    Object& _object;
    std::shared_ptr<Parser> _parser;
};

#endif // OBJECTSCOPEIMPLEMENTATION_H
