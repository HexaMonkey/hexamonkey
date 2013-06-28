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

#include "fromfilemodule.h"
#include "model.h"
#include "typescope.h"
#include "localscope.h"
#include "fromfileparser.h"

#ifdef USE_QT
    #include <QFileInfo>
    #include <QDateTime>
#else
    #include "fileutil.h"
#endif

#include "unused.h"

#include "interpreter.h"
#include "variabledescriptor.h"
#include "variable.h"
#include "blockexecution.h"

const VariableDescriptor sizeDescriptor = {"@size"};
const std::vector<VariableDescriptor> headerOnlyVars = {sizeDescriptor,{"@args"},{"@value"}, {"@info"}};

const std::vector<std::string> emptyParameterNames;
const std::vector<bool> emptyParameterModifiables;
const std::vector<Variant> emptyParameterDefaults;

FromFileModule::FromFileModule(Program program)
    :_program(program)
{
    UNUSED(hmcElemNames);
}

void FromFileModule::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    if(program().isValid())
    {
        Program formatDetections = program().elem(0);
        loadFormatDetections(formatDetections, formatAdder);
    }
}

void FromFileModule::requestImportations(std::vector<std::string> &formatRequested)
{
    if(program().isValid())
    {
        Program imports = program().elem(1);
        loadImports(imports, formatRequested);
    }
}

bool FromFileModule::doLoad()
{
    if(!program().isValid())
        return false;

    Program classDeclarations = program().elem(2);
    nameScan(classDeclarations);
    loadExtensions(classDeclarations);
    loadSpecifications(classDeclarations);
    return true;
}

Parser *FromFileModule::getParser(const ObjectType &type, Object &object, const Module &fromModule) const
{
    const std::string name = type.typeTemplate().name();
    auto it = _definitions.find(name);
    if(it == _definitions.end())
        return nullptr;

    const Program& definition = it->second;

    if(definition.size() == 0)
        return nullptr;

    return new FromFileParser(object, fromModule, definition, headerEnd(name));
}

int64_t FromFileModule::doGetFixedSize(const ObjectType &type, const Module &module) const
{
    const std::string& name = type.typeTemplate().name();
    auto alreadyIt = _fixedSizes.find(name);
    if(alreadyIt != _fixedSizes.end())
        return alreadyIt->second;

    auto definitionIt = _definitions.find(name);
    if(definitionIt == _definitions.end())
        return -1;

    Program definition = definitionIt->second;
    std::set<VariableDescriptor> descriptors;
    definition.buildDependencies(true, descriptors);
    if(sizeDependency(name))
    {
        std::cout<<type.typeTemplate().name()<<" unknown size"<<std::endl;
        _fixedSizes[type.typeTemplate().name()] = -1;
        return -1;
    }

    if(module.getFather(type).isNull())
    {
        int64_t size = definition.guessSize(module);
        if(size>0)
        {
            std::cout<<type.typeTemplate().name()<<" guessed size "<<size<<std::endl;
            _fixedSizes[type.typeTemplate().name()] = size;
            return size;
        }
    }

    std::cout<<type.typeTemplate().name()<<" father's size"<<std::endl;
    _fixedSizes[type.typeTemplate().name()] = 0;
    return 0;
}

bool FromFileModule::doCanHandleFunction(const std::string &name) const
{
    return _functions.find(name) != _functions.end();
}

Variable *FromFileModule::doExecuteFunction(const std::string &name, Scope &params, const Module &fromModule) const
{
    auto it = functionDescriptor(name);

    if(it == _functionDescriptors.end())
        return nullptr;

    Holder holder(program());
    CompositeScope scope;
    LocalScope localScope(holder);

    scope.addScope(localScope);
    scope.addScope(params);

    const Program& definition = std::get<3>(it->second);
    BlockExecution blockExecution(definition, fromModule, scope, nullptr);

    blockExecution.execute();

    return &holder.extract(blockExecution.extractReturnValue());
}

const std::vector<std::string> &FromFileModule::doGetFunctionParameterNames(const std::string &name) const
{
    auto it = functionDescriptor(name);

    if(it == _functionDescriptors.end())
        return emptyParameterNames;

    return std::get<0>(it->second);
}

const std::vector<bool> &FromFileModule::doGetFunctionParameterModifiables(const std::string &name) const
{
    auto it = functionDescriptor(name);

    if(it == _functionDescriptors.end())
        return emptyParameterModifiables;

    return std::get<1>(it->second);
}

const std::vector<Variant> &FromFileModule::doGetFunctionParameterDefaults(const std::string &name) const
{
    auto it = functionDescriptor(name);

    if(it == _functionDescriptors.end())
        return emptyParameterDefaults;

    return std::get<2>(it->second);
}

void FromFileModule::loadFormatDetections(Program &formatDetections, StandardFormatDetector::Adder &formatAdder)
{
    for(Program formatDetection: formatDetections)
    {
        switch(formatDetection.elem(0).payload().toInteger())
        {
            case ADD_MAGIC_NUMBER_OP:
                formatAdder.addMagicNumber(formatDetection.elem(1).payload().toString());
                break;

            case ADD_EXTENSION_OP:
                formatAdder.addExtension(formatDetection.elem(1).payload().toString());
                break;

            case ADD_SYNCBYTE_OP:
                formatAdder.addSyncbyte(formatDetection.elem(0).payload().toInteger(), formatDetection.elem(1).payload().toInteger());
                break;

            default:
                break;
        }
    }
}

void FromFileModule::loadImports(Program &imports, std::vector<std::string> &formatRequested)
{
    for(Program module: imports)
    {
        formatRequested.push_back(module.payload().toString());
    }
}

void FromFileModule::nameScan(Program& declarations)
{
    std::cout<<"Load templates :"<<std::endl;


    for(Program declaration : declarations)
    {
        if(declaration.id() == CLASS_DECLARATION)
        {
            Program typeTemplate = declaration.elem(0).elem(0);
            const std::string& name = typeTemplate.elem(0).payload().toString();
            std::vector<std::string> parameters;
            for(Program arg:typeTemplate.elem(1))
            {
                parameters.push_back(arg.payload().toString());
            }
            const ObjectTypeTemplate& temp = newTemplate(name, parameters);
            std::cout<<"    "<<temp<<std::endl;

            Program classDefinition = declaration.elem(1);
            _definitions[name] = classDefinition;
        }
        else if(declaration.id() == FUNCTION_DECLARATION)
        {
            const std::string& name = declaration.elem(0).payload().toString();
            _functions[name] = declaration;
        }
    }

    std::cout<<std::endl;
}

void FromFileModule::loadExtensions(Program &classDeclarations)
{
    std::cout<<"Load extensions :"<<std::endl;

    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.id() == CLASS_DECLARATION)
        {
            const std::string& name = classDeclaration.elem(0).elem(0).elem(0).payload().toString();
            const ObjectTypeTemplate& childTemplate = getTemplate(name);

            //Choose extension program
            //By defaut "extension"
            Program extension = classDeclaration.elem(0).elem(1);
            //If there is no "extension" then "specification" is taken
            if(extension.size() == 0)
            {
                extension = classDeclaration.elem(0).elem(2);
                if(extension.size() == 0)
                    continue;
            }

           //Adding extension lambda
           Program program = extension.elem(0);
           setExtension(childTemplate,[this, program](const ObjectType& type)
           {
               return program.evaluateType(ConstTypeScope(type), *this);
           });

           std::cout<<"    "<<childTemplate<<" extends "<<program.elem(0).payload()<<"(...)"<<std::endl;
        }
    }

    std::cout<<std::endl;
}

void FromFileModule::loadSpecifications(Program &classDeclarations)
{
    std::cout<<"Load specifications :"<<std::endl;

    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.id() == CLASS_DECLARATION)
        {
            ObjectType child = getTemplate(classDeclaration.elem(0).elem(0).elem(0).payload().toString())();
            for(Program type : classDeclaration.elem(0).elem(2))
            {
                ObjectType parent(type.evaluateType(EmptyScope(), *this));
                setSpecification(parent, child);
                std::cout<<"    "<<child<<" specifies "<<parent<<std::endl;
            }
        }
        else if(classDeclaration.id() == FORWARD)
        {
            ObjectType parent(classDeclaration.elem(0).evaluateType(EmptyScope(), *this));
            ObjectType child(classDeclaration.elem(1).evaluateType(EmptyScope(), *this));
            setSpecification(parent, child);
            std::cout<<"    "<<child<<" specifies "<<parent<<std::endl;
        }
    }

    std::cout<<std::endl;
}

bool FromFileModule::sizeDependency(const std::string &name) const
{
    auto alreadyIt = _sizeDependency.find(name);
    if(alreadyIt != _sizeDependency.end())
        return alreadyIt->second;

    auto definitionIt = _definitions.find(name);
    if(definitionIt == _definitions.end())
        return -1;

    Program definition = definitionIt->second;
    std::set<VariableDescriptor> descriptors;
    definition.buildDependencies(true, descriptors);
    bool result = descriptors.find(sizeDescriptor) != descriptors.end();
    _sizeDependency[name] = result;
    return result;
}

Program::const_iterator FromFileModule::headerEnd(const std::string &name) const
{
    auto alreadyIt = _headerEnd.find(name);
    if(alreadyIt != _headerEnd.end())
        return alreadyIt->second;

    Program definition = _definitions.find(name)->second;
    Program::const_reverse_iterator reverseHeaderEnd = definition.rbegin();

    for(;reverseHeaderEnd != definition.rend(); ++reverseHeaderEnd)
    {
        const Program& line = *reverseHeaderEnd;

        //Check dependencies
        std::set<VariableDescriptor> variableSet;
        line.buildDependencies(true, variableSet);
        if(std::any_of(headerOnlyVars.begin(), headerOnlyVars.end(), [&variableSet](const VariableDescriptor& headerOnlyVar) -> bool
        {
            auto find = variableSet.find(headerOnlyVar);
            if(find != variableSet.end())
                return true;

            auto upper = variableSet.upper_bound(headerOnlyVar);
            if(upper == variableSet.end())
                return false;

            if((*upper).inScopeOf(headerOnlyVar))
                return true;

            return false;

        }))
                break;

        //Check showcased declarations
        if(line.id() == DECLARATION)
        {
            if(line.elem(2).payload().toInteger())
                break;
        }
    }

    Program::const_iterator headerEnd = definition.begin();
    std::advance(headerEnd, std::distance(reverseHeaderEnd, definition.rend()));
    _headerEnd[name] = headerEnd;
    return headerEnd;
}

FromFileModule::FunctionDescriptorMap::iterator FromFileModule::functionDescriptor(const std::string &name) const
{
    auto alreadyIt = _functionDescriptors.find(name);
    if(alreadyIt != _functionDescriptors.end())
        return alreadyIt;

    auto it = _functions.find(name);
    if(it == _functions.end())
        return _functionDescriptors.end();

    const Program& declaration = it->second;
    const Program& arguments = declaration.elem(1);
    const Program& definition = declaration.elem(2);

    std::vector<std::string> parameterNames;
    std::vector<bool> parameterModifiables;
    std::vector<Variant> parameterDefaults;

    for(const Program& argument: arguments)
    {
        Holder holder(argument.elem(2));
        parameterNames.push_back(argument.elem(1).payload().toString());
        parameterModifiables.push_back(argument.elem(0).payload().toBool());
        parameterDefaults.push_back(holder.cevaluate(EmptyScope(), *this));
    }

    auto functionDescriptor = std::forward_as_tuple(parameterNames, parameterModifiables, parameterDefaults, definition);
    return _functionDescriptors.insert(std::make_pair(name, functionDescriptor)).first;
}

const Program &FromFileModule::program() const
{
    return _program;
}


bool FromFileModule::hasParser(const ObjectType &type) const
{
    return _definitions.find(type.typeTemplate().name()) != _definitions.end();
}
