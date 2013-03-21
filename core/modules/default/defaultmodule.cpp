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

#include "defaultmodule.h"

#include "defaulttypes.h"

#include "fileparser.h"
#include "arrayparser.h"
#include "tupleparser.h"

using namespace defaultTypes;

bool DefaultModule::doLoad()
{
    addTemplate(file);
    addParser("File", []parserLambda{return new FileParser(object);});

    addTemplate(array);
    addParser("Array", []parserLambda
    {
        if(type.parameterSpecified(0))
        {
            if(type.parameterSpecified(1))
                return new ArrayParser(object, module, type.parameterValue(0).toObjectType(), type.parameterValue(1).toInteger());
            else
                return new ArrayParser(object, module, type.parameterValue(0).toObjectType(), -1);
        }
        return nullptr;
    });
    setFixedSize("Array", [this]fixedSizeLambda
    {
         if(type.parameterSpecified(1))
         {
             int64_t s = type.parameterValue(1).toInteger();
             if(s>0)
                return s;
         }
         return -1;
    });

    addTemplate(tuple);
    addParser("Tuple", []parserLambda
    {
        if(type.parameterSpecified(0)  && type.parameterSpecified(1))
        {
            return new TupleParser(object, module, type.parameterValue(0).toObjectType(), type.parameterValue(1).toInteger());
        }
        return nullptr;
    });
    setFixedSize("Tuple", [this]fixedSizeLambda
    {
         if(type.parameterSpecified(0) && type.parameterSpecified(1))
         {
             int64_t t = module.getFixedSize(type.parameterValue(0).toObjectType());
             if(t>0)
                return t*type.parameterValue(1).toInteger();
         }
         return -1;
    });

    return true;
}
