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

const Variant nullVariant;

Variable::Variable()
    :_implementation(new VariableImplementation),
     _tag(Variable::Tag::undefined)
{
}

Variable::Variable(VariableImplementation *implementation, bool modifiable)
    : _implementation(implementation),
      _tag(modifiable ? Variable::Tag::modifiable : Variable::Tag::constant)

{
}

void Variable::setValue(const Variant &value)
{
    if (_tag != Tag::modifiable) {
        inPlaceCopy(true);
    }

    _implementation->doSetValue(value);
}

Variant Variable::value() const
{
    return _implementation->doGetValue();
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

VariableImplementation::~VariableImplementation()
{
}

void VariableImplementation::doSetValue(const Variant &/*value*/)
{
}

Variant VariableImplementation::doGetValue()
{
    return nullVariant;
}

class OwningVariableImplementation : public VariableImplementation
{
public:
    OwningVariableImplementation(Variant value);
protected:
    virtual void doSetValue(const Variant& value) override;
    virtual Variant doGetValue() override;
private:
    Variant _value;
};

OwningVariableImplementation::OwningVariableImplementation(Variant value)
    : _value(value)
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

class RefVariableImplementation : public VariableImplementation
{
public:
    RefVariableImplementation(Variant& value);
protected:
    virtual void doSetValue(const Variant& value) override;
    virtual Variant doGetValue() override;
private:
    Variant& _value;
};


RefVariableImplementation::RefVariableImplementation(Variant& value)
    : _value(value)
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

class ConstRefVariableImplementation : public VariableImplementation
{
public:
    ConstRefVariableImplementation(const Variant& value);
protected:
    virtual Variant doGetValue() override;
private:
    Variant _value;
};


ConstRefVariableImplementation::ConstRefVariableImplementation(const Variant& value)
    : _value(value)
{
}

Variant ConstRefVariableImplementation::doGetValue()
{
    return _value;
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

Variable Variable::constRef(const Variant &value)
{
    return Variable(new ConstRefVariableImplementation(value), false);
}

Variable Variable::null()
{
    return Variable(new OwningVariableImplementation(Variant()), true);
}

Variable Variable::nullConstant()
{
    return Variable(new ConstRefVariableImplementation(nullVariant), false);
}
