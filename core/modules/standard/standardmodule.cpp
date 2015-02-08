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

#include "core/modules/standard/standardmodule.h"
#include "core/modules/standard/standardtypes.h"
#include "core/modules/standard/intparser.h"
#include "core/modules/standard/bitparser.h"
#include "core/modules/standard/wordparser.h"
#include "core/modules/standard/floatparser.h"

using namespace standardTypes;

StandardModule::StandardModule(bool bigEndian)
    :bigEndian(bigEndian)
{
}

bool StandardModule::doLoad()
{
    addTemplate(integer);
    addParser("int", [this]parserLambda
    {
        if (type.parameterSpecified(0)) {
            int size = type.parameterValue(0).toInteger();
            int base = 0;
            if (type.parameterSpecified(1)) {
                base = type.parameterValue(1).toInteger();
            }
            Variant::Display display = Variant::decimal;
            switch (base) {
                case 2:
                    display = Variant::binary;
                    break;

                case 8:
                    display = Variant::octal;
                    break;

                case 16:
                    display = Variant::hexadecimal;
                    break;
            }

            if(size<=64)
            {
                switch(size)
                {
                    case 8:
                        return new Int8Parser(object, display);
                    case 16:
                        return new Int16Parser(object, bigEndian, display);
                    case 32:
                        return new Int32Parser(object, bigEndian, display);
                    case 64:
                        return new Int64Parser(object, bigEndian, display);
                    default:
                        return new IntXParser(object, size, bigEndian, display);
                }
            }
        }
        return nullptr;

    });
    setFixedSizeFromArg("int", 0);

    addTemplate(uinteger);
    addParser("uint", [this]parserLambda
    {
        if (type.parameterSpecified(0)) {
            int size = type.parameterValue(0).toInteger();
            int base = 0;
            if (type.parameterSpecified(1)) {
                base = type.parameterValue(1).toInteger();
            }
            Variant::Display display = Variant::decimal;
            switch (base) {
                case 2:
                    display = Variant::binary;
                    break;

                case 8:
                    display = Variant::octal;
                    break;

                case 16:
                    display = Variant::hexadecimal;
                    break;
            }

            if(size<=64)
            {
                switch(size)
                {
                    case 8:
                        return new UInt8Parser(object, display);
                    case 16:
                        return new UInt16Parser(object, bigEndian, display);
                    case 32:
                        return new UInt32Parser(object, bigEndian, display);
                    case 64:
                        return new UInt64Parser(object, bigEndian, display);
                    default:
                        return new UIntXParser(object, size, bigEndian, display);
                }
            }
        }
        return nullptr;
    });
    setFixedSizeFromArg("uint", 0);

    addTemplate(byte);
    addParser("byte", [this]parserLambda
    {
        return new UInt8Parser(object, Variant::hexadecimal);
    });
    setFixedSize("byte", 8);


    addTemplate(singleFloat);
    addParser("float", [this]parserLambda{return new SingleFloatParser(object, bigEndian);});
    setFixedSize("float", 32);

    addTemplate(doubleFloat);
    addParser("double", [this]parserLambda{return new DoubleFloatParser(object, bigEndian);});
    setFixedSize("double", 64);

    addTemplate(fixedFloat);
    addParser("fixedFloat", []parserLambda
    {
        if(type == fixed16)
            return new FixedFloat16Parser(object);
        if(type == fixed32)
            return new FixedFloat32Parser(object);
        return nullptr;
    });
    setFixedSize("fixedFloat", []fixedSizeLambda
    {
         if(type.parameterSpecified(0) && type.parameterSpecified(1))
             return type.parameterValue(0).toInteger()+type.parameterValue(1).toInteger();
         return -1;
    });

    addTemplate(string);
    addParser("String", []parserLambda
    {
        if(type.parameterSpecified(0))
            return new WordParser(object, type.parameterValue(0).toInteger());
        return new Utf8StringParser(object);
    });
    setFixedSize("String", []fixedSizeLambda
    {
         if(type.parameterSpecified(0))
             return 8*type.parameterValue(0).toInteger();
         return -1;
    });

    addTemplate(wstring);
    addParser("WString", [this]parserLambda
    {
        if(type.parameterSpecified(0))
            return new WideStringParser(object, type.parameterValue(0).toInteger(), bigEndian);
        return new Utf8StringParser(object);
    });
    setFixedSize("WString", []fixedSizeLambda
    {
         if(type.parameterSpecified(0))
             return 16*type.parameterValue(0).toInteger();
         return -1;
    });


    addTemplate(bitset);
    addParser("Bitset", []parserLambda
    {
        if(type.parameterSpecified(0))
            return new BitParser(object, type.parameterValue(0).toInteger());
        return nullptr;
    });
    setFixedSizeFromArg("Bitset", 0);

    return true;
}
