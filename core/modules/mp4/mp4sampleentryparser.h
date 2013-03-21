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

#ifndef MP4SAMPLEENTRYPARSER_H
#define MP4SAMPLEENTRYPARSER_H

#include "containerparser.h"

class Mp4SampleEntryParser : public ContainerParser
{
public:
    Mp4SampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4AudioSampleEntryParser : public ContainerParser
{
public:
    Mp4AudioSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4VisualSampleEntryParser : public ContainerParser
{
public:
    Mp4VisualSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4XMLMetaSampleEntryParser : public ContainerParser
{
public:
    Mp4XMLMetaSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TextMetaSampleEntryParser : public ContainerParser
{
public:
    Mp4TextMetaSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4HintSampleEntryParser : public ContainerParser
{
public:
    Mp4HintSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

#endif // MP4SAMPLEENTRYPARSER_H
