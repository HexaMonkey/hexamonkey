#include "mapscope.h"

#include "core/log/logmanager.h"

Variable MapScope::field(const Variant &key) const
{
    auto it = _fields.find(key);

    if (it != _fields.cend()) {
        return it->second;
    } else {
        return Variable();
    }
}

Variable MapScope::field(const Variant &key, bool createIfNeeded)
{
    auto it = _fields.find(key);

    if (it != _fields.cend()) {
        return it->second;
    } else {
        if (createIfNeeded) {
            return _fields[key] = Variable::null();
        } else {
            return Variable();
        }
    }
}

void MapScope::setField(const Variant &key, const Variable &variable)
{
    _fields[key] = variable;
}

Variable MapScope::doGetField(const Variant &key, bool /*modifiable*/, bool createIfNeeded)
{
    return field(key, createIfNeeded);
}

void MapScope::doSetField(const Variant &key, const Variable &variable)
{
    setField(key, variable);
}
