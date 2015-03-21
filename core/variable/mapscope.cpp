#include "mapscope.h"

#include "core/log/logmanager.h"

Variable MapScope::field(const std::string &key) const
{
    auto it = _fields.find(key);

    if (it != _fields.cend()) {
        return it->second;
    } else {
        return Variable();
    }
}

Variable MapScope::field(const std::string &key, bool createIfNeeded)
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

void MapScope::setField(const std::string &key, const Variable &variable)
{
    _fields[key] = variable;
}

Variable MapScope::doGetField(const Variant &key, bool /*modifiable*/, bool createIfNeeded)
{
    if (key.type() != Variant::stringType) {
        if (createIfNeeded) {
            Log::error("Map scope only support string keys");
        }
        return Variable();
    } else {
        return field(key.toString(), createIfNeeded);
    }
}

void MapScope::doSetField(const Variant &key, const Variable &variable)
{
    if (key.type() != Variant::stringType) {
        Log::error("Map scope only support string keys");
    } else {
        setField(key.toString(), variable);
    }
}
