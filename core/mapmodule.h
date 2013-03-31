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

#ifndef MAPMODULE_H
#define MAPMODULE_H

#include <unordered_map>
#include <string>
#include <functional>

#include "module.h"

#define parserLambda (const ObjectType &type, Object &object, const Module &module) ->Parser*
#define fixedSizeLambda (const ObjectType &type, const Module &module) ->int64_t
#define functionLambda (const Scope& scope, const Module &module) ->Variable*

/*!
 * \brief The MapModule class
 */
class MapModule : public Module
{
protected:
    typedef std::function<Parser* (const ObjectType &, Object &, const Module &)> ParserGenerator;
    typedef std::function<int64_t (const ObjectType &, const Module &)> FixedSizeGenerator;
    typedef std::function<Variable* (const Scope&, const Module &)> FunctionGenerator;

    virtual ~MapModule(){}

    void addParser(const std::string& name, const ParserGenerator& parserGenerator);
    void addParser(const std::string& name);

    void setFixedSize(const std::string& name, const FixedSizeGenerator& fixedSizeFunction);
    void setFixedSize(const std::string& name, int64_t fixedSize);
    void setFixedSizeFromArg(const std::string& name, int arg);

    void addFunction(const std::string& name,
                     const std::vector<std::string>& parameterNames,
                     const std::vector<bool>& parameterModifiables,
                     const std::vector<Variant>& parameterDefaults,
                     const FunctionGenerator& function);

    virtual bool hasParser(const ObjectType &type) const override;
    virtual Parser* getParser(const ObjectType &type, Object &object, const Module &fromModule) const override;

    virtual int64_t doGetFixedSize(const ObjectType &type, const Module &module) const override;

    bool doCanHandleFunction(const std::string& name) const override;
    Variable* doExecuteFunction(const std::string& name, Scope &params, const Module &fromModule) const override;
    const std::vector<std::string>& doGetFunctionParameterNames(const std::string& name) const override;
    const std::vector<bool>& doGetFunctionParameterModifiables(const std::string& name) const override;
    const std::vector<Variant>& doGetFunctionParameterDefaults(const std::string& name) const override;

private:
    static const ParserGenerator& nullGenerator;
    std::unordered_map<std::string, ParserGenerator> _map;
    std::unordered_map<std::string, FixedSizeGenerator> _sizes;

    typedef std::tuple<std::vector<std::string>, std::vector<bool>, std::vector<Variant>, FunctionGenerator> FunctionDescriptor;
    std::unordered_map<std::string,  FunctionDescriptor>  _functions;
};

#endif // MAPMODULE_H
