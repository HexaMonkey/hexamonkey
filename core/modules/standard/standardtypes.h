#ifndef STANDARDTYPES_H
#define STANDARDTYPES_H

#include "objecttypetemplate.h"
#include "objecttype.h"

namespace standardTypes
{
    const ObjectTypeTemplate integer("int",{"size", "base"});
    const ObjectTypeTemplate uinteger("uint",{"size", "base"});

    const ObjectType int8(integer(8));
    const ObjectType int16(integer(16));
    const ObjectType int32(integer(32));
    const ObjectType int64(integer(64));

    const ObjectType uint8(uinteger(8));
    const ObjectType uint16(uinteger(16));
    const ObjectType uint32(uinteger(32));
    const ObjectType uint64(uinteger(64));

    const ObjectTypeTemplate fixedFloat("fixedFloat", {"integer","decimal"});
    const ObjectTypeTemplate singleFloat("float");
    const ObjectTypeTemplate doubleFloat("double");

    const ObjectType fixed16(fixedFloat(8,8));
    const ObjectType fixed32(fixedFloat(16,16));

    const ObjectTypeTemplate string("String", {"charCount"});

    const ObjectTypeTemplate bitset("Bitset", {"size"});

    const ObjectTypeTemplate data("Data", {"size"});
}

#endif // STANDARDTYPES_H
