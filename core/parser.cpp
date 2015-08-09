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
#include "core/parsingexception.h"

ObjectType errorType;

Parser::Parser(Object& object)
    : _object(object),
      _headParsed(false),
      _parsed(false),
      _tailParsed(false),
      _hasHead(true),
      _hasTail(true),
      _isClean(false)
{

}

void Parser::parseHead()
{
    Object::Parsing parsing(object());
    if(parsing.isAvailable() && !_headParsed)
    {
        try {
            doParseHead();
            setHeadParsed();
        } catch (const ParsingException& exception) {
            handleParsingException(exception);
        }
    }
}

void Parser::parse()
{
    parseHead();

    {
        Object::Parsing parsing(object());
        if(parsing.isAvailable() && !_parsed)
        {
            try {
                doParse();
                _parsed = true;
            } catch (const ParsingException& exception) {
                handleParsingException(exception);
            }
        }
    }
}

bool Parser::parseSome(int hint)
{
    parseHead();
    Object::Parsing parsing(object());
    try {
        if(parsing.isAvailable() && !_parsed && doParseSome(hint))
        {
            _parsed = true;
        }
    } catch (const ParsingException& exception) {
        handleParsingException(exception);
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
        try {
            doParseTail();
            _tailParsed = true;
        } catch (const ParsingException& exception) {
            handleParsingException(exception);
        }
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
        return &object().type();
    }
}

const ObjectType &Parser::constType() const
{
    if (_headParsed) {
        return *_type;
    } else {
        return object().type();
    }
}

void Parser::clean()
{
    if (!_isClean) {
        _isClean = true;
        doClean();
    }
}

int64_t Parser::availableSize() const
{
    if(_object.file().good()) {
        return _object.size() - _object.pos();
    } else {
        return -1;
    }
}

Object &Parser::object()
{
    return _object;
}

const Object &Parser::object() const
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

bool Parser::needTailParsing()
{
    return doNeedTailParsing();
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

bool Parser::doNeedTailParsing()
{
    return false;
}

void Parser::doClean()
{
}

void Parser::setHeadParsed()
{
    if (!_headParsed) {
        _type.reset(new ObjectType(object().type()));
        _headParsed = true;
    }
}

void Parser::handleParsingException(const ParsingException &exception)
{
    Log::error(exception.what());
    object().invalidate();
}

SimpleParser::SimpleParser(Object &object) : Parser(object)
{
}

void SimpleParser::parseHead()
{
    Object::Parsing parsing(object());

    if(parsing.isAvailable() && !_headParsed)
    {
        try {
            doParseHead();
            setHeadParsed();
            _parsed = true;
            _tailParsed = true;
        } catch (const ParsingException& exception) {
            handleParsingException(exception);
        }
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
