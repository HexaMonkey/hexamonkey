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
    ObjectScope(std::shared_ptr<Parser*> sharedParserAccess);

    virtual void collect(const VariableAdder &addAccessible) override;
protected:
    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;
    virtual void doSetValue(const Variant &value) override;
    virtual Variant doGetValue() override;

private:
    Object& _object;
    std::shared_ptr< std::pair<bool, ObjectType> > _sharedType;
    std::shared_ptr<Parser*> _sharedParserAccess;
    VariableMemory _parserScope;
};

#endif // OBJECTSCOPEIMPLEMENTATION_H
