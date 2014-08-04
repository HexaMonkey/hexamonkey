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
#include "core/interpreter/variable.h"
#include "core/interpreter/scope/scope.h"
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

    addTemplate(data);
    addParser("Data", []parserLambda
    {
        if(type.parameterSpecified(0))
            return new DataParser(object, type.parameterValue(0).toInteger());
        else
            return new DataParser(object, -1);
    });
    setFixedSizeFromArg("Data", 0);

    addFunction("sizeof",
                {"type"},
                {false},
                {},
                []functionLambda
    {
        const ObjectType& type = scope.get(0).cvalue().toObjectType();
        int64_t size = module.getFixedSize(type);
        if(size != -1)
            return Variable::copy(size);
        else
            return Variable();

    });

    addFunction("int",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const Variant& value = scope.get(0).cvalue();
        if(value.canConvertTo(Variant::integer))
        {
            return Variable::copy(value.toInteger());
        }
        else if(value.canConvertTo(Variant::string))
        {
            std::stringstream S;
            S<<value.toString();
            int64_t i;
            if(!(S>>i).fail())
                return Variable::copy(i);
        }
        return Variable();
    });

    addFunction("float",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const Variant& value = scope.get(0).cvalue();
        if(value.canConvertTo(Variant::floating))
        {
            return Variable::copy(value.toDouble());
        }
        else if(value.canConvertTo(Variant::string))
        {
            std::stringstream S;
            S<<value.toString();
            double d;
            if(!(S>>d).fail())
                return Variable::copy(d);
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
        S<<std::setbase(scope.get(1).cvalue().toInteger())<<std::setw(scope.get(2).cvalue().toInteger())<<std::setfill('0')<<scope.get(0).cvalue();
        const std::string& s = S.str();
        return Variable::copy(s);
    });

    addFunction("fromAscii",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const char ch = scope.get(0).cvalue().toInteger();
        const std::string s(1, ch);
        return Variable::copy(s);
    });

    addFunction("toAscii",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const std::string& str = scope.get(0).cvalue().toString();
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
        std::string str = toStr(scope.get(0).cvalue());
        std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        return Variable::copy(str);
    });

    addFunction("lowercase",
                {"string"},
                {false},
                {},
                []functionLambda
    {
        std::string str = toStr(scope.get(0).cvalue());
        std::transform(str.begin(), str.end(),str.begin(), ::tolower);
        return Variable::copy(str);
    });

    addFunction("popCount",
                {"word"},
                {false},
                {},
                []functionLambda
    {
        uint64_t word = scope.get(0).cvalue().toUnsignedInteger();
        return Variable::copy(popCount(word));
    });

    addFunction("formatDate",
                {"date"},
                {false},
                {},
                []functionLambda
    {
        uint64_t secs = scope.get(0).cvalue().toUnsignedInteger();
        return Variable::copy(formatDate(secs));
    });

    addFunction("formatDuration",
                {"duration"},
                {false},
                {},
                []functionLambda
    {
        uint64_t secs = scope.get(0).cvalue().toUnsignedInteger();
        return Variable::copy(formatDuration(secs));
    });

    addFunction("substr",
                {"string", "start", "size"},
                {false, false, false},
                {},
                []functionLambda
    {

        if(   scope.get(0).cvalue().canConvertTo(Variant::string)
           && scope.get(1).cvalue().canConvertTo(Variant::unsignedInteger))
        {
            const std::string& str    = scope.get(0).cvalue().toString();

            uint64_t start;
            uint64_t size;
            if(scope.get(2).cvalue().canConvertTo(Variant::unsignedInteger))
            {
                start = scope.get(1).cvalue().toUnsignedInteger();
                size = scope.get(2).cvalue().toUnsignedInteger();
            }
            else
            {
                start = 0;
                size = scope.get(1).cvalue().toUnsignedInteger();
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
        ErrorManager::getInstance()->notify("[LOG]"+scope.get(0).cvalue().toString());
        return Variable();
    });

    addFunction("error",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        ErrorManager::getInstance()->notify("[ERROR]"+scope.get(0).cvalue().toString());
        return Variable();
    });

    addFunction("warning",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        ErrorManager::getInstance()->notify("[WARNING]"+scope.get(0).cvalue().toString());
        return Variable();
    });

    return true;
}
