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
#include "core/variable/variable.h"
#include "core/variable/commonvariable.h"
#include "core/log/logmanager.h"

const Variant undefinedVariant;
const Variant nullVariant = Variant::null();


class UndefinedVariableImplementation : public VariableImplementation
{
protected:
    virtual Variant doGetValue() override;
};

Variable::Variable()
    :_implementation(new UndefinedVariableImplementation),
     _tag(Variable::Tag::undefined)
{
}

Variable::Variable(VariableImplementation *implementation, bool modifiable)
    : _implementation(implementation),
      _tag(modifiable ? Variable::Tag::modifiable : Variable::Tag::constant)

{
}

Variant Variable::value() const
{
    return _implementation->doGetValue();
}

void Variable::setValue(const Variant &value)
{
    if (_tag == Tag::modifiable) {
        _implementation->doSetValue(value);
    } else {
        Log::warning("Trying to set the value of a constant variable");
    }
}

Variable Variable::field(const Variant &key, bool modifiable)
{
    modifiable = modifiable && _tag == Tag::modifiable;
    if (modifiable) {
        return _implementation->doGetField(key, true);
    } else {
        Variable result = _implementation->doGetField(key, false);
        result.setConstant();
        return result;
    }
}

void Variable::setField(const Variant &key, const Variable &variable)
{
    if (_tag == Tag::modifiable) {
        _implementation->doSetField(key, variable);
    } else {
        Log::warning("Trying to set a field on a constant variable");
    }
}

void Variable::removeField(const Variant &key)
{
    if (_tag == Tag::modifiable) {
        _implementation->doRemoveField(key);
    } else {
        Log::warning("Trying to remove a field on a constant variable");
    }
}

void Variable::setConstant()
{
    if (_tag == Tag::modifiable) {
        _tag = Tag::constant;
    }
}

bool Variable::isDefined() const
{
    return _tag != Tag::undefined;
}

void Variable::inPlaceCopy(bool modifiable)
{
    _tag = modifiable ? Tag::modifiable : Tag::constant;
    _implementation.reset(new OwningVariableImplementation(value()));
}

Variable Variable::copy(const Variant &value, bool modifiable)
{
    return Variable(new OwningVariableImplementation(value), modifiable);
}

Variable Variable::ref(Variant &value, bool modifiable)
{
    return Variable(new RefVariableImplementation(value), modifiable);
}

Variable Variable::refIfNotNull(Variant *value, bool modifiable)
{
    if (value != nullptr) {
        return Variable(new RefVariableImplementation(*value), modifiable);
    } else {
        return Variable();
    }
}

Variable Variable::constRef(const Variant &value)
{
    return Variable(new ConstRefVariableImplementation(value), false);
}

Variable Variable::constRefIfNotNull(const Variant *value)
{
    if (value != nullptr) {
        return Variable(new ConstRefVariableImplementation(*value), false);
    } else {
        return Variable();
    }
}

Variable Variable::null()
{
    return Variable(new OwningVariableImplementation(nullVariant), true);
}


Variable Variable::nullConstant()
{
    return Variable(new ConstRefVariableImplementation(nullVariant), false);
}


VariableImplementation::~VariableImplementation()
{
}

Variant VariableImplementation::doGetValue()
{
    return nullVariant;
}

void VariableImplementation::doSetValue(const Variant &/*value*/)
{
    Log::warning("Trying to assign a value to a variable that doesn't support assignment");
}

Variable VariableImplementation::doGetField(const Variant &/*key*/, bool /*modifiable*/)
{
    return Variable();
}

void VariableImplementation::doSetField(const Variant &/*key*/, const Variable &/*variable*/)
{
    Log::warning("Trying to assign a field on a variable that doesn't support assignment");
}

void VariableImplementation::doRemoveField(const Variant &key)
{
    Log::warning("Trying to remove a field on a variable that doesn't support removal");
}


Variant UndefinedVariableImplementation::doGetValue()
{
    return undefinedVariant;
}
