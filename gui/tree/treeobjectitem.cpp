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

#include <cctype>

#include "core/objecttypetemplate.h"
#include "gui/tree/treeobjectitem.h"

std::ostream& displayType(std::ostream& out, const ObjectType& type);
std::ostream& displayName(std::ostream& out, const std::string& name);
std::ostream& displayDecl(std::ostream& out, const ObjectType& type, const std::string& name);
std::ostream& displayVariant(std::ostream& out, const Variant& variant);

TreeObjectItem::TreeObjectItem(const ProgramLoader &programLoader, TreeItem *parent) :
    TreeItem(QList<QVariant>({"", "", ""}), parent),
    _index(0),
    filter(programLoader),
    _synchronised(false)
{
}

TreeObjectItem::TreeObjectItem(Object& object, const ProgramLoader &programLoader, TreeItem *parent) :
    TreeObjectItem(programLoader, parent)
{
    setObject(object);
}

Object &TreeObjectItem::object() const
{
    return *_object;
}

bool TreeObjectItem::synchronised()
{
    if(_synchronised)
    {
        return true;
    }
    else if(!_object->parsed())
    {
        return false;
    }
    else if(nextChild()==_object->end())
    {
        _synchronised = true;
        return true;
    }
    else
    {
        return false;
    }
}

Object::iterator TreeObjectItem::nextChild()
{
    Object::iterator nChild = _object->begin();
    std::advance(nChild, _index);
    return nChild;
}

Object::iterator TreeObjectItem::end()
{
    return _object->end();
}

void TreeObjectItem::advanceLastChild()
{
    ++_index;
}

void TreeObjectItem::setLastChildIndex(int64_t l)
{
    _index = l;
}

bool TreeObjectItem::filterObject(Object& object)
{
    return filter(object);
}

bool TreeObjectItem::updateFilter(const std::string &expression)
{
    return filter.setExpression(expression);
}

const std::string &TreeObjectItem::filterExpression()
{
    return filter.expression();
}

bool TreeObjectItem::hasStream() const
{
    return object().hasStream();
}

QVariant TreeObjectItem::clipboardValue() const
{
    const Variant& value = object().value();
    if(!value.isValueless()) {
        return QString(toStr(value).c_str());
    } else {
        return QVariant();
    }
}

bool TreeObjectItem::hasLinkTo() const
{
    return object().hasLinkTo();
}

qint64 TreeObjectItem::linkTo() const
{
    return object().linkTo();
}

void TreeObjectItem::onChildrenRemoved()
{
    _synchronised=false;
    _index = 0;
}

void TreeObjectItem::setObject(Object &object)
{
    _object = &object;
}

void TreeObjectItem::doLoad() const
{
    std::stringstream showcasePadding;
    {

        std::stringstream S;
        if(object().name() == "#")
        {
            std::string rank = toStr(object().rank());
            S << "[";
            S << "<span style=\"color:#000080;\">";
            S << rank;
            S << "</span>";
            S << "]";

            for(size_t i = rank.size(); i < 4; ++i)
            {
                showcasePadding<<"&nbsp;&nbsp;";
            }
        }
        else
        {
            displayDecl(S, object().type(), object().name());
        }

        if (!object().value().isNull()) {
            S << "&nbsp;=&nbsp;";
            displayVariant(S, object().value());
        }

        int attributeCount = object().maxAttributeNumber() + object().showcasedAttributes().size();
        if (attributeCount > 0)
        {
            S << showcasePadding.str();
            S << "&nbsp;&nbsp;(";

            int i;
            for (i = 0; i < object().maxAttributeNumber(); ++i) {
                Variant* value = object().attributeValue(i);
                if (value) {
                    displayVariant(S, *value);
                }
                if (i < attributeCount - 1) {
                    S << ",&nbsp;";
                }
            }

            for (int j = 0; i < attributeCount; ++i, ++j) {
                const std::string& key = object().showcasedAttributes()[j];
                Variant* value = object().attributeValue(key);
                if (value) {
                    displayName(S, key);
                    S << " = ";
                    displayVariant(S, *value);
                }
                if (i < attributeCount - 1) {
                    S << ",&nbsp;";
                }
            }
            S << ")";
        }


        itemData()[0] = S.str().c_str();
    }

    {
        std::stringstream S;
        S << "<span style=\"color:#000080;\">";
        S << "0x" << std::setfill('0') << std::setw(8) << std::hex << object().beginningPos()/8;
        S << "</span>";
        itemData()[1] = S.str().c_str();
    }

    {
        std::stringstream S;
        S << "<span style=\"color:#000080;\">";
        S << sizeDisplay(object().size());
        S << "</span>";
        itemData()[2] = S.str().c_str();
    }
}

std::ostream& displayType(std::ostream& out, const ObjectType& type)
{
    if (!type.elementType().isNull()) {
        displayType(out, type.elementType().toObjectType());
        out << "&nbsp;" << "[";
        if (!type.elementCount().isValueless()) {
            displayVariant(out, type.elementCount());
        }
        out << "]";
    } else {
        out << "<span style=\"color:#800080;\">" << type.name() << "</span>";

        int n = type.numberOfDisplayableParameters();

        if(n>0)
        {
            out<<"(";
            for(int i = 0; i < n; ++i)
            {
                if(type.parameterSpecified(i))
                {
                    displayVariant(out, type.parameterValue(i));
                }
                else
                {
                    out<<type.typeTemplate().parameterName(i);
                }
                if(i < n-1)
                    out<<", ";
            }
            out<<")";
        }
    }
    return out;
}

std::ostream& displayVariant(std::ostream& out, const Variant& variant)
{
    switch(variant.type())
    {
        case Variant::integerType:
        case Variant::unsignedIntegerType:
        case Variant::floatingType:
            out<<"<span style=\"color:#000080;\">"<<variant<<"</span>";
            break;

        case Variant::stringType:
            out<<"<span style=\"color:#008000;\">\""<<variant<<"\"</span>";
            break;

        case Variant::objectType:
            displayType(out, variant.toObjectType());
            break;

        default:
            break;
    }

    return out;
}

std::ostream& displayName(std::ostream& out, const std::string& name)
{
    if(!name.empty() && name != "*" && name[0] != '_') {
        out<<"<span style=\"color:#800000;\">"<<name<<"</span>";
    }
    return out;
}

std::ostream& displayDecl(std::ostream& out, const ObjectType& type, const std::string& name)
{
    if (!type.elementType().isNull()) {
        displayDecl(out, type.elementType().toObjectType(), name);
        out << "&nbsp;" << "[";
        if (!type.elementCount().isValueless()) {
            displayVariant(out, type.elementCount());
        }
        out << "]";
    } else {
        displayType(out, type);
        out << "&nbsp;";
        displayName(out, name);
    }

    return out;
}
