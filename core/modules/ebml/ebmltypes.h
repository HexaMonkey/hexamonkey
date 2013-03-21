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

#ifndef EBMLTYPES_H
#define EBMLTYPES_H

#include "objecttypetemplate.h"
#include "objecttype.h"


namespace ebmlTypes
{
    const ObjectTypeTemplate EBMLFile("EBMLFile");
    const ObjectTypeTemplate EBMLElement("EBMLElement", {"id"});
    const ObjectTypeTemplate largeInteger("LargeInteger");
    const ObjectTypeTemplate Date("Date");

    const int numberOfTypeElements = 8;
    const std::string typeElements[] = {"MasterElement","IntegerElement","UIntegerElement","FloatElement","StringElement","UTF8StringElement","DateElement","BinaryElement"};
    const std::string typeElementAtributes[] = {"master","integer","uinteger","float","string","utf-8","date","binary"};

    const int numberOfDefaultElements = 8;
    const std::string defaultElements[] = {"EBML", "EBMLVersion", "EBMLReadVersion", "EBMLMaxIDLength", "EBMLMaxSizeLength", "DocType", "DocTypeVersion", "DocTypeReadVersion"};
    const uint32_t defaultElementIds[] = {0xa45dfa3, 0x286, 0x2f7, 0x2f2, 0x2f3, 0x282, 0x287, 0x285};
    const int defaultElementTypes[] = {0,2,2,2,2,4,2,2};
}

#endif // EBMLTYPES_H
