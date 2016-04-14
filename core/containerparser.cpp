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

#include "core/containerparser.h"
#include "core/module.h"
#include "core/log/logmanager.h"
#include "core/parsingexception.h"

ContainerParser::ContainerParser(Object &object, const Module &module)
    : Parser(object),
      _module(module)
{
}

Object *ContainerParser::readVariable(const ObjectType &type, std::streamoff offset)
{
    Object* child = getVariable(type, offset);
    if (child != nullptr && child->size() == -1LL) {
        child->parse();
        child->setSize(child->_contentSize);
    }
    return child;
}

Object *ContainerParser::getVariable(const ObjectType &type, std::streamoff offset)
{
    object().seekObjectEnd(offset);
    return _module.handle(type, object());
}

Object *ContainerParser::addVariable(const ObjectType &type)
{
    Object* child = getVariable(type);
    object().addChild(child);
    return child;
}

Object *ContainerParser::addVariable(const ObjectType &type, const std::string &name)
{
    object().seekObjectEnd();

    Object* child = getVariable(type);
    child->setName(name);
    object().addChild(child);
    return child;
}

const Module &ContainerParser::module() const
{
    return _module;
}
