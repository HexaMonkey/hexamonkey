#ifndef LOCALSCOPE_H
#define LOCALSCOPE_H

#include <unordered_map>
#include <string>

#include "core/variable/variable.h"

class LocalScope : public VariableImplementation
{
public:
    LocalScope(const Variable& context);

    virtual void collect(const VariableAdder &addAccessible) override;
protected:
    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;
    virtual void doSetField(const Variant &key, const Variable &variable) override;
    virtual void doRemoveField(const Variant &key) override;

private:
    std::unordered_map<std::string, VariableMemory> _fields;
    VariableMemory _context;
};

#endif // LOCALSCOPE_H
