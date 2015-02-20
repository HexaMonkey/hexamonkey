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

#include "core/interpreter/scope/compositescope.h"

void CompositeScope::addScope(Scope *scope)
{
    _scopes.emplace_back(scope);
}

void CompositeScope::addScope(const Scope::Ptr &scope)
{
    _scopes.push_back(scope);
}

Variable CompositeScope::doGet(const Variant &key, bool modifiable)
{
    for(auto& scope : _scopes)
    {
        Variable variable = scope->get(key, modifiable);
        if(variable.isDefined())
            return variable;
    }
    return Variable();
}

const Scope::Ptr CompositeScope::doGetScope(const Variant &key)
{
    for(auto& scope : _scopes)
    {
        Scope::Ptr s = scope->getScope(key);
        if(s)
            return s;
    }
    return Ptr();
}

Variable CompositeScope::doDeclare(const Variant &key)
{
    for(auto& scope : _scopes)
    {
        Variable variable = scope->declare(key);
        if(variable.isDefined())
            return variable;
    }
    return Variable();
}
