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

#include "program.h"

Program::Program()
    : _object(nullptr)
{
}

Program::Program(Object &object)
    : _object(&object)
{
}

uint32_t Program::id() const
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
   // _object->explore();
    return _object->numberOfChildren() - 2;
}

Program Program::elem(int index) const
{

    return Program(*_object->access(index+2, true));
}


Program::const_iterator Program::begin() const
{
    //_object->explore();
    Object::iterator it = _object->begin();
    std::advance(it, 2);
    return const_iterator(it);
}

Program::const_iterator Program::end() const
{
    return const_iterator(_object->end());
}

Program::const_reverse_iterator Program::rbegin() const
{
    //_object->explore();
    return const_reverse_iterator(_object->rbegin());
}

Program::const_reverse_iterator Program::rend() const
{
    Object::reverse_iterator it = _object->rend();
    std::advance(it, -2);
    return const_reverse_iterator(it);
}
