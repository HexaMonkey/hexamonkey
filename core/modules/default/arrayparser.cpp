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

#include "core/modules/default/arrayparser.h"
#include "core/module.h"
#include "core/object.h"

ArrayParser::ArrayParser(ParsingOption &option, const ObjectType &elementType, int64_t size, const std::string &namePattern)
    :ElementaryContainerParser(option, elementType, namePattern), size(size)
{
    object().setToExpandOnAddition();
}

void ArrayParser::doParseHead()
{
    if (size != -1) {
        object().setSize(size);
    }
}

void ArrayParser::doParse()
{
    while(availableSize())
    {
        addElem();
    }
}

bool ArrayParser::doParseSome(int hint)
{
    for(int count = 0; count < hint; ++count)
    {
        if(availableSize()<=0)
            return true;

        addElem();
    }
    return false;
}
