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

#include "compiler/model.h"
#include "core/interpreter/fromfilemodule.h"
#include "core/interpreter/programloader.h"
#include "core/variable/variablepath.h"
#include "core/variable/variable.h"
#include "core/interpreter/fromfileparser.h"
#include "core/interpreter/blockexecution.h"
#include "core/variable/functionscope.h"
#include "core/variable/localscope.h"
#include "core/variable/typescope.h"
#include "core/util/fileutil.h"
#include "core/util/unused.h"

//#define LOAD_TRACE 1

const VariablePath sizeDescriptor = {"@size"};
const std::vector<VariablePath> headerOnlyVars = {
    sizeDescriptor,
    {"@args"},
    {"@value"},
    {"@info"},
    {"@linkTo"},
    {"@attr"}
};

const std::vector<std::string> emptyParameterNames;
const std::vector<bool> emptyParameterModifiables;
const std::vector<Variant> emptyParameterDefaults;

FromFileModule::FromFileModule(Program program)
    :_program(program),
      eval(*this)
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

    return new FromFileParser(object, fromModule, definition, headerEnd(name), needTailParsing(name));
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
        return HM_UNKNOWN_SIZE;
    }

    Program definition = definitionIt->second;

    if(sizeDependency(name))
    {
#ifdef LOAD_TRACE
        std::cerr<<type.typeTemplate().name()<<" unknown size"<<std::endl;
#endif
        _fixedSizes[type.typeTemplate().name()] = HM_UNKNOWN_SIZE;
        return HM_UNKNOWN_SIZE;
    }

    if(module.getFather(type).isNull())
    {
        int64_t size = guessSize(definition);
        if(size>=0)
        {
#ifdef LOAD_TRACE
            std::cerr<<type.typeTemplate().name()<<" guessed size "<<size<<std::endl;
#endif
            _fixedSizes[type.typeTemplate().name()] = size;
            return size;
        }
    }
#ifdef LOAD_TRACE
    std::cerr<<type.typeTemplate().name()<<" father's size"<<std::endl;
#endif

    _fixedSizes[type.typeTemplate().name()] = HM_PARENT_SIZE;
    return HM_PARENT_SIZE;
}

bool FromFileModule::doCanHandleFunction(const std::string &name) const
{
    return _functions.find(name) != _functions.end();
}

Variable FromFileModule::doExecuteFunction(const std::string &name, const Variable &params, const Module &fromModule) const
{
    auto it = functionDescriptor(name);

    if(it == _functionDescriptors.end())
        return Variable();

    Variable scope(new LocalScope(params), true);

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
            case HMC_ADD_MAGIC_NUMBER_OP:
                formatAdder.addMagicNumber(formatDetection.node(1).payload().toString());
                break;

            case HMC_ADD_EXTENSION_OP:
                formatAdder.addExtension(formatDetection.node(1).payload().toString());
                break;

            case HMC_ADD_SYNCBYTE_OP:
                formatAdder.addSyncbyte(formatDetection.node(1).payload().toInteger(), formatDetection.node(2).payload().toInteger());
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
#ifdef LOAD_TRACE
    std::cerr<<"Load templates :"<<std::endl;
#endif

    for(Program declaration : declarations)
    {
        if(declaration.tag() == HMC_CLASS_DECLARATION)
        {
            Program classInfo = declaration.node(0);
            Program typeTemplate = classInfo.node(0);
            const std::string& name = typeTemplate.node(0).payload().toString();
            std::vector<std::string> parameters;
            for(Program arg:typeTemplate.node(1))
            {
                parameters.push_back(arg.payload().toString());
            }
            ObjectTypeTemplate& objectTypeTemplate = newTemplate(name, parameters);
#ifdef LOAD_TRACE
            std::cerr<<"    "<<objectTypeTemplate<<std::endl;
#endif

            Program typeAttributes = declaration.node(0).node(3);
            for (Program typeAttribute : typeAttributes) {
                const std::string& name = typeAttribute.node(0).payload().toString();
                Program program = typeAttribute.node(1);
                auto generator = [this, program]objectTypeAttributeLambda
                {
                    return Evaluator(Variable(new TypeScope(type), false), *this).rightValue(program).value();
                };

                if (name == "elemType") {
                    objectTypeTemplate.setElementTypeGenerator(generator);
                } else if (name == "elemCount") {
                    objectTypeTemplate.setElementCountGenerator(generator);
                }
            }

            Program classDefinition = declaration.node(1);
            _definitions[name] = classDefinition;
        }
        else if(declaration.tag() == HMC_FUNCTION_DECLARATION)
        {
            const std::string& name = declaration.node(0).payload().toString();
            _functions[name] = declaration;
        }
    }
#ifdef LOAD_TRACE
    std::cerr<<std::endl;
#endif
}

void FromFileModule::loadExtensions(Program &classDeclarations)
{
#ifdef LOAD_TRACE
    std::cerr<<"Load extensions :"<<std::endl;
#endif

    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.tag() == HMC_CLASS_DECLARATION)
        {
            Program classInfo = classDeclaration.node(0);
            const std::string& name = classInfo.node(0).node(0).payload().toString();
            const ObjectTypeTemplate& childTemplate = getTemplate(name);

            //Choose extension program
            //By defaut "extension"
            Program extension = classInfo.node(1);
            //If there is no "extension" then "specification" is taken
            if(extension.size() == 0)
            {
                extension = classInfo.node(2);
                if(extension.size() == 0)
                    continue;
            }

           //Adding extension lambda
           Program program = extension.node(0);
           setExtension(childTemplate,[this, program](const ObjectType& type)
           {
               return Evaluator(Variable(new TypeScope(type), false), *this).type(program);
           });
#ifdef LOAD_TRACE
           std::cerr<<"    "<<childTemplate<<" extends "<<program.node(0).payload()<<"(...)"<<std::endl;
#endif
        }
    }
#ifdef LOAD_TRACE
    std::cerr<<std::endl;
#endif
}

void FromFileModule::loadSpecifications(Program &classDeclarations)
{
#ifdef LOAD_TRACE
    std::cerr<<"Load specifications :"<<std::endl;
#endif

    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.tag() == HMC_CLASS_DECLARATION)
        {
            ObjectType child = getTemplate(classDeclaration.node(0).node(0).node(0).payload().toString())();
            for(Program type : classDeclaration.node(0).node(2))
            {
                ObjectType parent(eval.type(type));
                setSpecification(parent, child);
#ifdef LOAD_TRACE
        std::cerr<<"    "<<child<<" specifies "<<parent<<std::endl;
#endif
            }
        }
        else if(classDeclaration.tag() == HMC_FORWARD)
        {
            ObjectType parent(eval.type(classDeclaration.node(0)));
            ObjectType child(eval.type(classDeclaration.node(1)));
            setSpecification(parent, child);
#ifdef LOAD_TRACE
        std::cerr<<"    "<<child<<" specifies "<<parent<<std::endl;
#endif
        }
    }
#ifdef LOAD_TRACE
    std::cerr<<std::endl;
#endif
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
    int64_t size = 0;
    for(const Program& line : instructions)
    {
        switch(line.tag())
        {
        case HMC_EXECUTION_BLOCK:
            size += guessSize(line);
            break;

        case HMC_DECLARATION:
            {
                if(!variableDependencies(line.node(0),false).empty())
                    return HM_UNKNOWN_SIZE;

                ObjectType type = eval.rightValue(line.node(0)).value().toObjectType();
                if(type.isNull())
                    return HM_UNKNOWN_SIZE;

                int64_t elemSize = getFixedSize(type);
                if(elemSize == -1)
                    return HM_UNKNOWN_SIZE;

                size += elemSize;
                break;
            }

        case HMC_LOOP:
        case HMC_DO_LOOP:
            if(guessSize(line.node(1)) != 0)
                return HM_UNKNOWN_SIZE;
            break;

        case HMC_CONDITIONAL_STATEMENT:
            {
                int64_t size1 = guessSize(line.node(1));
                if(size1 == -1)
                    return HM_UNKNOWN_SIZE;
                int64_t size2 = guessSize(line.node(2));
                if(size2 == -1 ||size2 != size1)
                    return HM_UNKNOWN_SIZE;
                size += size1;
                break;
            }

        case HMC_RIGHT_VALUE:
        case HMC_LOCAL_DECLARATION:
            break;

        default:
            return HM_UNKNOWN_SIZE;
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

    for (Program::const_iterator headerEnd = bodyBlock.begin(); headerEnd != bodyBlock.end(); ++headerEnd) {
        const Program& line = *headerEnd;
        //Check header mark
        if (line.tag() == HMC_HEADER_MARK) {
            return ++headerEnd;
        }
    }

    Program::const_reverse_iterator reverseHeaderEnd = bodyBlock.rbegin();
    for(;reverseHeaderEnd != bodyBlock.rend(); ++reverseHeaderEnd)
    {
        if(checkHeaderOnlyVar(*reverseHeaderEnd)) {
            break;
        }
    }

    Program::const_iterator headerEnd = bodyBlock.begin();
    std::advance(headerEnd, std::distance(reverseHeaderEnd, bodyBlock.rend()));
    _headerEnd[name] = headerEnd;
    return headerEnd;
}

bool FromFileModule::needTailParsing(const std::string &name) const
{
    auto alreadyIt = _needTailParsing.find(name);
    if(alreadyIt != _needTailParsing.end())
        return alreadyIt->second;

    Program tailBlock = _definitions.find(name)->second.node(1);

    bool result = false;
    for(Program line : tailBlock)
    {
        if(checkHeaderOnlyVar(line)) {
            result = true;
        }
    }

    _needTailParsing[name] = result;
    return result;
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
        parameterDefaults.push_back(eval.rightValue(argument.node(2)).value());
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


void FromFileModule::buildDependencies(const Program &instructions, bool modificationOnly, std::set<VariablePath> &descriptors, bool areVariablesModified)
{
    switch(instructions.tag())
    {
        case HMC_EXECUTION_BLOCK:
        case HMC_CONDITIONAL_STATEMENT:
        case HMC_LOOP:
        case HMC_DO_LOOP:
            for(Program elem : instructions) {
                buildDependencies(elem, modificationOnly, descriptors);
            }
            break;

        case HMC_TYPE:
            for(Program arg : instructions.node(1)) {
                buildDependencies(arg, modificationOnly, descriptors);
            }
            break;

        case HMC_DECLARATION:
            buildDependencies(instructions.node(0), modificationOnly, descriptors);
            break;

        case HMC_LOCAL_DECLARATIONS:
            for (const Program& localDeclaration : instructions) {
                if(localDeclaration.size()>=2) {
                    buildDependencies(localDeclaration.node(1), modificationOnly, descriptors);
                }
            }
            break;

        case HMC_RIGHT_VALUE:
            if(instructions.node(0).tag() == HMC_OPERATOR) {
                int op = instructions.node(0).payload().toInteger();
                for(int i = 0; i < operatorParameterCount[op]; ++i) {
                    buildDependencies(instructions.node(1+i), modificationOnly, descriptors, areVariablesModified || !((1<<i)&operatorParameterRelease[op]));
                }
            } else if(instructions.node(0).tag() == HMC_VARIABLE) {
                buildDependencies(instructions.node(0), modificationOnly, descriptors, areVariablesModified);
            } else if(instructions.node(0).tag() == HMC_TYPE) {
                buildDependencies(instructions.node(0), modificationOnly, descriptors);
            }
            break;

        case HMC_VARIABLE:
            if (!modificationOnly || areVariablesModified) {
                descriptors.insert(Evaluator().variablePath(instructions));
            }
            break;

        default:
            break;

    }
}

bool FromFileModule::checkHeaderOnlyVar(const Program &line) const
{
    //Check dependencies
    std::set<VariablePath> variableSet = variableDependencies(line, true);

    return std::any_of(headerOnlyVars.begin(), headerOnlyVars.end(), [&variableSet](const VariablePath& headerOnlyVar) -> bool
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

    });
}


std::set<VariablePath> FromFileModule::variableDependencies(const Program &instructions, bool modificationOnly)
{
    std::set<VariablePath> dependencies;
    buildDependencies(instructions, modificationOnly, dependencies);
    return dependencies;
}
