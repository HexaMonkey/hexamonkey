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

#ifndef STANDARDTYPES_H
#define STANDARDTYPES_H

#include "core/objecttypetemplate.h"
#include "core/objecttype.h"

namespace standardTypes
{
    const ObjectTypeTemplate integer("int",{"size", "_base"});
    const ObjectTypeTemplate uinteger("uint",{"size", "_base"});
    const ObjectTypeTemplate byte("byte");

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
    const ObjectTypeTemplate wstring("WString", {"charCount"});

    const ObjectTypeTemplate bitset("Bitset", {"size"});
}

#endif // STANDARDTYPES_H
