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
#include "core/variable/variablecollector.h"
#include "core/log/logmanager.h"

const Variant undefinedVariant;
const Variant nullVariant = Variant::null();

class UndefinedVariableImplementation : public VariableImplementation
{
protected:
    virtual Variant doGetValue() override {
        return undefinedVariant;
    }
};

UndefinedVariableImplementation undefinedVariableImplementation;

Variable::Variable()
    :_implementation(&undefinedVariableImplementation)
{
}

Variable::Variable(VariableImplementation *implementation, bool modifiable)
    : _implementation(implementation),
      _tag(modifiable)

{
    implementation->collector().registerVariable(implementation);
}

Variable::Variable(const Variable &variable)
    : _implementation(variable._implementation),
      _tag(variable._tag.data)
{
    if (_tag.flags.defined) {
        _implementation->collector().addDirectlyAccessible(_implementation);
    }
}

Variable::Variable(Variable &&variable)
    : _implementation(variable._implementation),
      _tag(variable._tag.data)
{
    variable._tag.data = 0;
}

Variable::Variable(const VariableMemory &variable)
    : _implementation(variable._implementation),
      _tag(variable._tag.data)
{
    if (_tag.flags.defined) {
        _implementation->collector().addDirectlyAccessible(_implementation);
    }
}

Variable::~Variable()
{
    if (_tag.flags.defined) {
        _implementation->collector().removeDirectlyAccessible(_implementation);
    }
}

Variable &Variable::operator =(const Variable& rhs)
{
    if (_tag.flags.defined) {
        _implementation->collector().removeDirectlyAccessible(_implementation);
    }

    _implementation = rhs._implementation;
    _tag.data = rhs._tag.data;

    if (_tag.flags.defined) {
        _implementation->collector().addDirectlyAccessible(_implementation);
    }

    return *this;
}

Variable &Variable::operator =(Variable &&rhs)
{
    if (_tag.flags.defined) {
        _implementation->collector().removeDirectlyAccessible(_implementation);
    }

    _implementation = rhs._implementation;
    _tag.data = rhs._tag.data;
    rhs._tag.data = 0;

    return *this;
}

Variant Variable::value() const
{
    return _implementation->doGetValue();
}

void Variable::setValue(const Variant &value) const
{
    if (_tag.flags.modifiable) {
        _implementation->doSetValue(value);
    } else {
        Log::warning("Trying to set the value of a constant variable");
    }
}

Variable Variable::field(const Variant &key, bool modifiable, bool createIfNeeded) const
{
    modifiable = modifiable && _tag.flags.modifiable;

    Variable result = _implementation->doGetField(key, modifiable, modifiable && createIfNeeded);

    if (!modifiable) {
        result.setConstant();
    }

    return result;
}

Variable Variable::field(const VariablePath &path, bool modifiable, bool createIfNeeded) const
{
    auto implementation = &_implementation;
    modifiable = modifiable  && _tag.flags.modifiable;
    Variable result = (*implementation)->doGetField(path[0], modifiable, modifiable && createIfNeeded);

    for (auto it = ++path.cbegin(); it != path.cend(); ++it) {
        implementation = &result._implementation;
        modifiable = modifiable  && _tag.flags.modifiable;

        result = (*implementation)->doGetField(*it, modifiable, modifiable && createIfNeeded);
    }

    if (!modifiable) {
        result.setConstant();
    }

    return result;
}

void Variable::setField(const Variant &key, const Variable& variable) const
{
    if (_tag.flags.modifiable) {
        _implementation->doSetField(key, variable);
    } else {
        Log::warning("Trying to set a field on a constant variable");
    }
}

void Variable::setField(const VariablePath &path, const Variable &variable) const
{
    try {
        auto implementation = &_implementation;
        if (!_tag.flags.modifiable) {
            throw Error::constModification;
        }

        auto it = path.cbegin();
        for (auto end = --path.cend();it != end; ++it) {

            Variable field = (*implementation)->doGetField(*it, true, true);
            implementation = &(field._implementation);
            if (!_tag.flags.modifiable) {
                throw Error::constModification;
            }
        }

        (*implementation)->doSetField(*it, variable);

    } catch (Error) {
        Log::warning("Trying to set a field on a constant variable");
    }
}

void Variable::removeField(const Variant &key) const
{
    if (_tag.flags.modifiable) {
        _implementation->doRemoveField(key);
    } else {
        Log::warning("Trying to set a field on a constant variable");
    }
}

void Variable::removeField(const VariablePath &path) const
{
    try {
        auto implementation = &_implementation;
        if (!_tag.flags.modifiable) {
            throw Error::constModification;
        }

        auto it = path.cbegin();
        for (auto end = --path.cend();it != end; ++it) {

            Variable field = (*implementation)->doGetField(*it, true, true);
            implementation = &(field._implementation);
            if (!_tag.flags.modifiable) {
                throw Error::constModification;
            }
        }

        (*implementation)->doRemoveField(*it);

    } catch (Error) {
        Log::warning("Trying to remove a field on a constant variable");
    }
}

Variable Variable::call(VariableArgs &args, VariableKeywordArgs &kwargs) const
{
    return _implementation->doCall(args, kwargs);
}

VariableCollector &Variable::collector() const
{
    return _implementation->collector();
}



void swap(Variable &first, Variable &second)
{
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;

    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first._implementation, second._implementation);
    swap(first._tag.data, second._tag.data);
}


VariableMemory::VariableMemory()
    : _implementation(&undefinedVariableImplementation)
{
}

VariableMemory::VariableMemory(const Variable &variable)
    : _implementation(variable._implementation),
      _tag(variable._tag.data)
{
}

VariableTag defaultDefinedTag(true);

VariableMemory::VariableMemory(VariableImplementation *implementation)
    : _implementation(implementation),
      _tag(implementation == &undefinedVariableImplementation ? 0 : defaultDefinedTag.data)
{
}


VariableImplementation::VariableImplementation(VariableCollector &variableCollector)
    : _collector(&variableCollector)
{
}

VariableImplementation::~VariableImplementation()
{
}

Variant VariableImplementation::doGetValue()
{
    return nullVariant;
}

void VariableImplementation::doSetValue(const Variant &value)
{
    Log::warning("Trying to assign a value ", value," to a variable that doesn't support assignment");
}

Variable VariableImplementation::doGetField(const Variant &/*key*/, bool /*modifiable*/, bool /*createIfNeeded*/)
{
    return Variable();
}

void VariableImplementation::doSetField(const Variant &key, const Variable &/*variable*/)
{
    Log::warning("Trying to assign a field ", key," on a variable that doesn't support assignment");
}

void VariableImplementation::doRemoveField(const Variant &key)
{
    Log::warning("Trying to remove a field ", key," on a variable that doesn't support removal");
}

Variable VariableImplementation::doCall(VariableArgs &/*args*/, VariableKeywordArgs &/*kwargs*/)
{
    Log::warning("Trying call a variable that cannot be called");
    return Variable();
}

VariableImplementation::VariableImplementation()
    : _collector(nullptr)
{
}

void VariableImplementation::collect(const std::function<void(VariableMemory &)> &/*addAccessible*/)
{
}

VariableMemory VariableImplementation::memory()
{
    return VariableMemory(this);
}

