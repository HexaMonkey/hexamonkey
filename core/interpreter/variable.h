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
#include <vector>

#include "variant.h"

/**
 * @brief Holds the memory for valued object used during program
 * interpretation.
 *
 * The variable can either own the value or be a reference to it.
 * In the case of owning, reference counting is used to keep track
 * of shared memory, in case of copying.
 *
 * The variable can also be constant or modifiable. When the variable
 * is constant and a modifiable reference is requested then a copy
 * reference to a copy will be given.
 */
class Variable
{
public:
    /**
     * @brief Construct an undefined \link Variable variable\endlink.
     */
    Variable();

    /**
     * @brief Get a modifiable reference to the value
     *
     * If the \link Variable variable\endlink is constant, then a copy
     * is made. The variable becomes modifiable.
     *
     * The reference is valid as long as the variable lives and no operation
     * are done on it.
     */
    Variant& value() const;

    /**
     * @brief Get a constant reference to the value
     *
     * The reference is valid as long as the \link Variable variable\endlink lives and no operation
     * are done on it.
     */
    const Variant& cvalue() const;

    /**
     * @brief Set the variable as constant which prevents modification of the value
     */
    void setConstant();

    /**
     * @brief Check if the variable not been constructed by the default constructor.
     */
    bool isDefined() const;

    /**
     * @brief Construct a \link Variable variable\endlink copying and then owning a value
     */
    static Variable copy(const Variant& value, bool modifiable = true);

    /**
     * @brief Construct a \link Variable variable\endlink managing the value given as a pointer
     */
    static Variable move(Variant* value, bool modifiable = true);

    /**
     * @brief Construct a \link Variable variable\endlink referencing a value (modifiable or constant reference)
     */
    static Variable ref(Variant& value, bool modifiable = true);

    /**
     * @brief Construct a \link Variable variable\endlink referencing a value (always constant reference)
     */
    static Variable constRef(const Variant& value);

    /**
     * @brief Construct a \link Variable variable\endlink owning a null value
     */
    static Variable null();

    /**
     * @brief Construct a \link Variable variable\endlink referencing the null constant (HMScript's NULL)
     */
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
