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
    if(path.empty())
        return Variable();

    if(path.size() == 1)
        return get(path[0], modifiable);

    Ptr scope = getScope(path, path.size()-1);

    if(scope)
    {
        return scope->get(path.back(), modifiable);
    }
    else
    {
        return Variable();
    }
}

Variable Scope::declare(const Variant &key)
{
    return doDeclare(key);
}

const Scope::Ptr Scope::getScope(const Variant &key)
{
    return doGetScope(key);
}

Variable Scope::doGet(const Variant &/*key*/, bool /*modifable*/)
{
    return Variable();
}

Variable Scope::doDeclare(const Variant &/*key*/)
{
    return Variable();
}

const Scope::Ptr Scope::doGetScope(const Variant &/*key*/)
{
    return Ptr();
}

const Scope::Ptr Scope::getScope(const VariablePath &path, int max)
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

