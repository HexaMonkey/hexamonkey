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

#ifndef FROMFILEMODULE_H
#define FROMFILEMODULE_H

#include <memory>

#include "core/mapmodule.h"
#include "core/interpreter/program.h"
#include "core/interpreter/evaluator.h"

/**
 * @brief Module implementation created from an HMDL file
 *
 * The module generates instances of FromFileParser as parsers.
 */
class FromFileModule : public Module
{
public:
    /**
     * @param program given by the \link ProgramLoader program loader\endlink.
     */
    FromFileModule(Program program);

private:
    virtual void addFormatDetection(StandardFormatDetector::Adder& formatAdder) final;
    virtual void requestImportations(std::vector<std::string>& formatRequested) final;
    virtual bool doLoad() final;

    virtual Parser* getParser(const ObjectType &type, Object& object, const Module& fromModule) const final;
    virtual bool hasParser(const ObjectType &type) const final;
    virtual int64_t doGetFixedSize(const ObjectType &type, const Module &module) const final;

    virtual bool doCanHandleFunction(const std::string& name) const final;
    virtual Variable doExecuteFunction(const std::string& name, const Variable &params, const Module &fromModule) const final;
    virtual const std::vector<std::string>& doGetFunctionParameterNames(const std::string& name) const final;
    virtual const std::vector<bool>& doGetFunctionParameterModifiables(const std::string& name) const final;
    virtual const std::vector<Variant>& doGetFunctionParameterDefaults(const std::string& name) const final;

    typedef std::tuple<std::vector<std::string>, std::vector<bool>, std::vector<Variant>, Program> FunctionDescriptor;
    typedef std::unordered_map<std::string, FunctionDescriptor> FunctionDescriptorMap;
    bool loadProgram(const std::string path);

    void loadFormatDetections(Program& formatDetections, StandardFormatDetector::Adder& formatAdder);
    void loadImports(Program& imports, std::vector<std::string>& formatRequested);

    void nameScan(Program &classDeclarations);
    void loadExtensions(Program &classDeclarations);
    void loadSpecifications(Program &classDeclarations);
    bool sizeDependency(const std::string& name) const;

    int64_t guessSize(const Program& instructions) const;

    std::set<VariablePath> variableDependencies(const Program& instructions, bool modificationOnly) const;
    void buildDependencies(const Program& instructions, bool modificationOnly, std::set<VariablePath>& descriptors, bool areVariablesModified = false) const;

    bool checkHeaderOnlyVar(const Program& line) const;
    Program::const_iterator headerEnd(const std::string& name) const;
    bool needTailParsing(const std::string& name) const;
    FunctionDescriptorMap::iterator functionDescriptor(const std::string& name) const;

    const Program& program() const;

    Program _program;

    std::unordered_map<std::string, Program> _definitions;
    std::unordered_map<std::string, Program> _functions;
    mutable FunctionDescriptorMap _functionDescriptors;

    mutable std::unordered_map<std::string, int64_t> _fixedSizes;
    mutable std::unordered_map<std::string, bool> _sizeDependency;
    mutable std::unordered_map<std::string, Program::const_iterator> _headerEnd;
    mutable std::unordered_map<std::string, bool> _needTailParsing;

    mutable VariableCollector _collector;
    Variable _scope;
    Evaluator _evaluator;

};

#endif // FROMFILEMODULE_H
