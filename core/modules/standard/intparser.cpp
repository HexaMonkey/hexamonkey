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

#include "core/modules/standard/intparser.h"

Int8Parser::Int8Parser(Object &object, Variant::Display display) : SimpleParser(object), display(display)
{
}

void Int8Parser::doParseHead()
{
    setSize(8);
    int8_t integer;
    file().read(reinterpret_cast<char* >(&integer), 8);

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}


Int16Parser::Int16Parser(Object &object, bool bigEndian, Variant::Display display)
    : SimpleParser(object), bigEndian(bigEndian), display(display)
{
}

void Int16Parser::doParseHead()
{
    setSize(16);
    int16_t integer;
    if(bigEndian)
    {
        file().read(reinterpret_cast<char* >(&integer)+1, 8);
        file().read(reinterpret_cast<char* >(&integer), 8);
    } else {
        file().read(reinterpret_cast<char* >(&integer), 16);
    }

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}


Int32Parser::Int32Parser(Object &object, bool bigEndian, Variant::Display display)
    : SimpleParser(object), bigEndian(bigEndian), display(display)
{
}

void Int32Parser::doParseHead()
{
    setSize(32);
    int32_t integer;
    file().read(reinterpret_cast<char* >(&integer), 32);
    if(bigEndian) {
        integer = __builtin_bswap32(integer);
    }

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}


Int64Parser::Int64Parser(Object &object, bool bigEndian, Variant::Display display)
    : SimpleParser(object), bigEndian(bigEndian), display(display)
{
}

void Int64Parser::doParseHead()
{
    setSize(64);
    int64_t integer;
    file().read(reinterpret_cast<char* >(&integer), 64);
    if(bigEndian)
        integer = __builtin_bswap64(integer);

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}


IntXParser::IntXParser(Object &object, uint8_t size, bool bigEndian, Variant::Display display)
    : SimpleParser(object),
      size(size),
      bigEndian(bigEndian),
      display(display)
{
}

void IntXParser::doParseHead()
{
    setSize(size);
    int byteSize = (size % 8 == 0) ? size/8 : size/8 + 1;
    char buffer[byteSize];
    int64_t integer = 0;
    char* pInteger = reinterpret_cast<char* >(&integer);
    file().read(buffer, size);
    for (int i = 0 ; i < byteSize ; i++) {
        if(bigEndian)
            pInteger[byteSize - 1 - i] = buffer[i];
        else
            pInteger[i] = buffer[i];
    }

    if (integer & 1LL<<(size-1) ) {
        integer |= 0xFFFFFFFFFFFFFFFFLL << size;
    }

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}


UInt8Parser::UInt8Parser(Object &object, Variant::Display display) : SimpleParser(object), display(display)
{
}

void UInt8Parser::doParseHead()
{
    setSize(8);
    uint8_t integer;
    file().read(reinterpret_cast<char* >(&integer), 8);

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}

UInt16Parser::UInt16Parser(Object &object, bool bigEndian, Variant::Display display)
    : SimpleParser(object), bigEndian(bigEndian), display(display)
{
}

void UInt16Parser::doParseHead()
{
    setSize(16);
    uint16_t integer;
    if (bigEndian) {
        file().read(reinterpret_cast<char* >(&integer)+1, 8);
        file().read(reinterpret_cast<char* >(&integer), 8);
    } else {
        file().read(reinterpret_cast<char* >(&integer), 16);
    }

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}


UInt32Parser::UInt32Parser(Object &object, bool bigEndian, Variant::Display display)
    : SimpleParser(object), bigEndian(bigEndian), display(display)
{
}

void UInt32Parser::doParseHead()
{
    setSize(32);
    uint32_t integer;
    file().read(reinterpret_cast<char* >(&integer), 32);
    if (bigEndian) {
        integer = __builtin_bswap32(integer);
    }

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}


UInt64Parser::UInt64Parser(Object &object, bool bigEndian, Variant::Display display)
    : SimpleParser(object), bigEndian(bigEndian), display(display)
{
}

void UInt64Parser::doParseHead()
{
    setSize(64);
    uint64_t integer;
    file().read(reinterpret_cast<char* >(&integer), 64);
    if(bigEndian) {
        integer = __builtin_bswap64(integer);
    }

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}


UIntXParser::UIntXParser(Object &object, uint8_t size, bool bigEndian, Variant::Display display)
    : SimpleParser(object),
      size(size),
      bigEndian(bigEndian),
      display(display)
{
}

void UIntXParser::doParseHead()
{
    setSize(size);
    int byteSize = ((size & 7) == 0) ? size/8 : size/8 + 1;
    char buffer[byteSize];
    uint64_t integer = 0;
    char* pInteger = reinterpret_cast<char* >(&integer);
    file().read(buffer, size);
    for (int i = 0 ; i < byteSize ; ++i) {
        if(bigEndian) {
            pInteger[byteSize -1 - i] = buffer[i];
        } else {
            pInteger[i] = buffer[i];
        }
    }

    Variant value(integer);
    value.setDisplayType(display);
    setValue(value);
}
