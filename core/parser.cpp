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

#include "core/parser.h"
#include "core/log/logmanager.h"

ObjectType errorType;

Parser::Parser(Object& object)
    : _object(object),
      _headParsed(false),
      _parsed(false),
      _tailParsed(false),
      _hasHead(true),
      _hasTail(true)
{
}

void Parser::parseHead()
{
    Parsing parsing(*this);
    if(parsing.available() && !_headParsed)
    {
        doParseHead();
        setHeadParsed();
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
    }
}

bool Parser::parseSome(int hint)
{
    parseHead();
    Parsing parsing(*this);
    if(parsing.available() && !_parsed && doParseSome(hint))
    {
        _parsed = true;
    }
    return _parsed;
}

void Parser::parseTail()
{
    if(!parsed())
    {
        parse();
    }

    if(!_tailParsed)
    {
        doParseTail();
        _tailParsed = true;
    }
}

bool Parser::headParsed() const
{
    return _headParsed;
}

bool Parser::parsed() const
{
    return _parsed;
}

bool Parser::tailParsed() const
{
    return _tailParsed || (!_hasTail && _parsed);
}

ObjectType *Parser::modifiableType()
{
    if (_headParsed) {
        Log::error("Cannot modify parser type's once it has been parsed");
        return nullptr;
    } else {
        return &_object._type.toObjectType();
    }
}

const ObjectType &Parser::constType() const
{
    if (_headParsed) {
        return *_type;
    } else {
        return _object._type.toObjectType();
    }
}

void Parser::setSize(int64_t size)
{
    _object.setSize(size);
}

void Parser::setName(const std::string &name)
{
    _object._name = name;
}

void Parser::setValue(const Variant &value)
{
    _object._value = value;
}

File &Parser::file()
{
    return _object._file;
}

int64_t Parser::availableSize() const
{
    if(_object.file().good()) {
        return _object.size() - _object.pos();
    } else {
        return -1;
    }
}

void Parser::setExpandOnAddition()
{
    _object._expandOnAddition = true;
}

Object &Parser::object()
{
    return _object;
}

void Parser::setParsed()
{
    setHeadParsed();
    _parsed = true;
}

void Parser::setNoHead()
{
    _hasHead = false;
    setHeadParsed();
}

void Parser::setNoTail()
{
    _hasTail = false;
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

void Parser::doParseTail()
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

void Parser::setHeadParsed()
{
    if (!_headParsed) {
        _type.reset(new ObjectType(_object._type.toObjectType()));
        _headParsed = true;
    }
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
        setHeadParsed();
        _parsed = true;
        _tailParsed = true;
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
