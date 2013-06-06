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

/*!
 * @brief The ObjectType class
 */
class ObjectType
{
public:
    ObjectType();
    ObjectType(const ObjectTypeTemplate& typeTemplate);

    const ObjectTypeTemplate &typeTemplate() const;
    const Variant& parameterValue(size_t index) const;
    bool parameterSpecified(size_t index) const;
    void setParameter(size_t index, const Variant& value);
    template<typename... Args> void setParameters(Args... args){return _setParameters(0, args...);}

    void setParameterByName(const std::string& parameterName, const Variant& value);

    const ObjectType &importParameters(const ObjectType& type);

    bool isNull() const;

    bool extendsDirectly(const ObjectType& other) const;

    std::ostream& display(std::ostream& out) const;

    friend void swap(ObjectType& a, ObjectType& b);
    ObjectType& operator=(ObjectType other);

private:
    friend class MutableTypeScope;
    friend class ConstTypeScope;
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
