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


#include <map>
#include <string>

#include "core/object.h"
#include "core/objecttypetemplate.h"
#include "core/interpreter/scope/objectscope.h"
#include "core/interpreter/scope/attributescope.h"
#include "core/interpreter/scope/contextscope.h"


#define A_SIZE 0
#define A_PARENT 1
#define A_ROOT 2
#define A_VALUE 3
#define A_RANK 4
#define A_POS 5
#define A_REM 6
#define A_NUMBER_OF_CHILDREN 7
#define A_BEGINNING_POS 8
#define A_LINK_TO 9
#define A_ATTR 10
#define A_CONTEXT 11
#define A_GLOBAL 12

const std::map<std::string, int> reserved = {
    {"@size",             A_SIZE},
    {"@value",            A_VALUE},
    {"@parent",           A_PARENT},
    {"@root",             A_ROOT},
    {"@rank",             A_RANK},
    {"@pos",              A_POS},
    {"@rem",              A_REM},
    {"@numberOfChildren", A_NUMBER_OF_CHILDREN},
    {"@beginningPos",     A_BEGINNING_POS},
    {"@linkTo",           A_LINK_TO},
    {"@attr",             A_ATTR},
    {"@context",          A_CONTEXT},
    {"@global",           A_GLOBAL}
};

ObjectScope::ObjectScope(Object &object)
    : _object(object),
      _typeScope(object._type.toObjectType())
{
}

Variable ObjectScope::doGet(const Variant &key, bool modifiable)
{
    if(key.isNull())
    {
        int numberOfChildren = _object.numberOfChildren();
        if(numberOfChildren > 0) {
            Object* elem = _object.access(numberOfChildren - 1, true);
            if(elem != nullptr)
            {
                return Variable::ref(elem->value(), modifiable);
            }
        }
    }
    else if(key.canConvertTo(Variant::string))
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
                    return Variable::ref(_object._size, modifiable);

                case A_VALUE:
                    return Variable::ref(_object._value, modifiable);

                case A_POS:
                    return Variable::ref(_object._pos, modifiable);

                case A_RANK:
                    return Variable::ref(_object._rank, false);

                case A_NUMBER_OF_CHILDREN:
                    _object.explore(1);
                    return Variable::copy(_object.numberOfChildren());

                case A_REM:
                    return Variable::copy(_object._size.toInteger() - _object._pos.toInteger());

                case A_BEGINNING_POS:
                    return Variable::copy((long long) _object.beginningPos());

                case A_LINK_TO:
                    return Variable::ref(_object._linkTo, modifiable);

                default:
                    return Variable();
            }
        }

        Object* elem = _object.lookUp(name, true);
        if(elem != nullptr)
        {
            return Variable::ref(elem->value(), modifiable);
        }
    }
    else if(key.canConvertTo(Variant::integer))
    {
        Object* elem = _object.access(key.toInteger(), true);
        if(elem != nullptr)
        {
            return Variable::ref(elem->value(), modifiable);
        }
    }
    else if(key.canConvertTo(Variant::objectType))
    {
        Object* elem = _object.lookForType(key.toObjectType(), true);
        if(elem != nullptr)
        {
            return Variable::ref(elem->value(), modifiable);
        }
    }

    return Variable();
}

Scope::Ptr ObjectScope::doGetScope(const Variant &key)
{
    if(key.isNull())
    {
        int numberOfChildren = _object.numberOfChildren();
        if(numberOfChildren > 0) {
            Object* elem = _object.access(numberOfChildren - 1, true);
            if(elem != nullptr)
            {
                return Ptr(new ObjectScope(*elem));
            }
        }
    }
    else if(key.canConvertTo(Variant::string))
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
                    return Ptr(new ObjectScope(*_object._parent));

                case A_ROOT:
                    return Ptr(new ObjectScope(_object.root()));

                case A_ATTR:
                    return Ptr(new AttributeScope(_object));

                case A_CONTEXT:
                    return Ptr(new ContextScope(_object));

                case A_GLOBAL:
                    return Ptr(new ContextScope(_object.root()));

                default:
                    return Ptr();
            }
        }

        Object* elem = _object.lookUp(name, true);
        if(elem != nullptr)
        {
            return Ptr(new ObjectScope(*elem));
        }
    }
    else if(key.canConvertTo(Variant::integer))
    {
        int index = key.toInteger();
        if(index == _object.numberOfChildren() && _object._lastChild != nullptr)
        {
            return Ptr(new ObjectScope(*_object._lastChild));
        }
        Object* elem = _object.access(index, true);
        if(elem != nullptr)
        {
            return Ptr(new ObjectScope(*elem));
        }
    }
    else if(key.canConvertTo(Variant::objectType))
    {
        Object* elem = _object.lookForType(key.toObjectType(), true);
        if(elem != nullptr)
        {
            return Ptr(new ObjectScope(*elem));
        }
    }

    return Ptr();
}

Variable ObjectScope::getValue(bool modifiable)
{
    return Variable::ref(_object._value, modifiable);
}
