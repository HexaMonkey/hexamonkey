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

#ifndef MP4SIMPLEPARSER_H
#define MP4SIMPLEPARSER_H

#include "parser.h"

class Mp4DateParser : public SimpleParser
{
public:
    Mp4DateParser(Object& object);
protected:
    void doParseHead();
};

class Mp4DurationParser : public SimpleParser
{
public:
    Mp4DurationParser(Object& object, int64_t scale);
protected:
    void doParseHead();
private:
    int64_t scale;
};

class Mp4LanguageParser : public SimpleParser
{
public:
    Mp4LanguageParser(Object& object);
protected:
    void doParseHead();
};
#endif // MP4SIMPLEPARSER_H
