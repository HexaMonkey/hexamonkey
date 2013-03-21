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

#include "object.h"
#include "parser.h"

Object::Object(File& file) :
    _file(file),
    _beginningPos(_file.tellg()),
    _size(-1),
    _contentSize(0),
    _parent(nullptr),
    _rank(-1),
    _name("*"),
    _info(""),
    _children(0),
    _expandOnAddition(false),
    _parsingInProgress(false)
{
}


Object::iterator Object::begin()
{
    return _children.begin();
}

Object::iterator Object::end()
{
    return _children.end();
}

Object::iterator Object::last()
{
    if(_children.begin() != _children.end())
    {
        Object::iterator it = _children.end();
        --it;
        return  it;
    }
    else
        return _children.end();
}

Object::const_iterator Object::begin() const
{
    return _children.begin();
}

Object::const_iterator Object::end() const
{
    return _children.end();
}

Object::const_iterator Object::last() const
{
    if(_children.begin() != _children.end())
    {
        Object::const_iterator it = _children.end();
        --it;
        return  it;
    }
    else
        return _children.end();
}

Object::reverse_iterator Object::rbegin()
{
    return _children.rbegin();
}

Object::reverse_iterator Object::rend()
{
    return _children.rend();
}

Object::const_reverse_iterator Object::rbegin() const
{
    return _children.rbegin();
}

Object::const_reverse_iterator Object::rend() const
{
    return _children.rend();
}

int Object::numberOfChildren() const
{
    return _children.size();
}

Object *Object::access(int64_t index, bool forceParse)
{
    iterator it = begin();
    std::advance(it, index);
    if(it != end())
    {
        return *it;
    }
    if(forceParse && !parsed())
    {
        int64_t pos = file().tellg();
        exploreSome(128);
        file().seekg(pos, std::ios_base::beg);
        return access(index, true);
    }
    else
    {
        std::cerr<<"Requested variable not in range"<<std::endl;
        return nullptr;
    }
}

Object* Object::lookUp(const std::string &name, bool forceParse)
{
    std::map<std::string, Object*>::iterator it = _lookUpTable.find(name);
    if(it != _lookUpTable.end())
    {
        return it->second;
    }
    if(forceParse && !parsed())
    {
        int64_t pos = file().tellg();
        exploreSome(128);
        file().seekg(pos, std::ios_base::beg);
        return lookUp(name, true);
    }
    else
    {
        return nullptr;
    }
}

Object* Object::lookForType(const ObjectType &targetType, bool forceParse)
{
    for(Object::iterator it = begin(); it != end(); ++it)
    {
        if ((*it)->type().extendsDirectly(targetType))
        {
            return *it;
        }
    }
    if(forceParse && !parsed())
    {
        int64_t pos = file().tellg();
        exploreSome(128);
        file().seekg(pos, std::ios_base::beg);
        return lookForType(targetType, false);
    }
    else
    {
        return nullptr;
    }
}

void Object::seekBeginning()
{
    _file.seekg(_beginningPos,std::ios::beg);
}

void Object::seekEnd()
{
    _file.seekg(_beginningPos+size(),std::ios::beg);
}

void Object::seekObjectEnd()
{
    iterator it = last();
    if(it != end())
        (*it)->seekEnd();
    else
        seekBeginning();
}

std::streamoff Object::pos() const
{
    return _file.tellg() - _beginningPos;
}

Variant &Object::value()
{
    return _value;
}

const Variant &Object::value() const
{
    return _value;
}

void Object::parse()
{
    if(!_parsers.empty())
    {
        _parsers.back()->parse();
    }
}

bool Object::parseSome(int hint)
{
    if(!_parsers.empty())
    {
        return _parsers.back()->parseSome(hint);
    }
    return true;
}

void Object::explore(int level)
{
    if(level == 0)
    {
        return;
    }

    if(!parsed())
    {
        if(!file().good())
        {
            _file.clear();
            std::cerr<<"clearing file"<<std::endl;
        }
        seekObjectEnd();
        parse();
    }

    for(Object::iterator it = begin(); it != end(); ++it)
    {
        if(level == -1)
        {
            (*it)->explore(-1);
        }
        else
        {
            (*it)->explore(level-1);
        }
    }
}

bool Object::exploreSome(int hint)
{
    if(!parsed())
    {
        if(!file().good())
        {
            _file.clear();
            std::cerr<<"clearing file"<<std::endl;
        }
        seekObjectEnd();

        return parseSome(hint);
    }
    return true;
}

void Object::setType(const ObjectType &type)
{
    if(!type.isNull())
        _type = type;
}

void Object::setName(const std::string &name)
{
    _name = name;
}

void Object::setInfo(const std::string &info)
{
    _info = info;
}

Showcase &Object::showcase()
{
    return _showcase;
}

const Showcase &Object::showcase() const
{
    return _showcase;
}

const ObjectType &Object::type() const
{
    return _type.toObjectType();
}

const std::string &Object::name() const
{
    return _name.toString();
}

const std::string &Object::info() const
{
    return _info.toString();
}

File& Object::file()
{
    return _file;
}

std::streampos Object::beginningPos() const
{
    return _beginningPos;
}

std::streamoff Object::size() const
{
    return _size.toInteger();
}

Object* Object::parent()
{
    return _parent;
}

const Object *Object::parent() const
{
    return _parent;
}

int64_t Object::rank() const
{
    return _rank.toInteger();
}

void Object::clear()
{
    _children.clear();
    _lookUpTable.clear();
}

void Object::addParser(Parser *parser)
{
    if(parser != nullptr)
    {
        parse();
        parser->parseHead();
        _parsers.push_back(std::unique_ptr<Parser>(parser));
    }
}

std::ostream& Object::display(std::ostream& out, std::string prefix) const
{
    out << prefix << type() << " " << name();
    if(!info().empty())
        out << " = " << info();
    out<<std::endl;

    if(numberOfChildren() < 20)
    {
        for(Object::const_iterator it = begin(); it != end(); ++it)
        {
            (*it)->display(out, prefix+"    ");
        }
    }
    return out;
}

bool Object::parsed()
{
    if(!_parsers.empty())
    {
        return _parsers.back()->parsed();
    }
    return true;
}

std::ostream& operator <<(std::ostream& out, const Object& object)
{
    return object.display(out);
}
