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
#include "treeobjectitem.h"
#include "objecttypetemplate.h"

std::ostream& displayType(std::ostream& out, const ObjectType& type);
std::ostream& displayName(std::ostream& out, const std::string& name);
std::ostream& displayDecl(std::ostream& out, const ObjectType& type, const std::string& name);
std::ostream& displayInfo(std::ostream& out, const std::string& info);

TreeObjectItem::TreeObjectItem(Object& object, Interpreter *interpreter, TreeItem *parent) :
    TreeItem(QList<QVariant>({"", "", ""}), parent),
    _object(object),
    _index(0),
    filter(interpreter),
    _synchronised(false)
{
    connect(this, SIGNAL(childrenRemoved()), this, SLOT(onChildrenRemoved()));
}

Object &TreeObjectItem::object() const
{
    return _object;
}

bool TreeObjectItem::synchronised()
{
    if(_synchronised)
    {
        return true;
    }
    else if(!_object.parsed())
    {
        return false;
    }
    else if(nextChild()==_object.end())
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
    Object::iterator nChild = _object.begin();
    std::advance(nChild, _index);
    return nChild;
}

Object::iterator TreeObjectItem::end()
{
    return _object.end();
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
    return filter.filterChildren(object);
}

bool TreeObjectItem::updateFilter(const std::string &expression)
{
    return filter.setExpression(expression);
}

const std::string &TreeObjectItem::filterExpression()
{
    return filter.expression();
}

void TreeObjectItem::onChildrenRemoved()
{
    _synchronised=false;
    _index = 0;
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

        if(!object().info().empty())
            displayInfo(S, object().info());
        else
        {
            const Variant& value = object().value();
            switch(value.type())
            {
                case Variant::string:
                    displayInfo(S, "\""+value.toString()+"\"");
                    break;

                case Variant::integer:
                case Variant::unsignedInteger:
                case Variant::floating:
                    displayInfo(S, toStr(value));
                    break;

                case Variant::objectType:
                {
                    std::stringstream O;
                    displayType(O, value.toObjectType());
                    displayInfo(S, O.str());
                }
                break;

                default:
                    break;
            }

        }


        if(!object().showcase().empty())
        {
            S << showcasePadding.str();
            S << "&nbsp;&nbsp;(";
            Showcase::const_iterator it = object().showcase().begin();
            while(true)
            {
                displayName(S, *it);
                displayInfo(S, object().lookUp(*it)->info());

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

QList<QVariant> display(Object& object)
{
    QList<QVariant> itemData;

    return itemData;
}

std::ostream& displayType(std::ostream& out, const ObjectType& type)
{
    const std::string templateName = type.typeTemplate().name();

    out << "<span style=\"color:#800080;\">" << templateName << "</span>";

    int n = type.typeTemplate().numberOfParameters();
    while(n>0 && !type.parameterSpecified(n-1)) --n;
    if(n>0)
    {
        out<<"(";
        for(int i = 0; i < n; ++i)
        {
            if(type.parameterSpecified(i))
            {
                const Variant& variant = type.parameterValue(i);
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
    return out;
}

std::ostream& displayName(std::ostream& out, const std::string& name)
{
    if(name != "*")
    {
        out<<"<span style=\"color:#800000;\">"<<name<<"</span>";
    }
    return out;
}

std::ostream& displayDecl(std::ostream& out, const ObjectType& type, const std::string& name)
{

    if(type.typeTemplate().name() == "Array")
    {
        displayDecl(out, type.parameterValue(0).toObjectType(), name);
        out << "&nbsp;";
        out << "[]";
    }
    else if(type.typeTemplate().name() == "Tuple")
    {
        displayDecl(out, type.parameterValue(0).toObjectType(), name);
        out << "&nbsp;";
        out << "[";
        out << "<span style=\"color:#000080;\">";
        out << type.parameterValue(1).toInteger();
        out << "</span>";
        out << "]";

    }
    else
    {
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
