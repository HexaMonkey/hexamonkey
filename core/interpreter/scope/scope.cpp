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

#include "core/interpreter/scope/scope.h"

Variable Scope::get(const Variant &key, bool modifiable)
{
    return doGet(key, modifiable);
}

Variable Scope::get(const VariablePath &path, bool modifiable)
{
    if(path.empty()) {
        return Variable();
    }

    if(path.size() == 1) {
        return get(path[0], modifiable);
    }

    Ptr scope = _getScope(path, path.size()-1);

    if(scope) {
        return scope->get(path.back(), modifiable);
    } else {
        return Variable();
    }
}

Variable Scope::declare(const Variant &key, const Variant& initialValue)
{
    return doDeclare(key, initialValue);
}

bool Scope::assignSubscope(const Variant &key, const Ptr &subscope)
{
    if (subscope) {
        return doAssignSubscope(key, subscope);
    } else {
        return false;
    }
}

bool Scope::assignSubscope(const Variant &key, Scope *subscope)
{
    if (subscope) {
        return doAssignSubscope(key, Ptr(subscope));
    } else {
        return false;
    }
}

bool Scope::assignSubscope(const VariablePath &path, const Scope::Ptr &subscope)
{
    if(path.size() == 1) {
        return assignSubscope(path[0], subscope);
    }

    Ptr scope = _getScope(path, path.size() - 1);

    if (scope) {
        return scope->assignSubscope(path.back(), subscope);
    } else {
        return false;
    }
}

void Scope::remove(const Variant &key)
{
    doRemove(key);
}

void Scope::remove(const VariablePath &path)
{
    if(path.size() == 1) {
        remove(path[0]);
    } else {

        Ptr scope = _getScope(path, path.size() - 1);

        if (scope) {
            return scope->remove(path.back());
        }
    }
}

Scope::Ptr Scope::getScope(const Variant &key)
{
    return doGetScope(key);
}

Scope::Ptr Scope::getScope(const VariablePath &path)
{
    return _getScope(path, path.size());
}

Variable Scope::doGet(const Variant &/*key*/, bool /*modifable*/)
{
    return Variable();
}

Variable Scope::doDeclare(const Variant &/*key*/, const Variant& /*initialValue*/)
{
    return Variable();
}

Scope::Ptr Scope::doGetScope(const Variant &/*key*/)
{
    return Ptr();
}

bool Scope::doAssignSubscope(const Variant &/*key*/, const Scope::Ptr &/*subscope*/)
{
    return false;
}

void Scope::doRemove(const Variant &/*key*/)
{
}

Variable Scope::getValue(bool /*modifiable*/)
{
    return Variable();
}

const Scope::Ptr Scope::_getScope(const VariablePath &path, int max)
{
    if(max < 1) {
        return Ptr();
    }

    Ptr current = getScope(path[0]);
    for(int i = 1; i < max; ++i) {
        if(!current) {
            return Ptr();
        }
        current = current->getScope(path[i]);
    }
    return current;
}
