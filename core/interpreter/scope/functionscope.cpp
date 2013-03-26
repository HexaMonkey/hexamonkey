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

#include "functionscope.h"

#include "variant.h"

void FunctionScope::addModifiableParameter(const std::string &name, Variant &variant)
{
    parameterIndex[name]=modifiableParameters.size();
    modifiableParameters.push_back(&variant);
    constantParameters.push_back(&variant);
}

void FunctionScope::addConstantParameter(const std::string &name, const Variant &variant)
{
    parameterIndex[name]=modifiableParameters.size();
    modifiableParameters.push_back(nullptr);
    constantParameters.push_back(&variant);
}

Variant *FunctionScope::get(const Variant &key) const
{
    unsigned int index = getIndex(key);
    if(index < modifiableParameters.size())
        return modifiableParameters[index];

    return nullptr;
}

const Variant *FunctionScope::cget(const Variant &key) const
{
    unsigned int index = getIndex(key);
    if(index < constantParameters.size())
        return constantParameters[index];

    return nullptr;
}


unsigned int FunctionScope::getIndex(const Variant &key) const
{
    unsigned int index = 0xffffffff;

    if(key.hasNumericalType())
    {
        index = key.toUnsignedInteger();
    }
    else if(key.type() == Variant::string)
    {
        auto it = parameterIndex.find(key.toString());
        if(it != parameterIndex.end())
            index = it->second;
    }

    return index;
}



