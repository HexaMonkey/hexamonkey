#ifndef MKVTYPES_H
#define MKVTYPES_H

#include "core/objecttypetemplate.h"
#include "core/objecttype.h"


namespace mkvTypes
{
    const ObjectTypeTemplate EBMLElement("EBMLElement", Variant::integer, "id");

    const ObjectTypeTemplate largeInteger("LargeInteger");
    const ObjectTypeTemplate Date("Date");
}

#endif // MKVTYPES_H
