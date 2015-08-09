#include "core/variable/functionscope.h"

#include "core/log/logmanager.h"

FunctionScope::FunctionScope(VariableCollector &collector) : VariableImplementation(collector)
{
}

void FunctionScope::addParameter(const Variable& variable)
{
    _fields.push_back(variable);
}

void FunctionScope::addNamedParameter(const Variable& variable, const std::string &name)
{
    _namedFields[name] = variable;
    addParameter(variable);
}

Variable FunctionScope::doGetField(const Variant &key, bool /*modifiable*/, bool createIfNeeded)
{
    if (key.hasNumericalType()) {
        int64_t number = key.toInteger();
        if (number >= 0 || number < _fields.size()) {
            return _fields[number];
        } else {
            if (createIfNeeded) {
                Log::error("Trying to assign a numbered parameter value outside of the bounds");
            }
            return Variable();
        }
    } else if (key.type() == Variant::stringType) {
        const std::string& name = key.toString();
        auto it = _namedFields.find(name);
        if (it != _namedFields.end()) {
            return it->second;
        } else {
            if (createIfNeeded) {
                Log::error("Trying to assign a numbered attribute value outside of the bounds");
            }
            return Variable();
        }
    } else {
        if (createIfNeeded) {
            Log::error("Trying to assign an attribute with an invalid key type");
        }
        return Variable();
    }
}
