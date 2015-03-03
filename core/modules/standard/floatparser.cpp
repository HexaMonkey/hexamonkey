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

#include "core/modules/standard/floatparser.h"

SingleFloatParser::SingleFloatParser(Object &object, bool bigEndian)
    : SimpleParser(object), bigEndian(bigEndian)
{
}

void SingleFloatParser::doParseHead()
{
    object().setSize(32);
    union {int32_t i; float f;} val;
    object().file().read(reinterpret_cast<char* >(&val.i), 32);
    if(bigEndian)
        val.i = __builtin_bswap32(val.i);

    object().setValue(val.f);
}


DoubleFloatParser::DoubleFloatParser(Object &object, bool bigEndian)
    : SimpleParser(object), bigEndian(bigEndian)
{
}

void DoubleFloatParser::doParseHead()
{
    object().setSize(64);
    union {int64_t i; double f;} val;
    object().file().read(reinterpret_cast<char* >(&val.i), 64);
    if(bigEndian)
        val.i = __builtin_bswap64(val.i);

    object().setValue(val.f);
}


FixedFloat16Parser::FixedFloat16Parser(Object &object) : SimpleParser(object)
{
}

void FixedFloat16Parser::doParseHead()
{
    object().setSize(16);

    int8_t integer;
    object().file().read(reinterpret_cast<char* >(&integer), 8);

    uint8_t decimal;
    object().file().read(reinterpret_cast<char* >(&decimal), 8);

    double f = integer + decimal/pow(2,8);
    object().setValue(f);
}


FixedFloat32Parser::FixedFloat32Parser(Object &object) : SimpleParser(object)
{
}

void FixedFloat32Parser::doParseHead()
{
    object().setSize(32);

    int16_t integer;
    object().file().read(reinterpret_cast<char* >(&integer)+1, 8);
    object().file().read(reinterpret_cast<char* >(&integer), 8);

    uint16_t decimal;
    object().file().read(reinterpret_cast<char* >(&decimal)+1, 8);
    object().file().read(reinterpret_cast<char* >(&decimal), 8);

    double f = integer + decimal/pow(2,16);
    object().setValue(f);
}
