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

#ifndef COMMONVARIABLEIMPLEMENTATIONS_H
#define COMMONVARIABLEIMPLEMENTATIONS_H

#include "core/variable/variable.h"

class VariableCollector;

/**
 * @brief Basic variable implentation owning a \link Variant value\endlink by copy
 */
class OwningVariableImplementation : public VariableImplementation
{
public:
    OwningVariableImplementation(VariableCollector& variableCollector, Variant value);
protected:
    virtual void doSetValue(const Variant& value) override;
    virtual Variant doGetValue() override;
private:
    Variant _value;
};

/**
 * @brief Basic variable implentation referencing a \link Variant value\endlink
 */
class RefVariableImplementation : public VariableImplementation
{
public:
    RefVariableImplementation(VariableCollector &variableCollector, Variant& value);
protected:
    virtual void doSetValue(const Variant& value) override;
    virtual Variant doGetValue() override;
private:
    Variant& _value;
};

/**
 * @brief Basic variable implentation const referencing a \link Variant value\endlink
 */
class ConstRefVariableImplementation : public VariableImplementation
{
public:
    ConstRefVariableImplementation(VariableCollector &variableCollector, const Variant& value);
protected:
    virtual Variant doGetValue() override;
private:
    Variant _value;
};

#endif // COMMONVARIABLEIMPLEMENTATIONS_H
