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

#include "variable.h"
#include "scope.h"

#include "strutil.h"
#include "bitutil.h"

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

    addFunction("sizeof",
                {"type"},
                {false},
                {},
                []functionLambda
    {
        const ObjectType& type = scope.cget(0)->toObjectType();
        int64_t size = module.getFixedSize(type);
        if(size != -1)
            return Variable::copy(size);
        else
            return Variable::null();

    });

    addFunction("int",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const Variant& value = *scope.cget(0);
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
        return Variable::null();
    });

    addFunction("float",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const Variant& value = *scope.cget(0);
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
        return Variable::null();
    });

    addFunction("str",
                {"value", "base", "size"},
                {false, false, false},
                {Variant(), 10, 0},
                []functionLambda
    {
        std::stringstream S;
        S<<std::setbase(scope.cget(1)->toInteger())<<std::setw(scope.cget(2)->toInteger())<<std::setfill('0')<<*scope.cget(0);
        const std::string& s = S.str();
        return Variable::copy(s);
    });

    addFunction("fromAscii",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const char ch = scope.cget(0)->toInteger();
        const std::string s(1, ch);
        return Variable::copy(s);
    });

    addFunction("toAscii",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const std::string& str = scope.cget(0)->toString();
        if(str.empty())
            return Variable::null();
        else
            return Variable::copy(str[0]);
    });

    addFunction("uppercase",
                {"string"},
                {false},
                {},
                []functionLambda
    {
        std::string str = toStr(*scope.cget(0));
        std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        return Variable::copy(str);
    });

    addFunction("lowercase",
                {"string"},
                {false},
                {},
                []functionLambda
    {
        std::string str = toStr(*scope.cget(0));
        std::transform(str.begin(), str.end(),str.begin(), ::tolower);
        return Variable::copy(str);
    });

    addFunction("popCount",
                {"word"},
                {false},
                {},
                []functionLambda
    {
        uint64_t word = scope.cget(0)->toUnsignedInteger();
        return Variable::copy(popCount(word));
    });

    addFunction("formatDate",
                {"date"},
                {false},
                {},
                []functionLambda
    {
        uint64_t secs = scope.cget(0)->toUnsignedInteger();
        return Variable::copy(formatDate(secs));
    });

    addFunction("formatDuration",
                {"duration"},
                {false},
                {},
                []functionLambda
    {
        uint64_t secs = scope.cget(0)->toUnsignedInteger();
        return Variable::copy(formatDuration(secs));
    });

    addFunction("substr",
                {"string", "start", "size"},
                {false, false, false},
                {},
                []functionLambda
    {

        if(   scope.cget(0)->canConvertTo(Variant::string)
           && scope.cget(1)->canConvertTo(Variant::integer))
        {
            const std::string& str    = scope.cget(0)->toString();

            int64_t start;
            int64_t size;
            if(scope.cget(2)->canConvertTo(Variant::integer))
            {
                start = scope.cget(1)->toInteger();
                size = scope.cget(2)->toInteger();
            }
            else
            {
                start = 0;
                size = scope.cget(1)->toInteger();
            }

            if(start < str.size())
            {
                if(start+size < str.size())
                    return Variable::copy(str.substr(start, size));
                else
                    return Variable::copy(str.substr(start));
            }
        }

        return Variable::null();
    });

    return true;
}
