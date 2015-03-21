#ifndef OBJECTCONTEXT_H
#define OBJECTCONTEXT_H

#include "core/variable/variable.h"

#include <unordered_map>

class Object;

class ObjectContext : public VariableImplementation
{
public:
    ObjectContext(Object& object);

    Variable field(const std::string& key) const;
    Variable field(const std::string& key, bool createIfNeeded);
    void     setField(const std::string& key, const Variable &variable);
    void     removeField(const std::string& key);

protected:
    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;
    virtual void doSetField(const Variant &key, const Variable &variable) override;
    virtual void doRemoveField(const Variant &key) override;

private:
    Object& _object;
    std::unordered_map<std::string, Variable> _fields;
};

#endif // OBJECTCONTEXT_H
