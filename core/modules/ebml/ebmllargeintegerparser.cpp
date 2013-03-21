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

#include "ebmllargeintegerparser.h"

EbmlLargeIntegerParser::EbmlLargeIntegerParser(Object &object)
    : SimpleParser(object)
{
}

void EbmlLargeIntegerParser::doParseHead()
{
    uint8_t byte;
    file().read(reinterpret_cast<char*>(&byte), 8);
    int count;
    for(count = 1; count <= 8; ++count)
    {
        if(byte&(1<<(8-count)))
            break;
    }
    int64_t var = byte & ~(1<<(8-count));

    for(int i = 1; i < count; ++i)
    {
        uint8_t byte;
        file().read(reinterpret_cast<char*>(&byte), 8);
        var = var<<8 | byte;
    }
    setSize(8*count);
    setValue(var);
    setInfo(toStr(var));
}
