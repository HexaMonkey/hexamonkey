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

class ConstRefVariableImplementation : public VariableImplementation
{
public:
    ConstRefVariableImplementation(const Variant& value);
protected:
    virtual Variant doGetValue() override;
private:
    Variant _value;
};

#endif // COMMONVARIABLEIMPLEMENTATIONS_H
