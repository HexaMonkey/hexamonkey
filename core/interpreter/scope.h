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

#ifndef SCOPE_H
#define SCOPE_H

class Variant;

class Scope
{
public:
    virtual ~Scope() {}
    virtual Variant* get(const Variant& key) const = 0;
    virtual const Variant* cget(const Variant& key) const = 0;
    Variant *declare(const Variant& key) const;
    Scope *getScope(const Variant& key) const;
protected:
    virtual Variant *doDeclare(const Variant& key) const;
    virtual Scope *doGetScope(const Variant&) const;
};

class MutableScope : public Scope
{
public:
    virtual ~MutableScope() {}
    Variant* get(const Variant& key) const override;
    const Variant* cget(const Variant& key) const override;
protected:
    virtual Variant* doGet(const Variant& key) const = 0;
};

class ConstScope : public Scope
{
public:
    virtual ~ConstScope() {}
    virtual Variant* get(const Variant& key) const override;
    virtual const Variant* cget(const Variant& key) const override;
protected:
    virtual const Variant* doCget(const Variant& key) const = 0;
};

class EmptyScope : public MutableScope
{
protected:
    Variant* doGet(const Variant &key) const override;
};

#endif // SCOPE_H
