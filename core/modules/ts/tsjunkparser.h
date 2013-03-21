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

#ifndef TSJUNKPARSER_H
#define TSJUNKPARSER_H

#include "containerparser.h"

/*!
 * \brief The TsJunkParser class
 */
class TsJunkParser : public ContainerParser
{
public:
    TsJunkParser(Object& object, const Module& module);
protected:
    void doParseHead();
};

#endif // TSJUNKPARSER_H
