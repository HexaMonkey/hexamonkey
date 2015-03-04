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

#include "core/objecttype.h"
#include "core/objecttypetemplate.h"

Variant undefinedVariant;
Variant nullVariant = Variant::null();

ObjectType::ObjectType() : _typeTemplate(&nullTypeTemplate)
{
}

ObjectType::ObjectType(const ObjectTypeTemplate& _typeTemplate)
    : _typeTemplate(&_typeTemplate),
      _parametersValue(_typeTemplate.numberOfParameters()),
      _name(_typeTemplate.name())
{
}

const ObjectTypeTemplate& ObjectType::typeTemplate() const
{
    return *_typeTemplate;
}

const Variant& ObjectType::parameterValue(size_t index) const
{
    if (index < _parametersValue.size()) {
        return _parametersValue[index];
    } else {
        return undefinedVariant;
    }
}

bool ObjectType::parameterSpecified(size_t index) const
{
    return (index < _parametersValue.size()) && (!_parametersValue[index].isUndefined());
}

void ObjectType::setParameter(size_t index, const Variant &value)
{
    while(index >= _parametersValue.size()) {
        _parametersValue.push_back(undefinedVariant);
    }

    _parametersValue.at(index).setValue(value);
}

const std::string &ObjectType::name() const
{
    return _name.toString();
}

void ObjectType::setName(const std::string &name)
{
    _name.setValue(name);
}

const ObjectType& ObjectType::importParameters(const ObjectType& other)
{
    for(int i = 0; i < typeTemplate().numberOfParameters();++i)
    {
        if(!parameterSpecified(i))
        {
            for(int j = 0; j < other.typeTemplate().numberOfParameters();++j)
            {
                if(typeTemplate().parameterName(i) == other.typeTemplate().parameterName(j))
                {

                    setParameter(i, other.parameterValue(j));
                }
            }
        }
    }
    return *this;
}

bool ObjectType::extendsDirectly(const ObjectType& other) const
{
    if(*_typeTemplate != *other._typeTemplate)
    {
        return false;
    }

    for(int i = 0; i < typeTemplate().numberOfParameters();++i)
    {
        if(other.parameterValue(i).type() != Variant::valuelessType
           && parameterValue(i) != other.parameterValue(i))
        {
            return false;
        }
    }
    return true;
}

bool ObjectType::isNull() const
{
    return (*_typeTemplate == nullTypeTemplate);
}

void swap(ObjectType& a, ObjectType& b)
{
    using std::swap;
    swap(a._typeTemplate,b._typeTemplate);
    swap(a._parametersValue, b._parametersValue);
    swap(a._elementCount, b._elementCount);
    swap(a._name, b._name);

}

ObjectType& ObjectType::operator =(ObjectType other)
{
    swap(*this, other);
    return *this;
}

bool operator==(const ObjectType& a, const ObjectType& b)
{
    if(a.typeTemplate() != b.typeTemplate())
        return false;

    for(int i = 0; i < a.typeTemplate().numberOfParameters();++i)
    {
        if(a.parameterValue(i) != b.parameterValue(i))
            return false;
    }
    return true;
}

bool operator!=(const ObjectType& a, const ObjectType& b)
{
    return !(a==b);
}

bool operator< (const ObjectType& a, const ObjectType& b)
{
    if(a.typeTemplate() != b.typeTemplate())
        return (a.typeTemplate() < b.typeTemplate());

    for(int i = 0; i < a.typeTemplate().numberOfParameters();++i)
    {
        if(a.parameterValue(i) != b.parameterValue(i))
            return (a.parameterValue(i) < b.parameterValue(i));
    }
    return false;
}

std::ostream& ObjectType::display(std::ostream& out) const
{
    if (!elementType().isValueless()) {
        out << elementType() << "[";
        if (!elementCount().isValueless()) {
            out << elementCount();
        }
        out << "]";
    } else {
        out << name();
        const int n = numberOfDisplayableParameters();
        if(n>0)
        {
            out<<"(";
            for(int i = 0; i < n; ++i)
            {
                out<<parameterValue(i);

                if(i < n-1)
                    out<<", ";
            }
            out<<")";
        }
    }
    return out;
}

int ObjectType::numberOfParameters() const
{
    return _parametersValue.size();
}

int ObjectType::numberOfDisplayableParameters() const
{
    int n = typeTemplate().numberOfParameters();
    while(n >= 1 && (typeTemplate().isParameterPrivate(n-1) || !parameterSpecified(n-1)))
    {
        n--;
    }
    return n;
}

const Variant &ObjectType::elementType() const
{
    return _elementType;
}

void ObjectType::setElementType(const ObjectType &type)
{
    _elementType.setValue(type);
}

const Variant &ObjectType::elementCount() const
{
    return _elementCount;
}

void ObjectType::setElementCount(long long count)
{
    _elementCount.setValue(count);
}

std::ostream& operator<<(std::ostream& out, const ObjectType& type)
{
    return type.display(out);
}


void ObjectType::_setParameters(int first)
{
}
