#include "typescope.h"


#include "objecttype.h"
#include "objecttypetemplate.h"


MutableTypeScope::MutableTypeScope(ObjectType &type)
    : _type(type)
{
}

Variant *MutableTypeScope::doGet(const Variant &key) const
{
    int i = -1;

    if(key.canConvertTo(Variant::integer))
        i = key.toInteger();
    else if (key.canConvertTo(Variant::string))
        i = _type.typeTemplate().parameterNumber(key.toString());


    if(i != -1)
    {
        return &_type._parametersValue[i];
    }

    return nullptr;
}


ConstTypeScope::ConstTypeScope(const ObjectType &type)
    :_type(type)
{
}

const Variant *ConstTypeScope::doCget(const Variant &key) const
{
    int i = -1;

    if(key.canConvertTo(Variant::integer))
        i = key.toInteger();
    else if (key.canConvertTo(Variant::string))
        i = _type.typeTemplate().parameterNumber(key.toString());


    if(i != -1)
    {
        return &_type._parametersValue[i];
    }

    return nullptr;
}
