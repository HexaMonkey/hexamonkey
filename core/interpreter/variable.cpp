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
#include "core/interpreter/variable.h"

const Variant undefinedVariant;
const Variant nullVariant;

Variable::Variable()
    : Variable(false, false, nullptr, &undefinedVariant)
{
}

Variant &Variable::value() const
{
    if(!_modifiable)
    {
        Variant* newValue = new Variant(*_constVar);
        _owner.reset(newValue);
        _modifiable = true;
        _var = newValue;
        _constVar = newValue;
    }
    return *_var;
}

const Variant &Variable::cvalue() const
{
    return *_constVar;
}

void Variable::setConstant()
{
    _modifiable = false;
}

bool Variable::isDefined() const
{
    return _constVar != &undefinedVariant;
}

Variable Variable::copy(const Variant &value, bool modifiable)
{
    return Variable(true, modifiable, new Variant(value));
}

Variable Variable::move(Variant *value, bool modifiable)
{
    return Variable(true, modifiable, value);
}

Variable Variable::ref(Variant &value, bool modifiable)
{
    return Variable(false, modifiable, &value);
}

Variable Variable::constRef(const Variant &value)
{
    return Variable(false, false, nullptr, &value);
}

Variable Variable::null()
{
    return Variable(true, true, new Variant);
}

Variable Variable::nullConstant()
{
    return Variable(false, false, nullptr, &nullVariant);
}

Variable::Variable(bool own, bool modifiable , Variant* var)
    : Variable(own, modifiable, var, var)
{
}

Variable::Variable(bool own, bool modifiable , Variant* var, const Variant* constVar)
    : _modifiable(modifiable),
      _var(var),
      _constVar(constVar)
{
    if(own)
    {
        _owner.reset(var);
    }
}



