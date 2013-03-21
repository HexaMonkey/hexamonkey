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

#include "tsjunkparser.h"
#include "standardtypes.h"

using namespace standardTypes;

TsJunkParser::TsJunkParser(Object &object, const Module &module)
    :ContainerParser(object, module)
{
    setAutogrow();
}

void TsJunkParser::doParseHead()
{
    while(true)
    {
        Object* syncbyte = getVariable(uint8);

        if(syncbyte->value().toUnsignedInteger() == 0x47)
        {
            syncbyte->seekBeginning();
            break;
        }
        else
        {
            std::cout<<syncbyte->value().toUnsignedInteger()<<std::endl;
            addChild(syncbyte);
            syncbyte->seekEnd();
        }
    }
}
