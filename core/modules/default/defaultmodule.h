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

#ifndef DEFAULTMODULE_H
#define DEFAULTMODULE_H

#include "core/mapmodule.h"
#include "core/log/logmanager.h"

#include "core/objecttypetemplate.h"

class DefaultModule : public MapModule
{
public:
    static const ObjectTypeTemplate file;
    static const ObjectTypeTemplate array;
    static const ObjectTypeTemplate tuple;
    static const ObjectTypeTemplate data;
    static const ObjectTypeTemplate structType;
    static const ObjectTypeTemplate enumType;

    static const ObjectTypeTemplate integer;
    static const ObjectTypeTemplate uinteger;
    static const ObjectTypeTemplate byte;

    static const ObjectType int8;
    static const ObjectType int16;
    static const ObjectType int32;
    static const ObjectType int64;

    static const ObjectType uint8;
    static const ObjectType uint16;
    static const ObjectType uint32;
    static const ObjectType uint64;

    static const ObjectTypeTemplate uuid;

    static const ObjectTypeTemplate fixedFloat;
    static const ObjectTypeTemplate singleFloat;
    static const ObjectTypeTemplate doubleFloat;

    static const ObjectTypeTemplate string;
    static const ObjectTypeTemplate wstring;

    static const ObjectTypeTemplate bitset;

protected:
    bool doLoad() override;
};

#endif // DEFAULTMODULE_H
