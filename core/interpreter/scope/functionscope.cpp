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

#include "core/variant.h"
#include "core/interpreter/scope/functionscope.h"

void FunctionScope::addParameter(const std::string &name, Variable variable)
{
    parameterIndex[name] = parameters.size();
    parameters.push_back(variable);
}

unsigned int FunctionScope::getIndex(const Variant &key) const
{
    unsigned int index = 0xffffffff;

    if (key.hasNumericalType()) {
        index = key.toUnsignedInteger();
    } else if(key.type() == Variant::stringType) {
        auto it = parameterIndex.find(key.toString());
        if(it != parameterIndex.end())
            index = it->second;
    }

    return index;
}

Variable FunctionScope::doGet(const Variant &key, bool /*modifiable*/)
{
    unsigned int index = getIndex(key);
    if (index < parameters.size()) {
        return parameters[index];
    }

    return Variable();
}


