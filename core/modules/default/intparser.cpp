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

#include "core/modules/default/intparser.h"

#include <sstream>

Int8Parser::Int8Parser(Object &object, Variant::Display display) : SimpleParser(object), display(display)
{
}

void Int8Parser::doParseHead()
{
    object().setSize(8);
    int8_t integer;
    object().file().read(reinterpret_cast<char* >(&integer), 8);

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}


Int16Parser::Int16Parser(Object &object, Variant::Display display)
    : SimpleParser(object), display(display)
{
}

void Int16Parser::doParseHead()
{
    object().setSize(16);
    int16_t integer;
    if(object().endianness() == Object::bigEndian)
    {
        object().file().read(reinterpret_cast<char* >(&integer)+1, 8);
        object().file().read(reinterpret_cast<char* >(&integer), 8);
    } else {
        object().file().read(reinterpret_cast<char* >(&integer), 16);
    }

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}


Int32Parser::Int32Parser(Object &object, Variant::Display display)
    : SimpleParser(object), display(display)
{
}

void Int32Parser::doParseHead()
{
    object().setSize(32);
    int32_t integer;
    object().file().read(reinterpret_cast<char* >(&integer), 32);
    if(object().endianness() == Object::bigEndian) {
        integer = __builtin_bswap32(integer);
    }

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}


Int64Parser::Int64Parser(Object &object, Variant::Display display)
    : SimpleParser(object), display(display)
{
}

void Int64Parser::doParseHead()
{
    object().setSize(64);
    int64_t integer;
    object().file().read(reinterpret_cast<char* >(&integer), 64);
    if(object().endianness() == Object::bigEndian)
        integer = __builtin_bswap64(integer);

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}


IntXParser::IntXParser(Object &object, uint8_t size, Variant::Display display)
    : SimpleParser(object),
      size(size),
      display(display)
{
}

void IntXParser::doParseHead()
{
    object().setSize(size);
    int byteSize = (size % 8 == 0) ? size/8 : size/8 + 1;
    char buffer[byteSize];
    int64_t integer = 0;
    char* pInteger = reinterpret_cast<char* >(&integer);
    object().file().read(buffer, size);
    for (int i = 0 ; i < byteSize ; i++) {
        if(object().endianness() == Object::bigEndian)
            pInteger[byteSize - 1 - i] = buffer[i];
        else
            pInteger[i] = buffer[i];
    }

    if (integer & 1LL<<(size-1) ) {
        integer |= 0xFFFFFFFFFFFFFFFFLL << size;
    }

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}


UInt8Parser::UInt8Parser(Object &object, Variant::Display display) : SimpleParser(object), display(display)
{
}

void UInt8Parser::doParseHead()
{
    object().setSize(8);
    uint8_t integer;
    object().file().read(reinterpret_cast<char* >(&integer), 8);

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}

UInt16Parser::UInt16Parser(Object &object, Variant::Display display)
    : SimpleParser(object), display(display)
{
}

void UInt16Parser::doParseHead()
{
    object().setSize(16);
    uint16_t integer;
    if (object().endianness() == Object::bigEndian) {
        object().file().read(reinterpret_cast<char* >(&integer)+1, 8);
        object().file().read(reinterpret_cast<char* >(&integer), 8);
    } else {
        object().file().read(reinterpret_cast<char* >(&integer), 16);
    }

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}


UInt32Parser::UInt32Parser(Object &object, Variant::Display display)
    : SimpleParser(object), display(display)
{
}

void UInt32Parser::doParseHead()
{
    object().setSize(32);
    uint32_t integer;
    object().file().read(reinterpret_cast<char* >(&integer), 32);
    if (object().endianness() == Object::bigEndian) {
        integer = __builtin_bswap32(integer);
    }

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}


UInt64Parser::UInt64Parser(Object &object, Variant::Display display)
    : SimpleParser(object), display(display)
{
}

void UInt64Parser::doParseHead()
{
    object().setSize(64);
    uint64_t integer;
    object().file().read(reinterpret_cast<char* >(&integer), 64);
    if(object().endianness() == Object::bigEndian) {
        integer = __builtin_bswap64(integer);
    }

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}


UIntXParser::UIntXParser(Object &object, uint8_t size, Variant::Display display)
    : SimpleParser(object),
      size(size),
      display(display)
{
}

void UIntXParser::doParseHead()
{
    object().setSize(size);
    int byteSize = ((size & 7) == 0) ? size/8 : size/8 + 1;
    char buffer[byteSize];
    uint64_t integer = 0;
    char* pInteger = reinterpret_cast<char* >(&integer);
    object().file().read(buffer, size);
    for (int i = 0 ; i < byteSize ; ++i) {
        if(object().endianness() == Object::bigEndian) {
            pInteger[byteSize -1 - i] = buffer[i];
        } else {
            pInteger[i] = buffer[i];
        }
    }

    Variant value(integer);
    value.setDisplayType(display);
    object().setValue(value);
}


UuidParser::UuidParser(Object &object) : SimpleParser(object)
{
}

void UuidParser::doParseHead()
{
    object().setSize(128);

    std::stringstream S;

    int32_t i0;
    int16_t i1;
    int16_t i2;
    int64_t i3;

    if(object().endianness() == Object::bigEndian) {
        object().file().read(reinterpret_cast<char* >(&i0), 32);
        i0 = __builtin_bswap32(i0);

        object().file().read(reinterpret_cast<char* >(&i1)+1, 8);
        object().file().read(reinterpret_cast<char* >(&i1), 8);

        object().file().read(reinterpret_cast<char* >(&i2)+1, 8);
        object().file().read(reinterpret_cast<char* >(&i2), 8);
    } else {
        object().file().read(reinterpret_cast<char* >(&i0), 32);
        object().file().read(reinterpret_cast<char* >(&i1), 16);
        object().file().read(reinterpret_cast<char* >(&i2), 16);
    }
    //always big endian
    object().file().read(reinterpret_cast<char* >(&i3), 64);
    i3 = __builtin_bswap64(i3);

    S<<std::hex<<std::setfill('0')<<std::uppercase
     <<std::setw(8)<<i0<<"-"
     <<std::setw(4)<<i1<<"-"
     <<std::setw(4)<<i2<<"-"
     <<std::setw(4)<<((i3>>48)&0xffff)<<"-"
     <<std::setw(12)<<(i3&0xffffffffffffLL);

    object().setValue(S.str());
}
