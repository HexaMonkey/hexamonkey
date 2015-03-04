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
#include "core/parser.h"
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

Variable AbstractTypeScope::doGet(const Variant &key, bool modifiable)
{
    int i = -1;

    ObjectType* mType = modifiable ? modifiableType() : nullptr;
    const ObjectType& cType = constType();

    if(key.canConvertTo(Variant::integerType)) {
        i = key.toInteger();
    } else if (key.canConvertTo(Variant::stringType)) {
        const std::string str = key.toString();

        if(!str.empty() && str[0]=='@')
        {
            auto it = reserved.find(str);
            if(it == reserved.end())
                return Variable();

            switch(it->second)
            {
                case A_COUNT:
                    return Variable::copy(constType().numberOfParameters(), false);

                case A_ELEMENT_TYPE:
                    if (mType) {
                        return Variable::ref(mType->_elementType);
                    } else {
                        return Variable::constRef(cType._elementType);
                    }

                case A_ELEMENT_COUNT:
                    if (mType) {
                        return Variable::ref(mType->_elementCount);
                    } else {
                        return Variable::constRef(cType._elementCount);
                    }

                case A_NAME:
                    if (mType) {
                        return Variable::ref(mType->_name);
                    } else {
                        return Variable::constRef(cType._name);
                    }
            }
        }

        i = cType.typeTemplate().parameterNumber(key.toString());
    }

    if(i != -1 && (size_t) i < cType._parametersValue.size())
    {
        if(mType)
        {
            return Variable::ref(mType->_parametersValue[i]);
        } else {
            return Variable::constRef(cType._parametersValue[i]);
        }
    }

    return Variable();
}

Variable AbstractTypeScope::getValue(bool /*modifiable*/)
{
    return Variable::copy(constType());
}

TypeScope::TypeScope(ObjectType &type, bool modifiable)
    : _type(modifiable? &type : nullptr),
      _constType(type)
{
}

TypeScope::TypeScope(const ObjectType &type)
    : _type(nullptr),
      _constType(type)
{
}

ObjectType *TypeScope::modifiableType()
{
    return _type;
}

const ObjectType &TypeScope::constType()
{
    return _constType;
}


ParserTypeScope::ParserTypeScope(Parser &parser)
    :_parser(parser)
{
}

ObjectType *ParserTypeScope::modifiableType()
{
    return _parser.modifiableType();
}

const ObjectType &ParserTypeScope::constType()
{
    return _parser.constType();
}
