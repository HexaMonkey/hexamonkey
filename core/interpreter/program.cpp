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

#include "compiler/model.h"
#include "core/variant.h"
#include "core/parser.h"
#include "core/interpreter/program.h"
#include "core/interpreter/variable.h"
#include "core/interpreter/scope/scope.h"
#include "core/interpreter/scope/functionscope.h"
#include "core/util/unused.h"

Program::Program()
    : _object(nullptr)
{
    UNUSED(hmcElemNames);
}

Program::Program(Object &object, std::shared_ptr<Memory> memory)
    : _object(&object),
      _memory(memory)
{
    _object->explore();
}

bool Program::isValid() const
{
    return _object != nullptr;
}

uint32_t Program::tag() const
{
    return _object->lookUp("id")->value().toInteger();
}

const Variant &Program::payload() const
{
    Object* object = _object->lookUp("payload", true);
    if(object == nullptr)
        object = _object;
    return object->value();
}

int Program::size() const
{   
    return _object->numberOfChildren() - 2;
}

Program Program::node(int index) const
{
    Object* nodeObject = _object->access(index+2, true);
    if(nodeObject != nullptr)
        return Program(*nodeObject, _memory);
    else
        return Program();
}

Program Program::operator [](int index) const
{
    return node(index);
}


Program::const_iterator Program::begin() const
{   
    Object::iterator it = _object->begin();
    std::advance(it, 2);
    return const_iterator(it, _memory);
}

Program::const_iterator Program::end() const
{   
    return const_iterator(_object->end(), _memory);
}

Program::const_reverse_iterator Program::rbegin() const
{   
    return const_reverse_iterator(_object->rbegin(), _memory);
}

Program::const_reverse_iterator Program::rend() const
{   
    Object::reverse_iterator it = _object->rend();
    std::advance(it, -2);
    return const_reverse_iterator(it, _memory);
}

Program::Memory &Program::memory() const
{
    return *_memory.get();
}

File &Program::Memory::file()
{
    return _file;
}

Object &Program::Memory::setFileObject(Object *fileObject)
{
    _fileObject.reset(fileObject);
    return *fileObject;
}
