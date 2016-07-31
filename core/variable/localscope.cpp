#include "localscope.h"

#include "core/log/logmanager.h"
#include "core/module.h"

LocalScope::LocalScope(const Variable &context, const Module &module)
    : VariableImplementation(context.collector()),
      _context(context),
      _module(module)
{
}

void LocalScope::collect(const VariableAdder &addAccessible)
{
    addAccessible(_context);
    for (auto& entry : _fields) {
        addAccessible(entry.second);
    }
}

Variable LocalScope::doGetField(const Variant &key, bool modifiable, bool createIfNeeded)
{
    if (key.type() == Variant::stringType) {
        auto it = _fields.find(key.toString());
        if (it != _fields.end()) {
            return it->second;
        }
    }

    const Variable variable = Variable(_context).field(key, modifiable, createIfNeeded);
    if (key.type() != Variant::Type::stringType || variable.isDefined()) {
        return variable;
    } else {
        const std::string& name = key.toString();
        const Variable moduleVariable = _module.getVariable(name, collector());
        if (moduleVariable.isDefined()) {
            _fields[name] = moduleVariable;
        }
        return moduleVariable;
    }

}

void LocalScope::doSetField(const Variant &key, const Variable &variable)
{
    if (key.type() != Variant::stringType) {
        Log::error("Local scope only support string keys");
        return;
    }

    _fields[key.toString()] = variable;
}

void LocalScope::doRemoveField(const Variant &key)
{
    if (key.type() != Variant::stringType) {
        Log::error("Local scope only support string keys");
        return;
    }

    _fields.erase(key.toString());
}
