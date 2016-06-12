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

#include <unordered_set>

#include "core/modules/default/defaultmodule.h"

#include "core/object.h"

#include "core/variable/variable.h"
#include "core/variable/variablecollector.h"
#include "core/util/strutil.h"
#include "core/util/bitutil.h"

#include "core/modules/default/filetypetemplate.h"
#include "core/modules/default/integertypetemplate.h"
#include "core/modules/default/arraytypetemplate.h"
#include "core/modules/default/tupletypetemplate.h"
#include "core/modules/default/datatypetemplate.h"
#include "core/modules/default/floattypetemplate.h"
#include "core/modules/default/stringtypetemplate.h"
#include "core/modules/default/enumtypetemplate.h"
#include "core/modules/default/structtypetemplate.h"

std::unordered_set<std::string> refactored = {"int", "uint", "byte", "uuid", "File", "Array", "Tuple", "Data", "float", "double", "fixedFloat", "String", "WString", "Enum", "Struct", "Bitset"};

bool DefaultModule::doLoad()
{
    addTemplate(new FileTypeTemplate);

    addTemplate(new ArrayTypeTemplate);

    addTemplate(new TupleTypeTemplate);

    addTemplate(new DataTypeTemplate);

    addTemplate(new StructTypeTemplate);

    addTemplate(new EnumTypeTemplate);

    addTemplate(new IntegerTypeTemplate);

    addTemplate(new UIntegerTypeTemplate);

    addTemplate(new ByteTypeTemplate);

    addTemplate(new BitsetTypeTemplate);

    addTemplate(new UuidTypeTemplate);

    addTemplate(new FloatTypeTemplate);

    addTemplate(new DoubleTypeTemplate);

    addTemplate(new FixedFloatTypeTemplate);

    addTemplate(new StringTypeTemplate);

    addTemplate(new WStringTypeTemplate);


    addFunction("sizeof",
                {"type"},
                {false},
                {},
                []functionLambda
    {
        const ObjectType& type = scope.field(0).value().toObjectType();
        int64_t size = type.fixedSize();
        if(size != -1)
            return scope.collector().copy(size);
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
            return scope.collector().copy(newValue);
        }
        else if(value.canConvertTo(Variant::stringType))
        {
            std::stringstream S;
            S<<value.toString();
            int64_t i;
            if(!(S>>i).fail()) {
                Variant newValue = i;
                newValue.setDisplayBase(base);
                return scope.collector().copy(newValue);
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
            return scope.collector().copy(newValue);
        }
        else if(value.canConvertTo(Variant::stringType))
        {
            std::stringstream S;
            S<<value.toString();
            double d;
            if(!(S>>d).fail()) {
                Variant newValue = d;
                newValue.setDisplayBase(base);
                return scope.collector().copy(newValue);
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
        return scope.collector().copy(s);
    });

    addFunction("fromAscii",
                {"value"},
                {false},
                {},
                []functionLambda
    {
        const char ch = scope.field(0).value().toInteger();
        const std::string s(1, ch);
        return scope.collector().copy(s);
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
            return scope.collector().copy(str[0]);
    });

    addFunction("uppercase",
                {"string"},
                {false},
                {},
                []functionLambda
    {
        std::string str = toStr(scope.field(0).value());
        std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        return scope.collector().copy(str);
    });

    addFunction("lowercase",
                {"string"},
                {false},
                {},
                []functionLambda
    {
        std::string str = toStr(scope.field(0).value());
        std::transform(str.begin(), str.end(),str.begin(), ::tolower);
        return scope.collector().copy(str);
    });

    addFunction("popCount",
                {"word"},
                {false},
                {},
                []functionLambda
    {
        uint64_t word = scope.field(0).value().toUnsignedInteger();
        return scope.collector().copy(popCount(word));
    });

    addFunction("formatDate",
                {"date"},
                {false},
                {},
                []functionLambda
    {
        uint64_t secs = scope.field(0).value().toUnsignedInteger();
        return scope.collector().copy(formatDate(secs));
    });

    addFunction("formatDuration",
                {"duration"},
                {false},
                {},
                []functionLambda
    {
        uint64_t secs = scope.field(0).value().toUnsignedInteger();
        return scope.collector().copy(formatDuration(secs));
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
                    return scope.collector().copy(str.substr(start, size));
                else
                    return scope.collector().copy(str.substr(start));
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
