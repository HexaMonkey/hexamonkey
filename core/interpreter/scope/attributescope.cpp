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

    Variant* value = _object.attributeValue(key);
    if (modifiable) {
        if (value) {
            return Variable::ref(*value);
        } else {
            return Variable::ref(_object.setAttributeValue(key, Variant()));
        }
    } else {

        if (value) {
            return Variable::constRef(*value);
        } else {
            return Variable();
        }
    }

    return Variable();
}
