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

#include "scope.h"

Scope::Scope()
{
    pthis = Ptr::ref(*this);
}

Variable Scope::get(const Variant &key) const
{
    return doGet(key);
}

Variable Scope::get(const VariablePath &path) const
{
    if(path.empty())
        return Variable();

    Ptr scope = getScope(path, path.size()-1);

    if(scope)
    {
        return scope->get(path.back());
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

const Scope::Ptr Scope::getScope(const Variant &key) const
{
    return doGetScope(key);
}

Variable Scope::doGet(const Variant &/*key*/) const
{
    return Variable();
}

Variable Scope::doDeclare(const Variant &/*key*/)
{
    return Variable();
}

const Scope::Ptr Scope::doGetScope(const Variant &/*key*/) const
{
    return Ptr();
}

const Scope::Ptr Scope::getScope(const VariablePath &path, int max) const
{
    Ptr current = pthis;
    for(int i = 0; i < max; ++i)
    {
        if(!current)
        {
            return Ptr();
        }
        current = current->getScope(path[i]);
    }
    return current;
}

