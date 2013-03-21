//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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
