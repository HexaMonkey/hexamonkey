#include "attributescope.h"

#include "core/object.h"

AttributeScope::AttributeScope(Object &object, bool modifiable)
    : _object(object),
      _modifiable(modifiable)
{
}

Variable AttributeScope::doGet(const Variant &key, bool modifiable)
{
    modifiable = _modifiable && modifiable;

    if (modifiable) {
        return Variable::ref(_object.setAttributeValue(key, Variant()));
    } else {
        Variant* value = _object.attributeValue(key);
        if (value) {
            return Variable::constRef(*value);
        } else {
            return Variable();
        }
    }

    return Variable();
}
