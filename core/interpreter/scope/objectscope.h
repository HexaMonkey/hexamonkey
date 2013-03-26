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

#ifndef DATASCOPE_H
#define DATASCOPE_H

#include "scope.h"
#include "typescope.h"

class Object;

class MutableObjectScope : public MutableScope
{
public:
    MutableObjectScope(Object& data);
protected:
    Variant* doGet(const Variant &key) const override;
    Scope* doGetScope(const Variant &key) const override;
private:
    Object& _object;
    MutableTypeScope _typeScope;
};

class ConstObjectScope : public ConstScope
{
public:
    ConstObjectScope(Object& data);
protected:
    const Variant* doCget(const Variant &key) const override;
    Scope* doGetScope(const Variant &key) const override;
private:
    Object& _object;
    ConstTypeScope _typeScope;
};

#endif // DATASCOPE_H
