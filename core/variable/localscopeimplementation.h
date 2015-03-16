#ifndef LOCALSCOPE_H
#define LOCALSCOPE_H

#include <unordered_map>
#include <string>

#include "core/variable/variable.h"

class LocalScopeImplementation : public VariableImplementation
{
public:
    LocalScopeImplementation(const Variable& context = Variable());

protected:
    virtual Variable doGetField(const Variant &key, bool modifiable) override;
    virtual void doSetField(const Variant &key, const Variable &variable) override;
    virtual void doRemoveField(const Variant &key) override;

private:
    std::unordered_map<std::string, Variable> _fields;
    Variable _context;
};

#endif // LOCALSCOPE_H
