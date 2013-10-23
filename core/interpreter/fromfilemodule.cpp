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

#include "programloader.h"
#include "variablepath.h"
#include "variable.h"
#include "blockexecution.h"

const VariablePath sizeDescriptor = {"@size"};
const std::vector<VariablePath> headerOnlyVars = {sizeDescriptor,{"@args"},{"@value"}, {"@info"}};

const std::vector<std::string> emptyParameterNames;
const std::vector<bool> emptyParameterModifiables;
const std::vector<Variant> emptyParameterDefaults;

FromFileModule::FromFileModule(Program program)
    :_program(program),
      eval(Scope(), *this)
{
    UNUSED(hmcElemNames);
}

void FromFileModule::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    if(program().isValid())
    {
        Program formatDetections = program().node(0);
        loadFormatDetections(formatDetections, formatAdder);
    }
}

void FromFileModule::requestImportations(std::vector<std::string> &formatRequested)
{
    if(program().isValid())
    {
        Program imports = program().node(1);
        loadImports(imports, formatRequested);
    }
}

bool FromFileModule::doLoad()
{
    if(!program().isValid())
        return false;

    Program classDeclarations = program().node(2);
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

    if(definition.node(0).size() == 0)
        return nullptr;

    return new FromFileParser(object, fromModule, definition, headerEnd(name));
}

int64_t FromFileModule::doGetFixedSize(const ObjectType &type, const Module &module) const
{
    const std::string& name = type.typeTemplate().name();
    auto alreadyIt = _fixedSizes.find(name);
    if(alreadyIt != _fixedSizes.end())
    {
        return alreadyIt->second;
    }

    auto definitionIt = _definitions.find(name);
    if(definitionIt == _definitions.end())
    {
        return -1;
    }

    Program definition = definitionIt->second;

    if(sizeDependency(name))
    {
        std::cerr<<type.typeTemplate().name()<<" unknown size"<<std::endl;
        _fixedSizes[type.typeTemplate().name()] = -1;
        return -1;
    }

    if(module.getFather(type).isNull())
    {
        int64_t size = guessSize(definition);
        if(size>0)
        {
            std::cerr<<type.typeTemplate().name()<<" guessed size "<<size<<std::endl;
            _fixedSizes[type.typeTemplate().name()] = size;
            return size;
        }
    }

    std::cerr<<type.typeTemplate().name()<<" father's size"<<std::endl;
    _fixedSizes[type.typeTemplate().name()] = 0;
    return 0;
}

bool FromFileModule::doCanHandleFunction(const std::string &name) const
{
    return _functions.find(name) != _functions.end();
}

Variable FromFileModule::doExecuteFunction(const std::string &name, Scope &params, const Module &fromModule) const
{
    auto it = functionDescriptor(name);

    if(it == _functionDescriptors.end())
        return Variable();

    CompositeScope scope;
    LocalScope localScope;

    scope.addScope(localScope);
    scope.addScope(params);

    const Program& definition = std::get<3>(it->second);
    Evaluator eval(scope, fromModule);
    BlockExecution blockExecution(definition, eval, scope, nullptr);

    blockExecution.execute();

    return blockExecution.returnValue();
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
        switch(formatDetection.node(0).payload().toInteger())
        {
            case ADD_MAGIC_NUMBER_OP:
                formatAdder.addMagicNumber(formatDetection.node(1).payload().toString());
                break;

            case ADD_EXTENSION_OP:
                formatAdder.addExtension(formatDetection.node(1).payload().toString());
                break;

            case ADD_SYNCBYTE_OP:
                formatAdder.addSyncbyte(formatDetection.node(0).payload().toInteger(), formatDetection.node(1).payload().toInteger());
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
    std::cerr<<"Load templates :"<<std::endl;


    for(Program declaration : declarations)
    {
        if(declaration.tag() == CLASS_DECLARATION)
        {
            Program typeTemplate = declaration.node(0).node(0);
            const std::string& name = typeTemplate.node(0).payload().toString();
            std::vector<std::string> parameters;
            for(Program arg:typeTemplate.node(1))
            {
                parameters.push_back(arg.payload().toString());
            }
            const ObjectTypeTemplate& temp = newTemplate(name, parameters);
            std::cerr<<"    "<<temp<<std::endl;

            Program classDefinition = declaration.node(1);
            _definitions[name] = classDefinition;
        }
        else if(declaration.tag() == FUNCTION_DECLARATION)
        {
            const std::string& name = declaration.node(0).payload().toString();
            _functions[name] = declaration;
        }
    }

    std::cerr<<std::endl;
}

void FromFileModule::loadExtensions(Program &classDeclarations)
{
    std::cerr<<"Load extensions :"<<std::endl;

    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.tag() == CLASS_DECLARATION)
        {
            const std::string& name = classDeclaration.node(0).node(0).node(0).payload().toString();
            const ObjectTypeTemplate& childTemplate = getTemplate(name);

            //Choose extension program
            //By defaut "extension"
            Program extension = classDeclaration.node(0).node(1);
            //If there is no "extension" then "specification" is taken
            if(extension.size() == 0)
            {
                extension = classDeclaration.node(0).node(2);
                if(extension.size() == 0)
                    continue;
            }

           //Adding extension lambda
           Program program = extension.node(0);
           setExtension(childTemplate,[this, program](const ObjectType& type)
           {
               return Evaluator(TypeScope(type), *this).type(program);
           });

           std::cerr<<"    "<<childTemplate<<" extends "<<program.node(0).payload()<<"(...)"<<std::endl;
        }
    }

    std::cerr<<std::endl;
}

void FromFileModule::loadSpecifications(Program &classDeclarations)
{
    std::cerr<<"Load specifications :"<<std::endl;

    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.tag() == CLASS_DECLARATION)
        {
            ObjectType child = getTemplate(classDeclaration.node(0).node(0).node(0).payload().toString())();
            for(Program type : classDeclaration.node(0).node(2))
            {
                ObjectType parent(eval.type(type));
                setSpecification(parent, child);
                std::cerr<<"    "<<child<<" specifies "<<parent<<std::endl;
            }
        }
        else if(classDeclaration.tag() == FORWARD)
        {
            ObjectType parent(eval.type(classDeclaration.node(0)));
            ObjectType child(eval.type(classDeclaration.node(1)));
            setSpecification(parent, child);
            std::cerr<<"    "<<child<<" specifies "<<parent<<std::endl;
        }
    }

    std::cerr<<std::endl;
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
    std::set<VariablePath> dependencies = variableDependencies(definition, true);

    bool result = dependencies.find(sizeDescriptor) != dependencies.end();
    _sizeDependency[name] = result;
    return result;
}

int64_t FromFileModule::guessSize(const Program &instructions) const
{
    if(instructions.size() == 0)
        return 0;

    int64_t size = 0;
    for(const Program& line : instructions)
    {
        switch(line.tag())
        {
        case EXECUTION_BLOCK:
            guessSize(line);
            break;

        case DECLARATION:
            {
                if(!variableDependencies(line.node(0),false).empty())
                    return -1;

                ObjectType type = eval.rightValue(line.node(0)).cvalue().toObjectType();
                if(type.isNull())
                    return -1;

                int64_t elemSize = getFixedSize(type);
                if(elemSize == -1)
                    return -1;

                size += elemSize;
                break;
            }

        case LOOP:
        case DO_LOOP:
            if(guessSize(line.node(1)) != 0)
                return -1;
            break;

        case CONDITIONAL_STATEMENT:
            {
                int64_t size1 = guessSize(line.node(1));
                if(size1 == -1)
                    return -1;
                int64_t size2 = guessSize(line.node(2));
                if(size2 == -1 ||size2 != size1)
                    return -1;
                size += size1;
                break;
            }

        case RIGHT_VALUE:
        case LOCAL_DECLARATION:
            break;

        default:
            return -1;
        }
    }
    return size;
}

Program::const_iterator FromFileModule::headerEnd(const std::string &name) const
{
    auto alreadyIt = _headerEnd.find(name);
    if(alreadyIt != _headerEnd.end())
        return alreadyIt->second;

    Program bodyBlock = _definitions.find(name)->second.node(0);
    Program::const_reverse_iterator reverseHeaderEnd = bodyBlock.rbegin();

    for(;reverseHeaderEnd != bodyBlock.rend(); ++reverseHeaderEnd)
    {
        const Program& line = *reverseHeaderEnd;

        //Check dependencies
        std::set<VariablePath> variableSet = variableDependencies(line, true);

        if(std::any_of(headerOnlyVars.begin(), headerOnlyVars.end(), [&variableSet](const VariablePath& headerOnlyVar) -> bool
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
        if(line.tag() == DECLARATION)
        {
            if(line.node(2).payload().toInteger())
                break;
        }
    }

    Program::const_iterator headerEnd = bodyBlock.begin();
    std::advance(headerEnd, std::distance(reverseHeaderEnd, bodyBlock.rend()));
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
    const Program& arguments = declaration.node(1);
    const Program& definition = declaration.node(2);

    std::vector<std::string> parameterNames;
    std::vector<bool> parameterModifiables;
    std::vector<Variant> parameterDefaults;

    for(const Program& argument: arguments)
    {
        parameterNames.push_back(argument.node(1).payload().toString());
        parameterModifiables.push_back(argument.node(0).payload().toBool());
        parameterDefaults.push_back(eval.rightValue(argument.node(2)).cvalue());
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


void FromFileModule::buildDependencies(const Program &instructions, bool modificationOnly, std::set<VariablePath> &descriptors)
{
    switch(instructions.tag())
    {
        case EXECUTION_BLOCK:
        case CONDITIONAL_STATEMENT:
        case LOOP:
        case DO_LOOP:
            for(Program elem : instructions)
                buildDependencies(elem, modificationOnly, descriptors);
            break;

        case TYPE:
            for(Program arg : instructions.node(1))
                buildDependencies(arg, modificationOnly, descriptors);
            break;

        case DECLARATION:
            buildDependencies(instructions.node(0), modificationOnly, descriptors);
            break;

        case LOCAL_DECLARATION:
            if(instructions.size()>=2)
                buildDependencies(instructions.node(1), modificationOnly, descriptors);
            break;

        case RIGHT_VALUE:
            if(instructions.node(0).tag() == OPERATOR)
            {
                int op = instructions.node(0).payload().toInteger();
                for(int i = 0; i < operatorParameterCount[op]; ++i)
                {
                    if(!modificationOnly || !((1<<i)&operatorParameterRelease[op]))
                    {
                        buildDependencies(instructions.node(1+i), modificationOnly, descriptors);
                    }
                }
            }
            else if(instructions.node(0).tag() == VARIABLE || instructions.node(0).tag() == TYPE)
            {
                buildDependencies(instructions.node(0), modificationOnly, descriptors);
            }
            break;

        case VARIABLE:
            descriptors.insert(Evaluator().variablePath(instructions));
            break;

        default:
            break;

    }
}


std::set<VariablePath> FromFileModule::variableDependencies(const Program &instructions, bool modificationOnly)
{
    std::set<VariablePath> dependencies;
    buildDependencies(instructions, modificationOnly, dependencies);
    return dependencies;
}
