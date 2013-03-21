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

#include "variable.h"
#include "variant.h"

Variable::~Variable()
{
    if(_own)
    {
        delete _var;
    }
}

Variant &Variable::value()
{
    if(_constant)
    {
        Variant* newValue = new Variant(*_constVar);
        _own = true;
        _constant = false;
        _var = newValue;
        _constVar = newValue;
    }
    return *_var;
}

const Variant &Variable::constValue() const
{
    return *_constVar;
}

Variable *Variable::copy(const Variant &value)
{
    Variant* newValue = new Variant(value);
    return new Variable(true, false, newValue, newValue);
}

Variable *Variable::move(Variant *value)
{
    return new Variable(true, false, value, value);
}

Variable *Variable::constReference(const Variant &value)
{
    return new Variable(false, true, NULL, &value);
}

Variable *Variable::reference(Variant &value)
{
    return new Variable(false, false, &value, &value);
}

Variable::Variable(bool own, bool constant , Variant* var, const Variant* constVar)
    : _own(own),
      _constant(constant),
      _var(var),
      _constVar(constVar)
{
}
