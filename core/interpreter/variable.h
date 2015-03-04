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

#include "core/variant.h"

class VariableImplementation;

/**
 * @brief Accesser (getter and setter) for a value
 *
 * The variable can also be constant or modifiable. When the variable
 * is constant and the setter is used then the variable will be transformed
 * into a copy of the current value
 *
 * A variable owns a \link VariableImplementation variable implementation\endlink that actually define
 * the getter and setter for the \link Variable variable\endlink. Common implementations can be used via
 * the static constructors: copy, ref, constRef, null and nullConstant.
 */
class Variable
{
public:
    /**
     * @brief Construct an undefined \link Variable variable\endlink.
     */
    Variable();
    /**
     * @brief Construct a variable
     * @param implementation Must be a new created instance of a subclass of \link VariableImplementation variable implementation\endlink
     * @param modifiable
     */
    Variable(VariableImplementation* implementation, bool modifiable);

    /**
     * @brief Set the value of the \link Variable variable\endlink
     */
    void setValue(const Variant &value);

    /**
     * @brief Get the value of the \link Variable variable\endlink
     */
    Variant value() const;

    /**
     * @brief Set the variable as constant which prevents modification of the value
     */
    void setConstant();

    /**
     * @brief Transform the variable into a copy of the current value
     */
    void inPlaceCopy(bool modifiable = true);

    /**
     * @brief Check if the variable not been constructed by the default constructor.
     */
    bool isDefined() const;

    /**
     * @brief Construct a \link Variable variable\endlink copying and then owning a value
     */
    static Variable copy(const Variant& value, bool modifiable = true);

    /**
     * @brief Construct a \link Variable variable\endlink referencing a value (modifiable or constant reference)
     */
    static Variable ref(Variant& value, bool modifiable = true);

    /**
     * @brief Construct a \link Variable variable\endlink referencing a value (always constant reference)
     */
    static Variable constRef(const Variant& value);

    /**
     * @brief Construct a \link Variable variable\endlink referencing the null constant (HMScript's NULL)
     */
    static Variable nullConstant();

private:
    enum class Tag {undefined = 0, constant = 1, modifiable = 2};

    std::shared_ptr<VariableImplementation> _implementation;
    Tag _tag;
};

/**
 * @brief Actual implementation for variable
 */
class VariableImplementation
{
public:
    virtual ~VariableImplementation();
protected:
    friend class Variable;

    virtual void doSetValue(const Variant& value);
    virtual Variant doGetValue();
};

#endif // VARIABLE_H
