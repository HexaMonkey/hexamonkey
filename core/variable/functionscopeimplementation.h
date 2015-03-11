#ifndef FUNCTIONSCOPEIMPLEMENTATION_H
#define FUNCTIONSCOPEIMPLEMENTATION_H

#include "core/variable/variable.h"

#include <unordered_map>
#include <vector>
#include <string>

class FunctionScopeImplementation : public VariableImplementation
{
public:
    void addParameter(Variable variable);
    void addNamedParameter(Variable variable, const std::string& name);

protected:
    virtual Variable doGetField(const Variant &key, bool modifiable) override;

private:
    std::vector<Variable> _fields;
    std::unordered_map<std::string, Variable> _namedFields;
};

#endif // FUNCTIONSCOPEIMPLEMENTATION_H
