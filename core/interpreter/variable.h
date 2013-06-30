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

#ifndef VARIABLE_H
#define VARIABLE_H

#include <memory>

#include "variant.h"

class Variable
{
public:
    Variable();

    Variant& value() const;
    const Variant& cvalue() const;

    void setConstant();

    bool isDefined() const;

    static Variable copy(const Variant& value, bool modifiable = true);
    static Variable move(Variant* value, bool modifiable = true);
    static Variable ref(Variant& value, bool modifiable = true);
    static Variable constRef(const Variant& value);
    static Variable null();
    static Variable nullConstant();
private:
    Variable(bool own, bool modifiable , Variant* var);
    Variable(bool own, bool constant , Variant* var, const Variant* constVar);

    mutable bool _modifiable;
    mutable Variant* _var;
    mutable const Variant* _constVar;
    mutable std::shared_ptr<Variant> _owner;
};

#endif // VARIABLE_H
