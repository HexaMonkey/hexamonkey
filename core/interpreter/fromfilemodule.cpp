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

const VariableDescriptor sizeDescriptor = {"@size"};
const std::vector<VariableDescriptor> headerOnlyVars = {sizeDescriptor,{"@args"},{"@value"}, {"@info"}};

FromFileModule::FromFileModule(const std::string &path, Interpreter* interpreter)
    :_interpreter(interpreter)
{
    UNUSED(hmcElemNames);
    programLoaded = loadProgram(path);
}

void FromFileModule::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    if(programLoaded)
    {
        Program formatDetections = interpreter().program().elem(0);
        loadFormatDetections(formatDetections, formatAdder);
    }
}

void FromFileModule::requestImportations(std::vector<std::string> &formatRequested)
{
    if(programLoaded)
    {
        Program imports = interpreter().program().elem(1);
        loadImports(imports, formatRequested);
    }
}

bool FromFileModule::doLoad()
{
    if(!programLoaded)
        return false;

    Program classDeclarations = interpreter().program().elem(2);
    loadTemplates(classDeclarations);
    loadExtensions(classDeclarations);
    loadSpecifications(classDeclarations);
    addParsers(classDeclarations);

    return true;
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
    interpreter().buildDependencies(definition, true, descriptors);
    if(sizeDependency(name))
    {
        std::cout<<type.typeTemplate().name()<<" unknown size"<<std::endl;
        _fixedSizes[type.typeTemplate().name()] = -1;
        return -1;
    }

    if(module.getFather(type).isNull())
    {
        int64_t size = interpreter().guessSize(definition, module);
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

bool FromFileModule::loadProgram(const std::string path)
{

    std::string hmPath  = path+".hm";
    std::string hmcPath = path+".hmc";

#ifdef USE_QT

    QFileInfo hmInfo(hmPath.c_str());
    QFileInfo hmcInfo(hmcPath.c_str());
    if(!hmInfo.exists())
    {
        if(!hmcInfo.exists())
        {
            std::cerr<<"Description file not found: "<<hmPath<<std::endl;
            return false;
        }
        std::cout<<"Loading existing description file : "<<hmcPath<<std::endl;
        return interpreter().loadFromHMC(hmcPath);
    }
    else
    {
        if(!hmcInfo.exists() || hmcInfo.lastModified() < hmInfo.lastModified())
        {
            std::cout<<"Compiling description file"<<std::endl;
            return interpreter().loadFromHM(hmPath, Interpreter::file);
        }
        std::cout<<"Loading existing description file : "<<hmcPath<<std::endl;
        return interpreter().loadFromHMC(hmcPath);
    }
#else
    if(fileExists(hmPath))
        return interpreter().loadFromHM(hmPath, Interpreter::file);
    else if(fileExists(hmcPath))
        return interpreter().loadFromHMC(hmcPath);
    else
        return false;
#endif
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

void FromFileModule::loadTemplates(Program& classDeclarations)
{
    std::cout<<"Loading templates :"<<std::endl;


    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.id() == CLASS_DECLARATION)
        {
            Program typeTemplate = classDeclaration.elem(0).elem(0);
            const std::string& name = typeTemplate.elem(0).payload().toString();
            std::vector<std::string> parameters;
            for(Program arg:typeTemplate.elem(1))
            {
                parameters.push_back(arg.payload().toString());
            }
            const ObjectTypeTemplate& temp = newTemplate(name, parameters);
            std::cout<<"    "<<temp<<std::endl;
        }
    }

    std::cout<<std::endl;
}

void FromFileModule::loadExtensions(Program &classDeclarations)
{
    std::cout<<"Loading extensions :"<<std::endl;

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

            _nonApexTemplates.insert(name);
           //Adding extension lambda
           Program program = extension.elem(0);
           setExtension(childTemplate,[this, program](const ObjectType& type)
           {
               return interpreter().evaluateType(program, ConstTypeScope(type), *this);
           });

           std::cout<<"    "<<childTemplate<<" extends "<<program.elem(0).payload()<<"(...)"<<std::endl;
        }
    }

    std::cout<<std::endl;
}

void FromFileModule::loadSpecifications(Program &classDeclarations)
{
    std::cout<<"Loading specifications :"<<std::endl;

    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.id() == CLASS_DECLARATION)
        {
            ObjectType child = getTemplate(classDeclaration.elem(0).elem(0).elem(0).payload().toString())();
            for(Program type : classDeclaration.elem(0).elem(2))
            {
                ObjectType parent(interpreter().evaluateType(type, EmptyScope(), *this));
                setSpecification(parent, child);
                std::cout<<"    "<<child<<" specifies "<<parent<<std::endl;
            }
        }
        else if(classDeclaration.id() == FORWARD)
        {
            ObjectType parent(interpreter().evaluateType(classDeclaration.elem(0), EmptyScope(), *this));
            ObjectType child(interpreter().evaluateType(classDeclaration.elem(1),  EmptyScope(), *this));
            setSpecification(parent, child);
            std::cout<<"    "<<child<<" specifies "<<parent<<std::endl;
        }
    }

    std::cout<<std::endl;
}

void FromFileModule::addParsers(Program &classDeclarations)
{
    std::cout<<"Adding parsers :"<<std::endl;

    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.id() == CLASS_DECLARATION)
        {
            Program classDefinition = classDeclaration.elem(1);
            const std::string& name = classDeclaration.elem(0).elem(0).elem(0).payload().toString();
            _definitions[name] = classDefinition;
            std::cout<<"    "<<name<<std::endl;
            if(classDefinition.size() == 0)
            {
                addParser(name);
                std::cout<<"        Empty "<<std::endl;
            }
            else
            {
                addParser(name, [this, classDefinition, name](const ObjectType &type, Object &object, const Module &module) ->Parser*
                {
                    return new FromFileParser(object, module, interpreter(), classDefinition, headerEnd(name));
                });
            }
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
    interpreter().buildDependencies(definition, true, descriptors);
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
        std::set<VariableDescriptor> variableSet;
        interpreter().buildDependencies(*reverseHeaderEnd, true, variableSet);
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
    }

    Program::const_iterator headerEnd = definition.begin();
    std::advance(headerEnd, std::distance(reverseHeaderEnd, definition.rend()));
    _headerEnd[name] = headerEnd;
    return headerEnd;
}

#if 0
void FromFileModule::guessSizes(Program &classDeclarations)
{
    std::cout<<"Guess sizes"<<std::endl;
    for(Program classDeclaration : classDeclarations)
    {
        if(classDeclaration.id() == CLASS_DECLARATION)
        {
            Program classDefinition = classDeclaration.elem(1);
            const std::string& name = classDeclaration.elem(0).elem(0).elem(0).payload().toString();
            if(_nonApexTemplates.find(name) != _nonApexTemplates.end())
                continue;

            int64_t size = interpreter().guessSize(classDefinition, *this);
            if(size>0)
            {
                std::cout<<"    "<<name<<" <-- "<<size<<std::endl;
                setFixedSize(name, size);
            }
        }
    }

    std::cout<<std::endl;
}
#endif

Interpreter &FromFileModule::interpreter() const
{
    return *_interpreter;
}
