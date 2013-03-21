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

#ifndef FLOATPARSER_H
#define FLOATPARSER_H

#include "parser.h"

class SingleFloatParser : public SimpleParser
{
public:
    SingleFloatParser(Object& object, bool bigEndian);
protected:
    void doParseHead();
private:
    bool bigEndian;
};

class DoubleFloatParser : public SimpleParser
{
public:
    DoubleFloatParser(Object& object, bool bigEndian);
protected:
    void doParseHead();
private:
    bool bigEndian;
};

class FixedFloat16Parser : public SimpleParser
{
public:
    FixedFloat16Parser(Object& object);
protected:
    void doParseHead();
};

class FixedFloat32Parser : public SimpleParser
{
public:
    FixedFloat32Parser(Object& object);
protected:
    void doParseHead();
};

#endif // FLOATPARSER_H
