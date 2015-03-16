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

#include "core/module.h"

#define parserLambda (const ObjectType &type, Object &object, const Module &module) ->Parser*
#define fixedSizeLambda (const ObjectType &type, const Module &module) ->int64_t
#define functionLambda (const Variable& scope, const Module &module) ->Variable

/*!
 * @brief Practical \link Module module\endlink implementation object using maps to generate parsers and functions
 */
class MapModule : public Module
{
protected:
    typedef std::function<Parser* (const ObjectType &, Object &, const Module &)> ParserGenerator;
    typedef std::function<int64_t (const ObjectType &, const Module &)> FixedSizeGenerator;
    typedef std::function<Variable (const Variable&, const Module &)> Functor;

    virtual ~MapModule(){}

    /**
     * @brief Define a parser generator for the \link ObjectTypeTemplate type template\endlink with this name
     */
    void addParser(const std::string& name, const ParserGenerator& parserGenerator);

    /**
     * @brief Define a null parser generator for the \link ObjectTypeTemplate type template\endlink with this name
     */
    void addParser(const std::string& name);

    /**
     * @brief Define a function to compute the fixed size for the \link ObjectType types\endlink
     * of the \link ObjectTypeTemplate type template\endlink with this name
     */
    void setFixedSize(const std::string& name, const FixedSizeGenerator& fixedSizeFunction);

    /**
     * @brief Define a fixed size for all \link ObjectType types\endlink
     * of the \link ObjectTypeTemplate type template\endlink with this name
     */
    void setFixedSize(const std::string& name, int64_t fixedSize);

    /**
     * @brief Link size for all \link ObjectType types\endlink
     * of the \link ObjectTypeTemplate type template\endlink with this name
     * with one of the arguments (
     *
     * For instance the size of an integer is given
     * by the first argument : int(16), int(32), int(64)...
     */
    void setFixedSizeFromArg(const std::string& name, int arg);

    /**
     * @brief Define a function signature and define a \link Functor functor\endlink to execute the function
     */
    void addFunction(const std::string& name,
                     const std::vector<std::string>& parameterNames,
                     const std::vector<bool>& parameterModifiables,
                     const std::vector<Variant>& parameterDefaults,
                     const Functor& functor);

private:
    bool hasParser(const ObjectType &type) const override;
    Parser* getParser(const ObjectType &type, Object &object, const Module &fromModule) const override;

    int64_t doGetFixedSize(const ObjectType &type, const Module &module) const override;

    bool doCanHandleFunction(const std::string& name) const override;
    Variable doExecuteFunction(const std::string& name, const Variable &params, const Module &fromModule) const override;
    const std::vector<std::string>& doGetFunctionParameterNames(const std::string& name) const override;
    const std::vector<bool>& doGetFunctionParameterModifiables(const std::string& name) const override;
    const std::vector<Variant>& doGetFunctionParameterDefaults(const std::string& name) const override;

    static const ParserGenerator& nullGenerator;
    std::unordered_map<std::string, ParserGenerator> _map;
    std::unordered_map<std::string, FixedSizeGenerator> _sizes;

    typedef std::tuple<std::vector<std::string>, std::vector<bool>, std::vector<Variant>, Functor> FunctionDescriptor;
    std::unordered_map<std::string,  FunctionDescriptor>  _functions;
};

#endif // MAPMODULE_H
