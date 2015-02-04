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

#include "core/interpreter/variable.h"
#include "core/interpreter/variablepath.h"
#include "core/util/ptrutil.h"

class Variant;

/**
 * @brief Hold \link Variable variables\endlink and other \link Scope scopes\endlink
 * using various \link Variant values\endlink as key
 *
 * Variables are typically access using a \link VariablePath path\endlink that accesses
 * first navigates through \link Scope scopes\endlink by their keys and then accesses
 * the variable.
 *
 * This implementation cannot have any variable or scopes, the class needs to be
 * subclass for pratical implementations.
 */
class Scope
{
public:
    typedef OptOwnPtr<Scope> Ptr;

    virtual ~Scope() {}
    /**
     * @brief Get a variable by its key
     *
     * Returns an invalid \link Variable variable\endlink if the key isn't handled
     */
    Variable get(const Variant& key, bool modifiable = true) const;

    /**
     * @brief Get a variable using a path describing the \link Scope scopes\endlink
     * indirection
     *
     * get({a, b, c}) is equivalent to getScope(a).getScope(b).get(c)
     */
    Variable get(const VariablePath& path, bool modifiable = true) const;

    /**
     * @brief Declare a new variable to be handled and return it
     *
     * Returns an invalid \link Variable variable\endlink if it cannot be declared
     */
    Variable declare(const Variant& key);

    /**
     * @brief Get a reference to a \link Scope subscope\endlink by its key
     *
     * Returns a null reference if the subscope isn't handled
     */
    const Ptr getScope(const Variant& key) const;

protected:
    /**
     * @brief Implentation for get
     */
    virtual Variable doGet(const Variant& key, bool modifiable) const;
    /**
     * @brief Implentation for declare
     */
    virtual Variable doDeclare(const Variant& key);
    /**
     * @brief Implentation for getScope
     */
    virtual const Ptr doGetScope(const Variant& key) const;

private :
    const Ptr getScope(const VariablePath& path, int max) const;
};

#endif // SCOPE_H
