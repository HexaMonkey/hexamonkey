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

#include "core/objecttype.h"
#include "core/objecttypetemplate.h"
#include "core/interpreter/scope/typescope.h"

#define A_COUNT 0
#define A_ELEMENT_TYPE 1
#define A_ELEMENT_COUNT 2
#define A_NAME 3

const std::map<std::string, int> reserved = {
    {"@count", A_COUNT},
    {"@elementType", A_ELEMENT_TYPE},
    {"@elementCount", A_ELEMENT_COUNT},
    {"@name", A_NAME}
};

TypeScope::TypeScope(ObjectType &type, bool modifiable)
    : _type(modifiable? &type : nullptr),
      _constType(&type)
{
}

TypeScope::TypeScope(const ObjectType &type)
    : _type(nullptr),
      _constType(&type)
{
}

Variable TypeScope::doGet(const Variant &key, bool modifiable)
{
    int i = -1;

    ObjectType* pType = modifiable ? _type : nullptr;
    const ObjectType& constType = *_constType;

    if(key.canConvertTo(Variant::integer)) {
        i = key.toInteger();
    } else if (key.canConvertTo(Variant::string)) {
        const std::string str = key.toString();

        if(!str.empty() && str[0]=='@')
        {
            auto it = reserved.find(str);
            if(it == reserved.end())
                return Variable();

            switch(it->second)
            {
                case A_COUNT:
                    return Variable::copy(_constType->numberOfParameters(), false);

                case A_ELEMENT_TYPE:
                    if (pType) {
                        return Variable::ref(pType->_elementType);
                    } else {
                        return Variable::constRef(constType._elementType);
                    }

                case A_ELEMENT_COUNT:
                    if (pType) {
                        return Variable::ref(pType->_elementCount);
                    } else {
                        return Variable::constRef(constType._elementCount);
                    }

                case A_NAME:
                    if (pType) {
                        return Variable::ref(pType->_name);
                    } else {
                        return Variable::constRef(constType._name);
                    }
            }
        }

        i = constType.typeTemplate().parameterNumber(key.toString());
    }

    if(i != -1 && (size_t) i < constType._parametersValue.size())
    {
        if(pType)
        {
            return Variable::ref(pType->_parametersValue[i]);
        } else {
            return Variable::constRef(constType._parametersValue[i]);
        }
    }

    return Variable();
}
