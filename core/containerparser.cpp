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

ContainerParser::ContainerParser(Object &object, const Module &module)
    : Parser(object),
      _module(module),
      _autogrow(false)

{
}

void ContainerParser::addChild(Object *child)
{
    if(child != nullptr/* && child->size() != 0*/)
    {
        std::streamoff pos = _object.pos();
        if (child->size() == -1) {
            if(_object.size() != -1 && child->_expandOnAddition) {
                child->_size.setValue(_object.size() - pos);
            } else {
                child->parse();
                child->_size.setValue(child->_contentSize);
            }
        }


        int64_t newSize = pos + child->size();
        if (_autogrow && newSize > _object.size()) {
            _object._size = newSize;
        }

        if (newSize <= _object.size() || _object.size() == -1) {
            if (_object._contentSize < newSize) {
                _object._contentSize = newSize;
            }
            if(!child->name().empty())
            {
                _object._lookUpTable[child->name()] = child;
            }
            child->_parent = &_object;
            child->seekEnd();

            _object._children.push_back(child);
            _object._ownedChildren.push_back(std::unique_ptr<Object>(child));
            child->_rank = _object._children.size() - 1;
            _object._lastChild = nullptr;
        } else {
            _object.seekEnd();

            setParsed();
            Log::error("Too big", child->type(), " ", child->name());
        }
    }
    _object._pos.setValue(file().tellg() - _object._beginningPos);
}

void ContainerParser::addChild(Object *child, const std::string &name)
{
    if(child != nullptr)
    {
        child->setName(name);
        addChild(child);
    }
    else
    {
        Log::error("Child is null");
    }
}

Object *ContainerParser::getVariable(const ObjectType &type)
{
   // std::cout<<"Get variable"<<std::endl;
    return _module.handle(type, file(), &_object);
}

Object *ContainerParser::addVariable(const ObjectType &type)
{
    _object.seekObjectEnd();

    Object* child = getVariable(type);
    addChild(child);
    return child;
}

Object *ContainerParser::addVariable(const ObjectType &type, const std::string &name)
{
    _object.seekObjectEnd();

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
    if(object().size()<0)
        setSize(0);
}
