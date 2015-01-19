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

#include "core/mapmodule.h"
#include "core/objecttypetemplate.h"

const std::vector<std::string> emptyParameterNames;
const std::vector<bool> emptyParameterModifiables;
const std::vector<Variant> emptyParameterDefaults;

const MapModule::ParserGenerator& MapModule::nullGenerator = []parserLambda{return nullptr;};

void MapModule::addParser(const std::string &name, const MapModule::ParserGenerator &parserGenerator)
{
    _map[name] = parserGenerator;
}

void MapModule::addParser(const std::string &name)
{
    addParser(name, nullGenerator);
    setFixedSize(name, 0);
}

void MapModule::setFixedSize(const std::string &name, const FixedSizeGenerator &fixedSizeFunction)
{
    _sizes[name] = fixedSizeFunction;
}

void MapModule::setFixedSize(const std::string &name, int64_t fixedSize)
{
    _sizes[name] = [fixedSize] fixedSizeLambda {return fixedSize;};
}

void MapModule::setFixedSizeFromArg(const std::string &name, int arg)
{
    _sizes[name] = [arg]fixedSizeLambda
        {
             if(type.parameterSpecified(arg))
                 return type.parameterValue(arg).toInteger();
             return -1;
};
}

void MapModule::addFunction(const std::string &name,
                            const std::vector<std::string> &parameterNames,
                            const std::vector<bool> &parameterModifiables,
                            const std::vector<Variant> &parameterDefaults,
                            const MapModule::Functor &functor)
{
    _functions[name] = std::make_tuple(parameterNames, parameterModifiables, parameterDefaults, functor);
}

bool MapModule::hasParser(const ObjectType &type) const
{
    return _map.find(type.typeTemplate().name()) != _map.end();
}

Parser *MapModule::getParser(const ObjectType &type, Object &object, const Module &fromModule) const
{
    auto it = _map.find(type.typeTemplate().name());
    if(it != _map.end())
    {
        return (it->second)(type, object, fromModule);
    }
    else
    {
        return nullptr;
    }
}

int64_t MapModule::doGetFixedSize(const ObjectType &type, const Module &module) const
{
    auto it = _sizes.find(type.typeTemplate().name());
    if(it != _sizes.end())
    {
        return (it->second)(type, module);
    }
    else
    {
        return -1;
    }
}

bool MapModule::doCanHandleFunction(const std::string &name) const
{
    return _functions.find(name) != _functions.end();
}

Variable MapModule::doExecuteFunction(const std::string &name, Scope &params, const Module &fromModule) const
{
    auto it = _functions.find(name);

    if(it == _functions.end())
        return Variable();

    const Functor& function = std::get<3>(it->second);

    return function(params, fromModule);
}


const std::vector<std::string> &MapModule::doGetFunctionParameterNames(const std::string &name) const
{
    auto it = _functions.find(name);

    if(it == _functions.end())
        return emptyParameterNames;

    const std::vector<std::string>& parameterNames = std::get<0>(it->second);
    return parameterNames;
}

const std::vector<bool> &MapModule::doGetFunctionParameterModifiables(const std::string &name) const
{
    auto it = _functions.find(name);

    if(it == _functions.end())
        return emptyParameterModifiables;

    const std::vector<bool>& parameterNames = std::get<1>(it->second);
    return parameterNames;
}

const std::vector<Variant> &MapModule::doGetFunctionParameterDefaults(const std::string &name) const
{
    auto it = _functions.find(name);

    if(it == _functions.end())
        return emptyParameterDefaults;

    const std::vector<Variant>& parameterDefaults = std::get<2>(it->second);
    return parameterDefaults;
}
