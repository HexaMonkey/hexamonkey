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

#include "core/modules/default/defaultmodule.h"
#include "core/modules/default/fileparser.h"
#include "core/modules/default/arrayparser.h"
#include "core/modules/default/tupleparser.h"
#include "core/modules/default/dataparser.h"
#include "core/modules/default/structparser.h"
#include "core/modules/default/intparser.h"
#include "core/modules/default/bitparser.h"
#include "core/modules/default/wordparser.h"
#include "core/modules/default/floatparser.h"
#include "core/modules/default/enumparser.h"

#include "core/variable/variable.h"
#include "core/util/strutil.h"
#include "core/util/bitutil.h"

const ObjectTypeTemplate DefaultModule::file("File", std::vector<std::string>(), [](ObjectTypeTemplate& typeTemplate) {
    typeTemplate.setVirtual(true);
});

const ObjectTypeTemplate DefaultModule::array("Array",{"elementType", "size", "_namePattern"}, [](ObjectTypeTemplate& typeTemplate) {
    typeTemplate.setElementTypeGenerator([]objectTypeAttributeLambda {
                                             return type.parameterValue(0);
                                         });
});

const ObjectTypeTemplate DefaultModule::tuple("Tuple",{"elementType", "count", "_namePattern"}, [](ObjectTypeTemplate& typeTemplate) {
    typeTemplate.setElementTypeGenerator([]objectTypeAttributeLambda {
                                             return type.parameterValue(0);
                                         });

    typeTemplate.setElementCountGenerator([]objectTypeAttributeLambda {
                                             return type.parameterValue(1);
                                          });
});
const ObjectTypeTemplate DefaultModule::data("Data", {"_size"});

const ObjectTypeTemplate DefaultModule::structType("Struct", {"_name"}, [](ObjectTypeTemplate& typeTemplate) {
    typeTemplate.setNameGenerator([]objectTypeAttributeLambda {
                                             return type.parameterValue(0);
                                         });
});

const ObjectTypeTemplate DefaultModule::enumType("Enum", {"type"});

const ObjectTypeTemplate DefaultModule::integer("int",{"size", "_base"});
const ObjectTypeTemplate DefaultModule::uinteger("uint",{"size", "_base"});
const ObjectTypeTemplate DefaultModule::byte("byte");

const ObjectType DefaultModule::int8(DefaultModule::integer(8));
const ObjectType DefaultModule::int16(DefaultModule::integer(16));
const ObjectType DefaultModule::int32(DefaultModule::integer(32));
const ObjectType DefaultModule::int64(DefaultModule::integer(64));

const ObjectType DefaultModule::uint8(DefaultModule::uinteger(8));
const ObjectType DefaultModule::uint16(DefaultModule::uinteger(16));
const ObjectType DefaultModule::uint32(DefaultModule::uinteger(32));
const ObjectType DefaultModule::uint64(DefaultModule::uinteger(64));

const ObjectTypeTemplate DefaultModule::uuid("uuid");

const ObjectTypeTemplate DefaultModule::fixedFloat("fixedFloat", {"integer","decimal"});
const ObjectTypeTemplate DefaultModule::singleFloat("float");
const ObjectTypeTemplate DefaultModule::doubleFloat("double");

const ObjectTypeTemplate DefaultModule::string("String", {"charCount"});
const ObjectTypeTemplate DefaultModule::wstring("WString", {"charCount"});

const ObjectTypeTemplate DefaultModule::bitset("Bitset", {"size"});


bool DefaultModule::doLoad()
{
    addTemplate(file);
    addParser("File", []parserLambda{return new FileParser(object);});

    addTemplate(array);
    addParser("Array", []parserLambda
    {
        if(type.parameterSpecified(0))
        {
            const ObjectType& elemType = type.parameterValue(0).toObjectType();
            const int64_t size =
                      type.parameterSpecified(1) ? type.parameterValue(1).toInteger() : -1LL;

            const std::string& namePattern =
                      type.parameterSpecified(2) ? type.parameterValue(2).toString() : "";

            return new ArrayParser(object, module, elemType, size, namePattern);
        }
        return nullptr;
    });
    setFixedSize("Array", [this]fixedSizeLambda
    {
         if(type.parameterSpecified(1))
         {
             int64_t s = type.parameterValue(1).toInteger();
             if(s>=0)
                return s;
         }
         return -1;
    });

    addTemplate(tuple);
    addParser("Tuple", []parserLambda
    {
        if(type.parameterSpecified(0) && type.parameterSpecified(1))
        {
            const ObjectType& elemType = type.parameterValue(0).toObjectType();
            const int64_t elemCount = type.parameterValue(1).toInteger();
            const std::string& namePattern =
                       type.parameterSpecified(2) ? type.parameterValue(2).toString() : "";
            return new TupleParser(object, module, elemType, elemCount, namePattern);
        }
        return nullptr;
    });
    setFixedSize("Tuple", [this]fixedSizeLambda
    {
         if(type.parameterSpecified(0) && type.parameterSpecified(1))
         {
             int64_t t = module.getFixedSize(type.parameterValue(0).toObjectType());
             if(t>=0)
                return t*type.parameterValue(1).toInteger();
         }
         return -1;
    });

    addTemplate(data);
    addParser("Data", []parserLambda
    {
        if(type.parameterSpecified(0))
            return new DataParser(object, type.parameterValue(0).toInteger());
        else
            return new DataParser(object, -1);
    });
    setFixedSizeFromArg("Data", 0);

    addTemplate(structType);
    addParser("Struct", []parserLambda
    {
        auto parser = new StructParser(object, module);
        for (int i = 0, n = (type.numberOfParameters()-1)/2; i < n; ++i) {
            parser->addElement(type.parameterValue(2*i+1).toObjectType(), type.parameterValue(2*i+2).toString());
        }
        return parser;
    });
    setFixedSize("Struct", [this]fixedSizeLambda
    {
         int s = 0;
         for (int i = 1; i < type.numberOfParameters(); i += 2) {
             int t = module.getFixedSize(type.parameterValue(i).toObjectType());
             if (t != -1) {
                 s += t;
             } else {
                 return -1;
             }
         }
         return s;
    });

    addTemplate(enumType);
    addParser("Enum", []parserLambda
    {
        if (type.parameterSpecified(0)) {
            auto parser = new EnumParser(object, module, type.parameterValue(0).toObjectType());
            for (int i = 0, n = (type.numberOfParameters()-1)/2; i < n; ++i) {
                parser->addElement(type.parameterValue(2*i+1), type.parameterValue(2*i+2));
            }

            return parser;
        } else {
            return nullptr;
        }
    });
    setFixedSize("Enum", [this]fixedSizeLambda
    {
         if (type.parameterSpecified(0)) {
             const ObjectType& childType = type.parameterValue(0).toObjectType();
             const int64_t t = module.getFixedSize(childType);
             return t;
         } else {
             return -1;
         }
    });

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
                        return new Int16Parser(object, display);
                    case 32:
                        return new Int32Parser(object, display);
                    case 64:
                        return new Int64Parser(object, display);
                    default:
                        return new IntXParser(object, size, display);
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
                        return new UInt16Parser(object, display);
                    case 32:
                        return new UInt32Parser(object, display);
                    case 64:
                        return new UInt64Parser(object, display);
                    default:
                        return new UIntXParser(object, size, display);
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

    addTemplate(uuid);
    addParser("uuid", [this]parserLambda
    {
        return new UuidParser(object);
    });
    setFixedSize("uuid", 128);

    addTemplate(singleFloat);
    addParser("float", [this]parserLambda{return new SingleFloatParser(object);});
    setFixedSize("float", 32);

    addTemplate(doubleFloat);
    addParser("double", [this]parserLambda{return new DoubleFloatParser(object);});
    setFixedSize("double", 64);

    addTemplate(fixedFloat);
    addParser("fixedFloat", []parserLambda
    {
        if(type.parameterSpecified(0) && type.parameterSpecified(1)) {
                      int64_t decimal = type.parameterValue(0).toInteger();
                      int64_t fractionnal = type.parameterValue(1).toInteger();
                      if (decimal == fractionnal) {
                          if (decimal == 16) {
                              return new FixedFloat16Parser(object);
                          } else if (decimal == 32) {
                              return new FixedFloat32Parser(object);
                          }
                      }
        }
        return nullptr;
    });
    setFixedSize("fixedFloat", []fixedSizeLambda
    {
         if(type.parameterSpecified(0) && type.parameterSpecified(1))
             return type.parameterValue(0).toInteger()+type.parameterValue(1).toInteger();
         return HM_UNKNOWN_SIZE;
    });

    addTemplate(string);
    addParser("String", []parserLambda
    {
        int numberOfChars;
        if(type.parameterSpecified(0)) {
            numberOfChars = (int) type.parameterValue(0).toInteger();
        } else {
            numberOfChars = -1;
        }

        return new Utf8StringParser(object, numberOfChars);
    });
    setFixedSize("String", []fixedSizeLambda
    {
         if(type.parameterSpecified(0))
             return 8*type.parameterValue(0).toInteger();
         return HM_UNKNOWN_SIZE;
    });

    addTemplate(wstring);
    addParser("WString", [this]parserLambda
    {
        int64_t size;
        if(type.parameterSpecified(0)) {
            size = type.parameterValue(0).toInteger();
        } else {
            size = -1;
        }

        return new WideStringParser(object, size);
    });
    setFixedSize("WString", []fixedSizeLambda
    {
         if(type.parameterSpecified(0))
             return 16*type.parameterValue(0).toInteger();
         return HM_UNKNOWN_SIZE;
    });


    addTemplate(bitset);
    addParser("Bitset", []parserLambda
    {
        if(type.parameterSpecified(0))
            return new BitParser(object, type.parameterValue(0).toInteger());
        return nullptr;
    });
    setFixedSizeFromArg("Bitset", 0);


    addFunction("sizeof",
                {"type"},
                {false},
                {},
                []functionLambda
    {
        const ObjectType& type = scope.field(0).value().toObjectType();
        int64_t size = module.getFixedSize(type);
        if(size != -1)
            return Variable::copy(size);
        else
            return Variable();

    });

    addFunction("int",
                {"value", "base"},
                {false, false},
                {Variant(), 10},
                []functionLambda
    {
        const Variant& value = scope.field(0).value();
        int base = (int) scope.field(1).value().toInteger();
        if(value.canConvertTo(Variant::integerType))
        {
            Variant newValue = value.toInteger();
            newValue.setDisplayBase(base);
            return Variable::copy(newValue);
        }
        else if(value.canConvertTo(Variant::stringType))
        {
            std::stringstream S;
            S<<value.toString();
            int64_t i;
            if(!(S>>i).fail()) {
                Variant newValue = i;
                newValue.setDisplayBase(base);
                return Variable::copy(newValue);
            }
        }
        return Variable();
    });

    addFunction("float",
                {"value", "base"},
                {false},
                {Variant(), 10},
                []functionLambda
    {
        const Variant& value = scope.field(0).value();
        int base = (int) scope.field(1).value().toInteger();
        if(value.canConvertTo(Variant::floatingType))
        {
            Variant newValue = value.toDouble();
            newValue.setDisplayBase(base);
            return Variable::copy(newValue);
        }
        else if(value.canConvertTo(Variant::stringType))
        {
            std::stringstream S;
            S<<value.toString();
            double d;
            if(!(S>>d).fail()) {
                Variant newValue = d;
                newValue.setDisplayBase(base);
                return Variable::copy(newValue);
            }
        }
        return Variable();
    });

    addFunction("str",
                {"value", "base", "size"},
                {false, false, false},
                {Variant(), 10, 0},
                []functionLambda
    {
        std::stringstream S;
        S<<std::setbase(scope.field(1).value().toInteger())<<std::setw(scope.field(2).value().toInteger())<<std::setfill('0');
        scope.field(0).value().display(S, false);
        const std::string& s = S.str();
        return Variable::copy(s);
    });

    addFunction("fromAscii",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const char ch = scope.field(0).value().toInteger();
        const std::string s(1, ch);
        return Variable::copy(s);
    });

    addFunction("toAscii",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const std::string& str = scope.field(0).value().toString();
        if(str.empty())
            return Variable();
        else
            return Variable::copy(str[0]);
    });

    addFunction("uppercase",
                {"string"},
                {false},
                {},
                []functionLambda
    {
        std::string str = toStr(scope.field(0).value());
        std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        return Variable::copy(str);
    });

    addFunction("lowercase",
                {"string"},
                {false},
                {},
                []functionLambda
    {
        std::string str = toStr(scope.field(0).value());
        std::transform(str.begin(), str.end(),str.begin(), ::tolower);
        return Variable::copy(str);
    });

    addFunction("popCount",
                {"word"},
                {false},
                {},
                []functionLambda
    {
        uint64_t word = scope.field(0).value().toUnsignedInteger();
        return Variable::copy(popCount(word));
    });

    addFunction("formatDate",
                {"date"},
                {false},
                {},
                []functionLambda
    {
        uint64_t secs = scope.field(0).value().toUnsignedInteger();
        return Variable::copy(formatDate(secs));
    });

    addFunction("formatDuration",
                {"duration"},
                {false},
                {},
                []functionLambda
    {
        uint64_t secs = scope.field(0).value().toUnsignedInteger();
        return Variable::copy(formatDuration(secs));
    });

    addFunction("substr",
                {"string", "start", "size"},
                {false, false, false},
                {},
                []functionLambda
    {

        if(   scope.field(0).value().canConvertTo(Variant::stringType)
           && scope.field(1).value().canConvertTo(Variant::unsignedIntegerType))
        {
            const std::string str = scope.field(0).value().toString();

            uint64_t start;
            uint64_t size;
            if(scope.field(2).value().canConvertTo(Variant::unsignedIntegerType)) {
                start = scope.field(1).value().toUnsignedInteger();
                size = scope.field(2).value().toUnsignedInteger();
            } else {
                start = 0;
                size = scope.field(1).value().toUnsignedInteger();
            }

            if(start < str.size())
            {
                if(start+size < str.size())
                    return Variable::copy(str.substr(start, size));
                else
                    return Variable::copy(str.substr(start));
            }
        }

        return Variable();
    });

    addFunction("log",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        Log::info(scope.field(0).value().toString());
        return Variable();
    });

    addFunction("error",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        Log::error(scope.field(0).value().toString());
        return Variable();
    });

    addFunction("warning",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        Log::warning(scope.field(0).value().toString());
        return Variable();
    });

    return true;
}
