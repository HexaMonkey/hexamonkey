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

#include <algorithm>

#include "core/object.h"
#include "core/parser.h"

#define BUFFER_SIZE 1048576

Object::Object(File& file) :
    _file(file),
    _beginningPos(_file.tellg()),
    _size(-1),
    _contentSize(0),
    _parent(nullptr),
    _lastChild(nullptr),
    _rank(-1),
    _name("*"),
    _pos(0),
    _children(0),
    _expandOnAddition(false),
    _parsedCount(0),
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
    if(index >=0 && index < numberOfChildren())
    {
        return _children[index];
    }
    if(forceParse && !parsed())
    {
        int64_t pos = file().tellg();
        int n = numberOfChildren();
        exploreSome(128);
        if(n == numberOfChildren())
        {
            std::cerr<<"Parsing locked for index "<<index<<std::endl;
            return nullptr;
        }
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
        int n = numberOfChildren();
        exploreSome(128);
        if(n == numberOfChildren())
        {
            std::cerr<<"Parsing locked for look up "<<name<<std::endl;
            return nullptr;
        }
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

void Object::dump(std::ostream &out) const
{
    // make sure it is not packetised ?
    std::ifstream in  (file().path(), std::ios::in | std::ios::binary);

    if(size() == -1)
    {
        return;
    }

    size_t done = 0;
    size_t n = size()/8;

    in.seekg(beginningPos()/8);

    char buffer[BUFFER_SIZE];
    //Copy file part by chunks
    while(done < n)
    {

        size_t chunkSize = std::min<size_t>(n - done, BUFFER_SIZE);
        in.read(buffer, chunkSize);

        out.write(buffer, chunkSize);
        done += chunkSize;
    }
}

void Object::dumpToFile(const std::string &path) const
{
    std::ofstream out (path, std::ios::out | std::ios::binary);
    dump(out);
}

bool Object::hasStream() const
{
    try {
        return !_stateMap.at("_stream").isNull();
    } catch(std::out_of_range) {
        return false;
    }
}

void Object::dumpStream(std::ostream &out)
{
    FragmentedFile file(this, nullptr);
    file.dump(out);
    // ModuleLoader moduleLoader;
    // moduleLoader.addModule("ts", new StreamModule());
    // StreamFile file(this);

}

void Object::dumpStreamToFile(const std::string &path)
{
    std::ofstream out (path, std::ios::out | std::ios::binary);
    dumpStream(out);
}

void Object::seekBeginning()
{
    _file.seekg(_beginningPos,std::ios::beg);
}

void Object::seekEnd()
{
    if(size() != -1)
        _file.seekg(_beginningPos+size(), std::ios::beg);
    else
        _file.seekg(_beginningPos, std::ios::beg);
}

void Object::seekObjectEnd()
{
    _file.seekg(_beginningPos+_pos.toInteger(), std::ios::beg);
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
    parseBody();
    parseTail();
}

void Object::parseBody()
{
    while(_parsedCount < _parsers.size())
    {
        auto& parser = _parsers[_parsedCount];
        parser->parse();
        ++_parsedCount;
    }
}

bool Object::parseSome(int hint)
{
    size_t initialCount = _children.size();

    while(_parsedCount < _parsers.size() && _children.size() < initialCount+hint)
    {
        auto& parser = _parsers[_parsedCount];
        if(parser->parseSome(initialCount+hint-_children.size()))
        {
            ++_parsedCount;
        }
        else
        {
            return false;
        }
    }

    if(_parsedCount == _parsers.size())
    {
        parseTail();
        return true;
    }
    else
    {
        return false;
    }
}

void Object::parseTail()
{
    for(auto& parser : _parsers)
    {
        if(parser)
        {
            parser->parseTail();
            parser.reset();
        }
    }
}

void Object::explore(int depth)
{
    if(depth == 0)
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
        if(depth == -1)
        {
            (*it)->explore(-1);
        }
        else
        {
            (*it)->explore(depth-1);
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

const Variant &Object::info() const
{
    return _info;
}

File &Object::file()
{
    return _file;
}

const File &Object::file() const
{
    return _file;
}

std::streampos Object::beginningPos() const
{
    return _beginningPos;
}

std::streamoff Object::size() const
{
    if(_size.hasNumericalType())
        return _size.toInteger();
    else
        return -1LL;
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

void Object::addParser(Parser *parser)
{
    if(parser != nullptr)
    {
        if(parser->hasHead())
        {
            parseBody();
            parser->parseHead();
        }
        _parsers.push_back(std::unique_ptr<Parser>(parser));
    }
}

std::ostream& Object::display(std::ostream& out, std::string prefix) const
{
    out << prefix << type() << " " << name();
    if(!info().isNull())
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
    for(int i = _parsers.size() - 1; i >= 0; --i)
    {
        auto& parser = _parsers[i];
        if(parser && !parser->tailParsed())
        {
            return false;
        }
    }
    return true;
}

std::ostream& operator <<(std::ostream& out, const Object& object)
{
    return object.display(out);
}
