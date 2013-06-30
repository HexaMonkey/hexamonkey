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

#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

#include <vector>

#include "variant.h"

class ObjectTypeTemplate;

/**
 * @brief Symbol representing a class of \link Object objects\endlink that have the
 * same semantics and are to be parsed in the same way.
 *
 * The \link Module modules\endlink use the object's \link ObjectType type\link and its
 * inheritence model for \link ObjectType types\link to generate \link Parser parser\endlink.
 *
 * A \link ObjectType type\endlink is defined by a \link ObjectTypeTemplate type template\endlink
 * and values stored as \link Variant variants\endlink specified for some of its parameters. The easiest way to construct a
 * \link ObjectType type\endlink is to use the \link ObjectTypeTemplate type template\endlink's
 * operator().
 */
class ObjectType
{
public:
    /**
     * @brief Construct a \link ObjectType type\endlink with the null
     * \link type template ObjectTypeTemplate\endlink
     */
    ObjectType();
    /**
     * @brief Construct a \link ObjectType type\endlink with the
     * \link type template ObjectTypeTemplate\endlink given.
     *
     * The parameters are set to the null \link Variant variant\endlink, and can be
     * set to other values subsequently.
     */
    ObjectType(const ObjectTypeTemplate& typeTemplate);

    /**
     * @brief Get the \link ObjectTypeTemplate type template\endlink the \link ObjectType type\endlink implements
     * @return
     */
    const ObjectTypeTemplate &typeTemplate() const;
    /**
     * @brief Get the value of a parameter by its index.
     *
     * Raise an exception if out of bound
     */
    const Variant& parameterValue(size_t index) const;

    /**
     * @brief Check if a parameter is non-null
     */
    bool parameterSpecified(size_t index) const;

    /**
     * @brief Set the value of a parameter
     *
     * Raise an exception if out of bound
     */
    void setParameter(size_t index, const Variant& value);

    /**
     * @brief Set a value for the first parameters
     */
    template<typename... Args> void setParameters(Args... args){return _setParameters(0, args...);}

    /**
     * @brief Set the parameters not specified (i.e. with a null value)
     * with the value of the other type's parameter with the same name
     */
    const ObjectType &importParameters(const ObjectType& other);

    /**
     * @brief Check if the \link ObjectTypeTemplate type template\endlink is null
     */
    bool isNull() const;

    /**
     * @brief Check if the \link ObjectType other type\endlink has the same
     * \link ObjectTypeTemplate type template\endlink and that the value for
     * all of the parameters specified by the \link ObjectType other type\endlink
     * are the same.
     *
     * The \link ObjectType type\endlink can have however parameters specified that
     * are not by the other.
     *
     * This function define the basis of extension for two \link ObjectType types\endlink
     * with the same \link ObjectTypeTemplate type template\endlink.
     */
    bool extendsDirectly(const ObjectType& other) const;

    /**
     * @brief Output a representation of the \link ObjectType type\endlink into a stream
     *
     * Same effect as <<
     */
    std::ostream& display(std::ostream& out) const;

    friend void swap(ObjectType& a, ObjectType& b);
    ObjectType& operator=(ObjectType other);

private:
    friend class TypeScope;
    const ObjectTypeTemplate* _typeTemplate;
    std::vector<Variant> _parametersValue;

    void _setParameters(int first)
    {
    }

    template<typename... Args>
    void _setParameters(int first, const Variant& v, Args... args)
    {
        setParameter(first, v);
        _setParameters(first+1, args...);
    }
};

bool operator==(const ObjectType& a, const ObjectType& b);
bool operator!=(const ObjectType& a, const ObjectType& b);
bool operator< (const ObjectType& a, const ObjectType& b);

std::ostream& operator<<(std::ostream& out, const ObjectType& type);

#endif // OBJECTTYPE_H
