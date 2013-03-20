#ifndef MKVTYPES_H
#define MKVTYPES_H

#include "objecttypetemplate.h"
#include "objecttype.h"


namespace mkvTypes
{
    const ObjectTypeTemplate EBMLElement("EBMLElement", Variant::integer, "id");

    const ObjectTypeTemplate largeInteger("LargeInteger");
    const ObjectTypeTemplate Date("Date");
}

#endif // MKVTYPES_H
