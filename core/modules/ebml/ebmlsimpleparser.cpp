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

#include "core/modules/default/defaultmodule.h"
#include "core/modules/ebml/ebmlsimpleparser.h"
#include "core/modules/ebml/ebmlmodule.h"

EbmlIntegerParser::EbmlIntegerParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlIntegerParser::doParseHead()
{
    Object* child = addVariable(DefaultModule::integer(availableSize()),"payload");
    if (child) {
        object().setValue(child->value());
    }
}

EbmlUIntegerParser::EbmlUIntegerParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlUIntegerParser::doParseHead()
{
    Object* child = addVariable(DefaultModule::uinteger(availableSize()),"payload");
    if (child) {
        object().setValue(child->value());
    }
}

EbmlStringParser::EbmlStringParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlStringParser::doParseHead()
{
    Object* child = addVariable(DefaultModule::string(availableSize()/8),"payload");
    if (child) {
        object().setValue(child->value());
    }
}

EbmlBinaryParser::EbmlBinaryParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlBinaryParser::doParseHead()
{
    addVariable(DefaultModule::data(availableSize()), "payload");
}

EbmlDateContainerParser::EbmlDateContainerParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlDateContainerParser::doParseHead()
{
    Object* child = addVariable(EbmlModule::Date(), "payload");
    if (child) {
        object().setValue(child->value());
    }
}

EbmlFloatParser::EbmlFloatParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlFloatParser::doParseHead()
{
    Object* child = nullptr;
    if(availableSize() == 32) {
        addVariable(DefaultModule::singleFloat(),"payload");
    } else {
        addVariable(DefaultModule::doubleFloat(),"payload");
    }
    if (child) {
        object().setValue(child->value());
    }
}
