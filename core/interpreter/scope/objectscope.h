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

#include "core/interpreter/scope/scope.h"
#include "core/interpreter/scope/typescope.h"

class Object;

/**
 * @brief Scope implementation that gives acces to an \link Object object\endlink members and
 * reserved \link Variable variables\endlink/\link Scope scopes\endlink.
 *
 * The members can either be accesses by their index, name or \link ObjectType type\endlink
 *
 * See the HMDL language documentation for reserved
 * \link Variable variables\endlink/\link Scope scopes\endlink
 */
class ObjectScope : public Scope
{
public:
    ObjectScope(Object& object);
protected:
    virtual Variable doGet(const Variant &key, bool modifiable) override;
    Ptr doGetScope(const Variant &key) override;
    Variable getValue(bool modifiable);
private:
    Object& _object;
    TypeScope _typeScope;

};

#endif // DATASCOPE_H
