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

#include "parser.h"



Parser::Parser(Object& object)
    : _object(object),
      _headParsed(false),
      _parsed(false)
{
}

void Parser::parseHead()
{
    Parsing parsing(*this);
    if(parsing.available() && !_headParsed)
    {
        doParseHead();
        _headParsed = true;
    }
}

void Parser::parse()
{
    parseHead();
    Parsing parsing(*this);
    if(parsing.available() && !_parsed)
    {
        doParse();
        _parsed = true;
        cleanUp();
    }
}

bool Parser::parseSome(int hint)
{
    parseHead();
    Parsing parsing(*this);
    if(parsing.available() && !_parsed && doParseSome(hint))
    {
        _parsed = true;
        cleanUp();
    }
    return _parsed;
}

bool Parser::parsed()
{
    return _parsed;
}

ObjectType &Parser::type()
{
    return _object._type.toObjectType();
}

void Parser::setSize(int64_t size)
{
    _object._size = size;
}

void Parser::setName(const std::string &name)
{
    _object._name = name;
}

void Parser::setInfo(const std::string &info)
{
    _object._info = info;
}

void Parser::setValue(const Variant &value)
{
    _object._value = value;
}

Showcase &Parser::showcase()
{
    return _object.showcase();
}

File &Parser::file()
{
    return _object._file;
}

int64_t Parser::availableSize() const
{
    if(_object.file().good())
        return _object.size() - _object.pos();
    else
        return -1;

}

void Parser::setExpandOnAddition()
{
    _object._expandOnAddition = true;
}

Object *Parser::access(int64_t index, bool forceParse)
{
    return _object.access(index, forceParse);
}

Object *Parser::lookUp(const std::string& name, bool forceParse)
{
    return _object.lookUp(name, forceParse);
}

Object *Parser::lookForType(const ObjectType &type, bool forceParse)
{
    return _object.lookForType(type, forceParse);
}

Object &Parser::object()
{
    return _object;
}

void Parser::doParseHead()
{
}

void Parser::doParse()
{
}

bool Parser::doParseSome(int hint)
{
    doParse();
    return true;
}

void Parser::cleanUp()
{
}

bool Parser::lockObject()
{
    if(_object._parsingInProgress == true)
        return false;
    else
    {
        _object._parsingInProgress = true;
        return true;
    }
}

void Parser::unlockObject()
{
    _object._parsingInProgress = false;
}


SimpleParser::SimpleParser(Object &object) : Parser(object)
{
}

void SimpleParser::parseHead()
{
    Parsing parsing(*this);

    if(parsing.available() && !_headParsed)
    {
        doParseHead();
        _headParsed = true;
        _parsed = true;
        cleanUp();
    }
}

void SimpleParser::doParse()
{
    parseHead();
}

bool SimpleParser::doParseSome(int /*hint*/)
{
    parseHead();
    return true;
}
