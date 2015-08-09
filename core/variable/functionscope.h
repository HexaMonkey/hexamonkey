#ifndef FUNCTIONSCOPEIMPLEMENTATION_H
#define FUNCTIONSCOPEIMPLEMENTATION_H

#include "core/variable/variable.h"

#include <unordered_map>
#include <vector>
#include <string>

class FunctionScope : public VariableImplementation
{
public:
    FunctionScope(VariableCollector& collector);

    void addParameter(const Variable& variable);
    void addNamedParameter(const Variable& variable, const std::string& name);

protected:
    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;

private:
    std::vector<VariableMemory> _fields;
    std::unordered_map<std::string, VariableMemory> _namedFields;
};

#endif // FUNCTIONSCOPEIMPLEMENTATION_H
