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

#include <bitset>

#include "core/modules/standard/bitparser.h"

#include "core/log/logmanager.h"

BitParser::BitParser(Object &object, int64_t size)
    : SimpleParser(object),
      size(size)
{
}

void BitParser::doParseHead()
{
    if(size>64)
    {
        Log::warning("Size to large for bitset");
    }
    object().setSize(size);

    std::bitset<64> flag;
    unsigned char bit;
    for(unsigned int i = 0; i < size; i+=1)
    {
        object().file().read(reinterpret_cast<char*>(&bit), 1);
        flag <<= 1;
        flag |= bit;
    }

    Variant value(flag.to_ullong());
    value.setDisplayType(Variant::binary);
    object().setValue(value);
}


ContentParser::ContentParser(Object &object, int64_t size)
    : SimpleParser(object),
      size(size)
{
    object.setToExpandOnAddition();
}

void ContentParser::doParseHead()
{
    object().setSize(size);
}
