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

#ifndef INTPARSER_H
#define INTPARSER_H

#include "core/parser.h"

class Int8Parser : public SimpleParser
{
public:
    Int8Parser(Object& object, int base = 10);
protected:
    void doParseHead();
	int base;
};

class Int16Parser : public SimpleParser
{
public:
    Int16Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class Int32Parser : public SimpleParser
{
public:
    Int32Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class Int64Parser : public SimpleParser
{
public:
    Int64Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class IntXParser : public SimpleParser
{
public:
    IntXParser(Object& object, uint8_t size, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    uint8_t size;
    bool bigEndian;
    int base;
};

class UInt8Parser : public SimpleParser
{
public:
    UInt8Parser(Object& object, int base = 10);
protected:
    void doParseHead();
	int base;
};

class UInt16Parser : public SimpleParser
{
public:
    UInt16Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class UInt32Parser : public SimpleParser
{
public:
    UInt32Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class UInt64Parser : public SimpleParser
{
public:
    UInt64Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class UIntXParser : public SimpleParser
{
public:
    UIntXParser(Object& object, uint8_t size, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    uint8_t size;
    bool bigEndian;
    int base;
};

#endif // INTPARSER_H
