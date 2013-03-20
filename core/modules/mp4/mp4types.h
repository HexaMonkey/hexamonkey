#ifndef MP4TYPES_H
#define MP4TYPES_H

#include "objecttypetemplate.h"
#include "objecttype.h"

namespace mp4Types
{
    const ObjectTypeTemplate Mp4File("Mp4File");

    const ObjectTypeTemplate Box("Box", {"box_type"});
    const ObjectTypeTemplate FullBox("FullBox", {"box_type", "version", "flags"});
    const ObjectTypeTemplate SampleEntry("EntrySample", {"handler_type", "box_type"});

    const ObjectTypeTemplate Date("Date", {"size"});
    const ObjectType Date32(Date(32));
    const ObjectType Date64(Date(64));

    const ObjectTypeTemplate Duration("Duration", {"size", "scale"});

    const ObjectTypeTemplate Language("Language");
}

#endif // MP4TYPES_H
