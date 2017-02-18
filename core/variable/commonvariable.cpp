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

#include "commonvariable.h"
#include "core/variable/variable.h"

OwningVariableImplementation::OwningVariableImplementation(VariableCollector &variableCollector, Variant value)
    : VariableImplementation(variableCollector), _value(value)
{
}

void OwningVariableImplementation::doSetValue(const Variant &value)
{
    _value = value;
}

Variant OwningVariableImplementation::doGetValue()
{
    return _value;
}

bool OwningVariableImplementation::isByRefOnly()
{
    return false;
}


RefVariableImplementation::RefVariableImplementation(VariableCollector &variableCollector, Variant& value)
    : VariableImplementation(variableCollector), _value(value)
{
}

void RefVariableImplementation::doSetValue(const Variant &value)
{
    _value = value;
}

Variant RefVariableImplementation::doGetValue()
{
    return _value;
}

bool RefVariableImplementation::isByRefOnly()
{
    return false;
}

ConstRefVariableImplementation::ConstRefVariableImplementation(VariableCollector &variableCollector, const Variant& value)
    : VariableImplementation(variableCollector), _value(value)
{
}

Variant ConstRefVariableImplementation::doGetValue()
{
    return _value;
}

bool ConstRefVariableImplementation::isByRefOnly()
{
    return false;
}
