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

#define A_SIZE 0
#define A_PARENT 1
#define A_ARGS 2
#define A_VALUE 3
#define A_INFO 4
#define A_RANK 5

const std::map<std::string, int> reserved = {{"@size",A_SIZE}, {"@value",A_VALUE}, {"@info", A_INFO}, {"@parent",A_PARENT}, {"@args",A_ARGS}, {"@rank",A_RANK}};

MutableObjectScope::MutableObjectScope(Object &object)
    :_object(object), _typeScope(_object._type.toObjectType())
{
}

Variant *MutableObjectScope::doGet(const Variant &key) const
{
    if(key.canConvertTo(Variant::string))
    {
        const std::string& name = key.toString();
        if(name[0]=='@')
        {
            auto it = reserved.find(name);
            if(it == reserved.end())
                return nullptr;

            switch(it->second)
            {
                case A_SIZE:
                    return &_object._size;

                case A_VALUE:
                    return &_object._value;

                case A_INFO:
                    return &_object._info;

                default:
                    return nullptr;
            }
        }

        Object* elem = _object.lookUp(name, true);
        if(elem != nullptr)
        {
            return &elem->value();
        }
    }

    if(key.canConvertTo(Variant::integer))
    {
        Object* elem = _object.access(key.toInteger(), true);
        if(elem != nullptr)
        {
            return &elem->value();
        }
    }

    if(key.canConvertTo(Variant::objectType))
    {
        Object* elem = _object.lookForType(key.toObjectType(), true);
        if(elem != nullptr)
        {
            return &elem->value();
        }
    }

    return nullptr;
}

Scope *MutableObjectScope::doGetScope(const Variant &key) const
{
    if(key.canConvertTo(Variant::string))
    {
        const std::string& name = key.toString();
        if(name[0]=='@')
        {
            auto it = reserved.find(name);
            if(it == reserved.end())
                return nullptr;

            switch(it->second)
            {
                case A_PARENT:
                    return new MutableObjectScope(*_object._parent);

                case A_ARGS:
                return new MutableTypeScope(_object._type.toObjectType());

                default:
                    return nullptr;
            }
        }

        Object* elem = _object.lookUp(name, true);
        if(elem != nullptr)
        {
            return new MutableObjectScope(*elem);
        }
    }

    if(key.canConvertTo(Variant::integer))
    {
        Object* elem = _object.access(key.toInteger(), true);
        if(elem != nullptr)
        {
            return new MutableObjectScope(*elem);
        }
    }

    if(key.canConvertTo(Variant::objectType))
    {
        Object* elem = _object.lookForType(key.toObjectType(), true);
        if(elem != nullptr)
        {
            return new MutableObjectScope(*elem);
        }
    }

    return nullptr;
}


ConstObjectScope::ConstObjectScope(Object &object)
    : _object(object), _typeScope(_object._type.toObjectType())
{
}

const Variant *ConstObjectScope::doCget(const Variant &key) const
{
    if(key.canConvertTo(Variant::string))
    {
        const std::string& name = key.toString();
        if(name[0]=='@')
        {
            auto it = reserved.find(name);
            if(it == reserved.end())
                return nullptr;

            switch(it->second)
            {
                case A_SIZE:
                    return &_object._size;

                case A_VALUE:
                    return &_object._value;

                case A_INFO:
                    return &_object._info;

                case A_RANK:
                    return &_object._rank;

                default:
                    return nullptr;
            }
        }

        Object* elem = _object.lookUp(name, true);
        if(elem != nullptr)
        {
            return &elem->value();
        }
    }

    if(key.canConvertTo(Variant::integer))
    {
        Object* elem = _object.access(key.toInteger(), true);
        if(elem != nullptr)
        {
            return &elem->value();
        }
    }

    if(key.canConvertTo(Variant::objectType))
    {
        Object* elem = _object.lookForType(key.toObjectType(), true);
        if(elem != nullptr)
        {
            return &elem->value();
        }
    }

    return nullptr;
}

Scope *ConstObjectScope::doGetScope(const Variant &key) const
{
    if(key.canConvertTo(Variant::string))
    {
        const std::string& name = key.toString();
        if(name[0]=='@')
        {
            auto it = reserved.find(name);
            if(it == reserved.end())
                return nullptr;

            switch(it->second)
            {
                case A_PARENT:
                return new ConstObjectScope(*_object._parent);

                case A_ARGS:
                return new ConstTypeScope(_object._type.toObjectType());

                default:
                    return nullptr;
            }
        }

        Object* elem = _object.lookUp(name, true);
        if(elem != nullptr)
        {
            return new ConstObjectScope(*elem);
        }
    }

    if(key.canConvertTo(Variant::integer))
    {
        Object* elem = _object.access(key.toInteger(), true);
        if(elem != nullptr)
        {
            return new ConstObjectScope(*elem);
        }
    }

    if(key.canConvertTo(Variant::objectType))
    {
        Object* elem = _object.lookForType(key.toObjectType(), true);
        if(elem != nullptr)
        {
            return new ConstObjectScope(*elem);
        }
    }

    return nullptr;
}
