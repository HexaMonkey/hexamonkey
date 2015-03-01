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

#include "core/interpreter/scope/localscope.h"

LocalScope::LocalScope(const Scope::Ptr &contextScope)
    :_contextScope(contextScope)
{
}

LocalScope::LocalScope(Scope * contextScope)
    :_contextScope(Scope::Ptr(contextScope))
{
}

Variable LocalScope::doGet(const Variant &key, bool modifiable)
{
    if(key.type() == Variant::string) {
        const std::string& string = key.toString();

        auto variableIt = _variables.find(string);
        if (variableIt != _variables.end()) {
            return variableIt->second;
        }

        auto scopeIt = _subscopes.find(string);
        if (scopeIt != _subscopes.end()) {
            return scopeIt->second->getValue(modifiable);
        }
    }

    if (_contextScope) {
        return _contextScope->get(key, modifiable);
    }

    return Variable();
}

Variable LocalScope::doDeclare(const Variant &key, const Variant& initialValue)
{
    Variable variable = Variable::copy(initialValue);
    _variables[key.toString()] = variable;
    return variable;
}

Scope::Ptr LocalScope::doGetScope(const Variant &key)
{
    if(key.type() == Variant::string) {
        auto it = _subscopes.find(key.toString());
        if (it != _subscopes.end()) {
            return it->second;
        }
    }

    if (_contextScope) {
        return _contextScope->getScope(key);
    }

    return Ptr();
}

bool LocalScope::doAssignSubscope(const Variant &key, const Scope::Ptr &subscope)
{
    if(key.type() == Variant::string) {
        const std::string& string = key.toString();
        _variables.erase(string);
        _subscopes[string] = subscope;
        return true;
    }

    return false;
}

void LocalScope::doRemove(const Variant &key)
{
    if(key.type() == Variant::string) {
        const std::string& string = key.toString();
        _variables.erase(string);
        _subscopes.erase(string);
    }
}
