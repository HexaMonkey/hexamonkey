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
      _module(module),
      _autogrow(false)

{
}

void ContainerParser::addChild(Object *child)
{
    if (child != nullptr) {
        std::streamoff pos = object().pos();
        if (child->size() == -1LL) {
            if (object().size() != -1LL && child->isSetToExpandOnAddition()) {
                child->setSize(object().size() - pos);
            } else {
                child->parse();
                child->setSize(child->_contentSize);
            }
        }

        if (!(child->isValid())) {
            throwChildError(*child, ParsingException::InvalidChild, "child invalid");
        }

        int64_t newSize = pos + child->size();
        if (_autogrow && newSize > object().size()) {
            object().setSize(newSize);
        }

        std::streamoff newPos;
        const bool fileGood = object().file().good();
        const int64_t objectSize = object().size();
        const int64_t newAbsolutePosition = object().beginningPos() + newSize;
        const int64_t fileSize = object().file().size();
        if (!fileGood || newAbsolutePosition > fileSize) {
            throwChildError(*child, ParsingException::OutOfFile, "out of file");
        } else if (objectSize != -1 && newSize > objectSize) {
            throwChildError(*child, ParsingException::OutOfParent, "too big");
        } else {
            if (object()._contentSize < newSize) {
                object()._contentSize = newSize;
            }

            if(!child->name().empty()) {
                object()._lookUpTable[child->name()] = child;
            }

            child->_parent = &object();
            newPos = child->beginningPos() - object().beginningPos();
            if (child->size() != -1LL) {
                newPos += child->size();
            }

            object()._children.push_back(child);
            object()._ownedChildren.push_back(std::unique_ptr<Object>(child));
            child->_rank = _object._children.size() - 1;
            object()._lastChild = nullptr;
        }

        object().setPos(newPos);
        object().seekObjectEnd();
    }
}

void ContainerParser::addChild(Object *child, const std::string &name)
{
    if(child != nullptr) {
        child->setName(name);
        addChild(child);
    } else {
        Log::error("Child is null");
    }
}

Object *ContainerParser::getVariable(const ObjectType &type)
{
    return _module.handle(type, object().file(), &object());
}

Object *ContainerParser::addVariable(const ObjectType &type)
{
    object().seekObjectEnd();

    Object* child = getVariable(type);
    addChild(child);
    return child;
}

Object *ContainerParser::addVariable(const ObjectType &type, const std::string &name)
{
    object().seekObjectEnd();

    Object* child = getVariable(type);
    addChild(child, name);
    return child;
}

const Module &ContainerParser::module() const
{
    return _module;
}


void ContainerParser::setAutogrow()
{
    _autogrow = true;
    if(object().size()<0) {
        object().setSize(0);
    }
}

void ContainerParser::throwChildError(const Object &child, ParsingException::Type type, const std::string reason) const
{
    throw ParsingException(type, concat("Child ", child, " cannot be added to ", object(), " : ", reason));
}
