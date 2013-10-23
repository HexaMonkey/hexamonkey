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

#include "core/modules/default/defaulttypes.h"
#include "core/modules/ebml/ebmlsimpleparser.h"
#include "core/modules/ebml/ebmltypes.h"
#include "core/modules/standard/standardtypes.h"

EbmlIntegerParser::EbmlIntegerParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlIntegerParser::doParseHead()
{
    addVariable(standardTypes::integer(availableSize()),"payload");
    showcase().add("payload");
}

EbmlUIntegerParser::EbmlUIntegerParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlUIntegerParser::doParseHead()
{
    addVariable(standardTypes::uinteger(availableSize()),"payload");
    showcase().add("payload");
}

EbmlStringParser::EbmlStringParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlStringParser::doParseHead()
{
    addVariable(standardTypes::string(availableSize()/8),"payload");
    showcase().add("payload");
}

EbmlBinaryParser::EbmlBinaryParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlBinaryParser::doParseHead()
{
    addVariable(defaultTypes::data(availableSize()), "payload");
}

EbmlDateContainerParser::EbmlDateContainerParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlDateContainerParser::doParseHead()
{
    addVariable(ebmlTypes::Date(), "payload");
    showcase().add("payload");
}

EbmlFloatParser::EbmlFloatParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlFloatParser::doParseHead()
{
    if(availableSize() == 32)
        addVariable(standardTypes::singleFloat(),"payload");
    else
        addVariable(standardTypes::doubleFloat(),"payload");
    showcase().add("payload");
}
