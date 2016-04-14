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
      _sharedType(new std::pair<bool, ObjectType>(false, ObjectType())),
      _headParsed(false),
      _parsed(false),
      _tailParsed(false),
      _hasHead(true),
      _hasTail(true)
{

}

void Parser::parseHead()
{
    if (!_headParsed)
    {
        try {
            Object::ParsingContext context(object());
            doParseHead();
            setHeadParsed();
        } catch (const ParsingException& exception) {
            handleParsingException(exception);
        } catch (const Object::ParsingContext::LockException&) {

        }
    }
}

void Parser::parse()
{
    parseHead();

    if (!_parsed)
    {
        try {
            Object::ParsingContext context(object());
            doParse();
            _parsed = true;
        } catch (const ParsingException& exception) {
            handleParsingException(exception);
        } catch (const Object::ParsingContext::LockException&) {

        }
    }
}

bool Parser::parseSome(int hint)
{
    parseHead();
    if (!_parsed)
    {
        try {
            Object::ParsingContext context(object());
            if (doParseSome(hint))
            {
                _parsed = true;
            }
        } catch (const ParsingException& exception) {
            handleParsingException(exception);
        } catch (const Object::ParsingContext::LockException&) {

        }
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
            Object::ParsingContext context(object());
            doParseTail();
            _tailParsed = true;
        } catch (const ParsingException& exception) {
            handleParsingException(exception);
        } catch (const Object::ParsingContext::LockException&) {

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
        return _sharedType->second;
    } else {
        return object().type();
    }
}

std::shared_ptr<std::pair<bool, ObjectType> > Parser::sharedType() const
{
    return _sharedType;
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

bool Parser::doParseSome(int /*hint*/)
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

void Parser::setHeadParsed()
{
    if (!_headParsed) {
        _headParsed = true;
        _sharedType->first = true;
        _sharedType->second = object().type();
    }
}

void Parser::handleParsingException(const ParsingException &exception)
{
    Log::error(exception.what());
    object().invalidate();
}

void parseBytePattern(const std::string &pattern, std::vector<std::vector<unsigned char> >& byteList, std::vector<std::vector<unsigned char> >& maskList)
{
    bool isCurrentMask = false;
    std::vector<unsigned char> current;
    std::string buffer = "00";
    for (int i = 0, n = pattern.size(); i < n; ++i)
    {
        char ch = pattern[i];
        if (ch == '&') {
            byteList.emplace_back(std::move(current));
            current.resize(0);
            isCurrentMask = true;
        } else if (ch == '|') {
            if (isCurrentMask) {
                maskList.emplace_back(std::move(current));
                current.resize(0);
                isCurrentMask = false;
            } else {
                maskList.emplace_back(std::vector<unsigned char>(current.size(), 0xff));
                byteList.emplace_back(std::move(current));
                current.resize(0);
            }
        } else if (ch == ' ') {
            /** ignore **/
        } else if (i < n - 1) {
            buffer[0] = ch;
            ++i;
            buffer[1] = pattern[i];
            current.insert(current.begin(), strtol(buffer.c_str(), nullptr, 16));
        }
    }

    if (isCurrentMask) {
        maskList.emplace_back(std::move(current));
        current.resize(0);
        isCurrentMask = false;
    } else {
        maskList.emplace_back(std::vector<unsigned char>(current.size(), 0xff));
        byteList.emplace_back(std::move(current));
        current.resize(0);
    }
}

int64_t Parser::findBytePattern(const std::string &pattern)
{
    std::vector<std::vector<unsigned char>> byteList;
    std::vector<std::vector<unsigned char>> maskList;

    parseBytePattern(pattern, byteList, maskList);

    size_t patternCount = byteList.size();
    size_t width = 0;
    for (const auto& bytes : byteList) {
        if (bytes.size() > width) {
            width = bytes.size();
        }
    }
    object().seekObjectEnd();
    size_t pos = object().pos();
    File& file = object().file();
    std::vector<unsigned char> buffer(width);
    for (size_t i = 0; file.good();++i) {
        file.read(reinterpret_cast<char *>(&buffer[i%width]), 8);
        for (size_t j = 0; j < patternCount; ++j) {
            const auto& bytes = byteList[j];
            const auto& masks = maskList[j];
            size_t patternSize = bytes.size();
            if (i >= patternSize - 1) {
                for (size_t k = 0; k < patternSize; ++k) {
                    if (bytes[k] != (masks[k] & buffer[(i - k) % width])) {
                        break;
                    }

                    if (k >= patternSize - 1) {
                        return pos + 8*(i - k);
                    }
                }
            }
        }
    }
    file.clear();
    return -1;
}

SimpleParser::SimpleParser(Object &object) : Parser(object)
{
}

void SimpleParser::parseHead()
{
    if (!_headParsed)
    {
        try {
            Object::ParsingContext context(object());
            doParseHead();
            setHeadParsed();
            _parsed = true;
            _tailParsed = true;
        } catch (const ParsingException& exception) {
            handleParsingException(exception);
        } catch (const Object::ParsingContext::LockException&) {
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
