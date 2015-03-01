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
#include "core/interpreter/scope/scopeptr.h"

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
    typedef ScopePtr Ptr;

    virtual ~Scope() {}
    /**
     * @brief Get a variable by its key
     *
     * Returns an invalid \link Variable variable\endlink if the key isn't handled
     */
    Variable get(const Variant& key, bool modifiable = false);

    /**
     * @brief Get a variable using a path describing the \link Scope scopes\endlink
     * indirection
     *
     * get({a, b, c}) is equivalent to getScope(a).getScope(b).get(c)
     */
    Variable get(const VariablePath& path, bool modifiable = false);

    /**
     * @brief Declare a new variable to be handled and return it
     *
     * Returns an invalid \link Variable variable\endlink if it cannot be declared
     */
    Variable declare(const Variant& key, const Variant &initialValue = Variant());

    /**
     * @brief Assign subscope whith the given key
     *
     * Return true if the assignment was successful
     */
    bool assignSubscope(const Variant& key, const Ptr& subscope);
    bool assignSubscope(const Variant &key, Scope* subscope);
    bool assignSubscope(const VariablePath& path, const Ptr& subscope);

    /**
     * @brief Delete variable and/or subscope
     */
    void remove(const Variant& key);
    void remove(const VariablePath& path);

    /**
     * @brief Get a reference to a \link Scope subscope\endlink by its key
     *
     * Returns a null reference if the subscope isn't handled
     */
    Ptr getScope(const Variant& key);

    Ptr getScope(const VariablePath& path);

    /**
     * @brief Get designated value for the scope
     */
    virtual Variable getValue(bool modifiable);

protected:
    /**
     * @brief Implementation for get
     */
    virtual Variable doGet(const Variant& key, bool modifiable);
    /**
     * @brief Implementation for declare
     */
    virtual Variable doDeclare(const Variant& key, const Variant& initialValue);
    /**
     * @brief Implementation for getScope
     */
    virtual Ptr doGetScope(const Variant& key);

    /**
     * @brief Implementation for assignSubscope
     */
    virtual bool doAssignSubscope(const Variant& key, const Ptr& subscope);

    /**
     * @brief Implementation for remove
     */
    virtual void doRemove(const Variant &key);

private :
    const Scope::Ptr _getScope(const VariablePath& path, int max);
};

#endif // SCOPE_H
