#include "localscope.h"

#include "core/log/logmanager.h"

LocalScope::LocalScope(const Variable &context)
    : VariableImplementation(context.collector()), _context(context)
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

    return Variable(_context).field(key, modifiable, createIfNeeded);
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
