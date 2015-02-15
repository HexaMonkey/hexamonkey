#include "contextscope.h"

#include "core/object.h"

ContextScope::ContextScope(Object &object, bool modifiable)
    : _object(object),
      _modifiable(modifiable)
{
}

Variable ContextScope::doGet(const Variant &key, bool modifiable)
{
    modifiable = _modifiable && modifiable;

    Variant* value = _object.contextValue(key);
    if (modifiable) {
        if (value) {
            return Variable::ref(*value);
        } else {
            return Variable::ref(_object.setContextValue(key, Variant()));
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
