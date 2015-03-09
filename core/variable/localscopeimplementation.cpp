#include "localscopeimplementation.h"

#include "core/log/logmanager.h"

LocalScopeImplementation::LocalScopeImplementation(const Variable &context)
    : _context(context)
{
}

Variable LocalScopeImplementation::doGetField(const Variant &key, bool modifiable)
{
    if (key.type() != Variant::stringType) {
        if (modifiable) {
            Log::error("Local scope only support string keys");
        }
        return Variable();
    }

    auto it = _fields.find(key.toString());
    if (it != _fields.end()) {
        return it->second;
    } else {
        if (modifiable) {
            Log::error("Local scope fields must be declared with \"var\"");
        }
        return Variable();
    }
}

void LocalScopeImplementation::doSetField(const Variant &key, const Variable &variable)
{
    if (key.type() != Variant::stringType) {
        Log::error("Local scope only support string keys");
        return;
    }

    _fields[key.toString()] = variable;
}

void LocalScopeImplementation::doRemoveField(const Variant &key)
{
    if (key.type() != Variant::stringType) {
        Log::error("Local scope only support string keys");
        return;
    }

    _fields.erase(key.toString());
}
