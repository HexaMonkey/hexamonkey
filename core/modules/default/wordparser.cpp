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

#include "core/modules/default/wordparser.h"
#include "core/log/logmanager.h"

Utf8StringParser::Utf8StringParser(Object &object, int numberOfChars)
    : SimpleParser(object), numberOfChars(numberOfChars)
{
}

void Utf8StringParser::doParseHead()
{
    std::stringstream S;
    std::streamoff stringLength = 0;
    while((numberOfChars == -1 || stringLength < numberOfChars) && object().file().good())
    {
        char ch;
        object().file().read(&ch, 8);

        ++stringLength;
        if(ch == '\0')
        {
            break;
        }
        unsigned char mask = 1 << 7;
        if((ch & mask) == 0)
        {
            S<<ch;
        }
        else
        {
            S <<'?';
            unsigned char& toCount = reinterpret_cast<unsigned char&>(ch);
            char testChar = 1;
            //skipping extended characters but still
            //check if no \0, to avoid infinite loop on faulty
            //data
            for(mask>>=1; toCount & mask; mask>>=1)
            {
                object().file().read(&testChar, 8);
                ++stringLength;
                if (testChar == '\0') {
                    break;
                }
            }

            if (testChar == '\0') {
                break;
            }
        }
    }
    object().setValue(S.str());
    if (numberOfChars == -1) {
        object().setSize(8 * stringLength);
    } else {
        object().setSize(8 * numberOfChars);
    }
}


WideStringParser::WideStringParser(Object &object, int numberOfChars)
    : SimpleParser(object), numberOfChars(numberOfChars)
{
}

void WideStringParser::doParseHead()
{
    std::stringstream S;
    std::streamoff stringLength = 0;

    for(int i = 0; numberOfChars == -1 || i < numberOfChars; ++i)
    {
        uint16_t ch = 0;
        if(object().endianness() == Object::bigEndian) {
            object().file().read(reinterpret_cast<char*>(&ch)+1, 8);
            object().file().read(reinterpret_cast<char*>(&ch), 8);
        } else {
            object().file().read(reinterpret_cast<char*>(&ch), 16);
        }
        ++stringLength;

        if(ch == 0) {
            break;
        }

        if(ch < 0x80) {
            S << (char)ch;
        } else {
            S << '?';
        }
    }

    if (numberOfChars == -1) {
        object().setSize(16 * stringLength);
    }  else {
        object().setSize(16 * numberOfChars);
    }

    object().setValue(S.str());
}
