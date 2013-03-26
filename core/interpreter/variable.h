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

class Variant;

class Variable
{
public:
    ~Variable();

    Variant& value();
    const Variant& cvalue() const;

    bool isConst() const;
    bool isOwner() const;

    static Variable* copy(const Variant& value);
    static Variable* move(Variant* value);
    static Variable* reference(Variant& value);
    static Variable* constReference(const Variant& value);
    static Variable* null();
private:
    Variable(bool own, bool constant , Variant* var, const Variant* constVar);
    Variable() = delete;
    Variable(const Variable&) = delete;
    Variable& operator =(const Variable&) = delete;

    bool _own;
    bool _constant;
    Variant* _var;
    const Variant* _constVar;

};

#endif // VARIABLE_H
