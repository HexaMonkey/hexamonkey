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
std::ostream& displayInfo(std::ostream& out, const std::string& info);
std::ostream& displayVariant(std::ostream& out, const Variant& variant);
std::string getString(const Object &object);

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
    const Variant& info = object().info();
    if(!info.isNull())
    {
       const std::string& infoString = info.toString();
       if(!infoString.empty() && infoString[0] == '"' && infoString[infoString.size() - 1] == '"')
           return QString(infoString.substr(1, infoString.size()-2).c_str());
       else
           return QString(infoString.c_str());
    }
    else
    {

        const Variant& value = object().value();
        if(!value.isNull())
        {
            return QString(toStr(value).c_str());
        }
        else
        {
            return QVariant();
        }
    }
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

        displayInfo(S, getString(object()));

        if(itemData().length() <3)
        {
            std::cerr<<"Error: item data too short"<<std::endl;
                return;
        }
        if(!object().showcase().empty())
        {
            S << showcasePadding.str();
            S << "&nbsp;&nbsp;(";
            Showcase::const_iterator it = object().showcase().begin();
            while(true)
            {
                displayName(S, *it);
                auto child = object().lookUp(*it);
                if (child) {
                    displayInfo(S, getString(*child));
                }

                ++it;
                if(it == object().showcase().end())
                    break;
                S << ",&nbsp;";
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
        if (!type.elementCount().isNull()) {
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
        case Variant::integer:
            out<<"<span style=\"color:#000080;\">"<<variant.toInteger()<<"</span>";
            break;

        case Variant::unsignedInteger:
            out<<"<span style=\"color:#000080;\">"<<variant.toUnsignedInteger()<<"</span>";
            break;

        case Variant::floating:
            out<<"<span style=\"color:#000080;\">"<<variant.toDouble()<<"</span>";
            break;

        case Variant::string:
            out<<"<span style=\"color:#008000;\">\""<<variant.toString()<<"\"</span>";
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
    if(!name.empty() && name != "*" && name[0] != '_')
    {
        out<<"<span style=\"color:#800000;\">"<<name<<"</span>";
    }
    return out;
}

std::ostream& displayDecl(std::ostream& out, const ObjectType& type, const std::string& name)
{
    if (!type.elementType().isNull()) {
        displayDecl(out, type.elementType().toObjectType(), name);
        out << "&nbsp;" << "[";
        if (!type.elementCount().isNull()) {
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

std::ostream& displayInfo(std::ostream& out, const std::string& info)
{
    if(!info.empty())
    {
        out << "&nbsp;=&nbsp;";
        unsigned int i = 0;
        if(info[0] == '"')
        {
            out <<"<span style=\"color:#008000;\">\"";
            for(++i; i < info.size() && info[i] != '"'; ++i)
            {
                out << info[i];
            }

            out << info[i];
            ++i;
            out << "</span>";
        }
        else if(std::isdigit(info[0]))
        {
            out <<"<span style=\"color:#000080;\">";
            for(; i < info.size() && info[i]!=' '; ++i)
            {
                out << info[i];
            }
            out << "</span>";
        }

        for(; i < info.size(); ++i)
        {
            out << info[i];
        }
    }
    return out;
}

std::string getString(const Object& object)
{
    if(!object.info().isNull())
        return object.info().toString();
    else
    {
        const Variant& value = object.value();
        switch(value.type())
        {
            case Variant::string:
                return "\""+value.toString()+"\"";

            case Variant::integer:
            case Variant::unsignedInteger:
            case Variant::floating:
                return toStr(value);

            case Variant::objectType:
            {
                std::stringstream O;
                displayType(O, value.toObjectType());
                return O.str();
            }

            default:
                return "";
        }
    }
    return "";
}
