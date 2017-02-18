#include "objectcontext.h"

#include "core/object.h"
#include "core/log/logmanager.h"
#include "core/variable/variablecollector.h"

ObjectContext::ObjectContext(Object &object)
    : VariableImplementation(object.collector()), _object(object)
{
}

Variable ObjectContext::field(const std::string &key) const
{
    for (const Object* object = &_object; object != nullptr; object->parent()) {
        const ObjectContext* context = object->context();
        if (context != nullptr) {
            const auto it = context->_fields.find(key);
            if (it != context->_fields.cend()) {
                return it->second;
            }
        }
    }

    return Variable();
}

Variable ObjectContext::field(const std::string &key, bool createIfNeeded)
{
    for (Object* object = &_object; object != nullptr; object = object->parent()) {
        ObjectContext* context = object->context();
        if (context != nullptr) {
            auto it = context->_fields.find(key);
            if (it != context->_fields.end()) {
                return it->second;
            }
        }
    }

    if (createIfNeeded) {
        return _fields[key] = collector().null();
    } else {
        return Variable();
    }
}

void ObjectContext::setField(const std::string &key, const Variable &variable)
{
    _fields[key] = variable;
}

void ObjectContext::removeField(const std::string &key)
{
    _fields.erase(key);
}

Variable ObjectContext::doGetField(const Variant &key, bool /*modifiable*/, bool createIfNeeded)
{
    if (key.type() != Variant::stringType) {
        if (createIfNeeded) {
            Log::error("Context scope only support string keys");
        }
        return Variable();
    }

    return field(key.toString(), createIfNeeded);
}

void ObjectContext::doSetField(const Variant &key, const Variable &variable)
{
    if (key.type() != Variant::stringType) {
        Log::error("Context scope only support string keys");
        return;
    }

    setField(key.toString(), variable);
}

void ObjectContext::doRemoveField(const Variant &key)
{
    if (key.type() != Variant::stringType) {
        Log::error("Context scope only support string keys");
        return;
    }

    removeField(key.toString());
}

void ObjectContext::collect(const std::function<void(VariableMemory&)>& addAccessible)
{
    for (auto& entry : _fields) {
        addAccessible(entry.second);
    }
}

bool ObjectContext::isByRefOnly()
{
    return true;
}
