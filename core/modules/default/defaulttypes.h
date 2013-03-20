#ifndef DEFAULTTYPES_H
#define DEFAULTTYPES_H

#include "objecttypetemplate.h"
#include "objecttype.h"

namespace defaultTypes
{
    const ObjectTypeTemplate file("File");

    const ObjectTypeTemplate array("Array",{"elementType", "size"});

    const ObjectTypeTemplate tuple("Tuple",{"elementType", "count"});
}

#endif // DEFAULTTYPES_H
