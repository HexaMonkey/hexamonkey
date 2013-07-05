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

#include "objectscope.h"

#include <map>
#include <string>

#include "object.h"
#include "objecttypetemplate.h"

#define A_SIZE 0
#define A_PARENT 1
#define A_ARGS 2
#define A_VALUE 3
#define A_INFO 4
#define A_RANK 5
#define A_POS 6

const std::map<std::string, int> reserved = {{"@size",A_SIZE}, {"@value",A_VALUE}, {"@info", A_INFO}, {"@parent",A_PARENT}, {"@args",A_ARGS}, {"@rank",A_RANK}, {"@pos",A_POS}};

ObjectScope::ObjectScope(Object &object, bool modifiable)
    : _object(object),
      _modifiable(modifiable),
      _typeScope(object._type.toObjectType(), modifiable)

{
}

Variable ObjectScope::doGet(const Variant &key) const
{
    if(key.canConvertTo(Variant::string))
    {
        const std::string& name = key.toString();
        if(!name.empty() && name[0]=='@')
        {
            auto it = reserved.find(name);
            if(it == reserved.end())
                return Variable();

            switch(it->second)
            {
                case A_SIZE:
                    return Variable::ref(_object._size, _modifiable);

                case A_VALUE:
                    return Variable::ref(_object._value, _modifiable);

                case A_INFO:
                    return Variable::ref(_object._info, _modifiable);

                case A_POS:
                    return Variable::copy(_object.pos());

                default:
                    return Variable();
            }
        }

        Object* elem = _object.lookUp(name, true);
        if(elem != nullptr)
        {
            return Variable::ref(elem->value(), _modifiable);
        }
    }
    else if(key.canConvertTo(Variant::integer))
    {
        Object* elem = _object.access(key.toInteger(), true);
        if(elem != nullptr)
        {
            return Variable::ref(elem->value(), _modifiable);
        }
    }
    else if(key.canConvertTo(Variant::objectType))
    {
        Object* elem = _object.lookForType(key.toObjectType(), true);
        if(elem != nullptr)
        {
            return Variable::ref(elem->value(), _modifiable);
        }
    }

    return Variable();
}

const Scope::Ptr ObjectScope::doGetScope(const Variant &key) const
{
    if(key.canConvertTo(Variant::string))
    {
        const std::string& name = key.toString();
        if(!name.empty() && name[0]=='@')
        {
            auto it = reserved.find(name);
            if(it == reserved.end())
                return Ptr();

            switch(it->second)
            {
                case A_PARENT:
                return Ptr::move(new ObjectScope(*_object._parent, _modifiable));

                case A_ARGS:
                return Ptr::move(new TypeScope(_object._type.toObjectType(), _modifiable));

                default:
                    return Ptr();
            }
        }

        Object* elem = _object.lookUp(name, true);
        if(elem != nullptr)
        {
            return Ptr::move(new ObjectScope(*elem, _modifiable));
        }
    }

    if(key.canConvertTo(Variant::integer))
    {
        Object* elem = _object.access(key.toInteger(), true);
        if(elem != nullptr)
        {
            return Ptr::move(new ObjectScope(*elem, _modifiable));
        }
    }

    if(key.canConvertTo(Variant::objectType))
    {
        Object* elem = _object.lookForType(key.toObjectType(), true);
        if(elem != nullptr)
        {
            return Ptr::move(new ObjectScope(*elem, _modifiable));
        }
    }

    return Ptr();
}
