#include "objectcontext.h"

#include "core/object.h"
#include "core/log/logmanager.h"

ObjectContext::ObjectContext(Object &object)
    : _object(object)
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

Variable ObjectContext::field(const std::string &key, bool modifiable)
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

    if (modifiable) {
        return _fields[key] = Variable::null();
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

Variable ObjectContext::doGetField(const Variant &key, bool modifiable)
{
    if (key.type() != Variant::stringType) {
        if (modifiable) {
            Log::error("Context scope only support string keys");
        }
        return Variable();
    }

    return field(key.toString(), modifiable);
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
