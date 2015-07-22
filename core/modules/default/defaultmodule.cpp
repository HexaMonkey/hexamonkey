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
#include "core/modules/default/defaulttypes.h"
#include "core/modules/default/fileparser.h"
#include "core/modules/default/arrayparser.h"
#include "core/modules/default/tupleparser.h"
#include "core/modules/default/dataparser.h"
#include "core/modules/default/structparser.h"
#include "core/variable/variable.h"
#include "core/util/strutil.h"
#include "core/util/bitutil.h"

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
