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


WordParser::WordParser(Object &object, int numberOfChars)
    : SimpleParser(object), numberOfChars(numberOfChars)
{
}

void WordParser::doParseHead()
{
    object().setSize(numberOfChars * 8 );
    char* _word = new char[numberOfChars+1];
    object().file().read(_word, object().size());
    _word[numberOfChars] = '\0';
    object().setValue(std::string(_word));
    delete(_word);
}


Utf8StringParser::Utf8StringParser(Object &object)
    : SimpleParser(object)
{
}

void Utf8StringParser::doParseHead()
{
    std::stringstream S;
    std::streamoff stringLength = 0;
    while(object().file().good())
    {
        char ch;
        object().file().read(&ch, 8);
        ++stringLength;
        if(ch == '\0')
        {
            break;
        }
        char mask = 1 << 7;
        if((ch & mask) == 0)
        {
            S<<ch;
        }
        else
        {
            char testChar = 1;
            //skipping extended characters but still
            //check if no \0, to avoid infinite loop on faulty
            //data
            for(mask>>=1; ch & mask; mask>>=1)
            {
                object().file().seekg(8,std::ios::cur);
                object().file().read(&testChar, 8);
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
    object().setSize(8*stringLength);
}


WideStringParser::WideStringParser(Object &object, int numberOfChars)
    : SimpleParser(object), numberOfChars(numberOfChars)
{
}

void WideStringParser::doParseHead()
{
    object().setSize(numberOfChars * 16);
    std::string word(numberOfChars, '?');

    for(int i = 0; i < numberOfChars; ++i)
    {
        uint16_t ch = 0;
        if(object().endianness() == Object::bigEndian) {
            object().file().read(reinterpret_cast<char*>(&ch)+1, 8);
            object().file().read(reinterpret_cast<char*>(&ch), 8);
        } else {
            object().file().read(reinterpret_cast<char*>(&ch), 16);
        }

        if(ch == 0) {
            word.resize(i);
            break;
        }
        if(ch < 0x80) {
            word[i] = ch;
        }
    }

    object().setValue(word);
}
