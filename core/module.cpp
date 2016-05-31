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

#include "core/module.h"
#include "core/object.h"
#include "core/objecttypetemplate.h"
#include "core/util/iterutil.h"
#include "core/parser.h"
#include "core/log/logmanager.h"
#include "core/modules/default/defaultmodule.h"

const std::vector<std::string> emptyParameterNames;
const std::vector<bool> emptyParameterModifiables;
const std::vector<Variant> emptyParameterDefaults;

Module::Module()
    :_loaded(false)
{
}

void Module::import(const Module &module, const std::string& name)
{
    _importedModulesChain.insert(_importedModulesChain.begin(), &module);
    _importedModulesMap[name] = &module;
}

const Module &Module::getImportedModule(const std::string &name) const
{
    return *_importedModulesMap.find(name)->second;
}

bool Module::isExtension(const ObjectType& child, const ObjectType& parent) const
{
    if(child.extendsDirectly(parent))
        return true;

    ObjectType father = getFather(child);

    if(!father.isNull())
        return isExtension(father, parent);

    return false;
}

void Module::setExtension(const ObjectTypeTemplate &childTemplate, const std::function<ObjectType (const ObjectType &)> &parentFunction)
{
    _extensions.insert(std::make_pair(&childTemplate, parentFunction));
}

void Module::setExtension(const ObjectTypeTemplate &childTemplate, const ObjectType &parent)
{
    setExtension(childTemplate, [parent](const ObjectType& type)
    {
        return parent;
    });
}

void Module::setExtension(const ObjectTypeTemplate &childTemplate, const ObjectType &parent, const std::map<int, int> &parameterMapping)
{
    setExtension(childTemplate, [parent, parameterMapping](const ObjectType& type)
    {
        ObjectType father = parent;
        for(const auto& binding : parameterMapping)
        {
            father.setParameter(binding.second, type.parameterValue(binding.first));
        }
        return father;
    });
}



ObjectType Module::getFather(const ObjectType &child) const
{
    //Searching locally
    {
        auto father = getFatherLocally(child);
        if (!father.isNull()) {
            return father;
        }
    }

    //Searching in imported modules
    for(const Module* module: _importedModulesChain)
    {
        ObjectType father = module->getFather(child);
        if(!father.isNull())
        {
            return father;
        }
    }

    return ObjectType();
}

ObjectType Module::getFatherLocally(const ObjectType &child) const
{
    return child.parent();
}

ObjectType Module::specify(const ObjectType &parent) const
{
    ObjectType child = specifyLocally(parent);

    if(child.isNull())
    {
        for(const Module* module : _importedModulesChain)
        {
            child = module->specify(parent);
            if(!child.isNull())
                return child;
        }
    }
    return child;
}


ObjectType Module::specifyLocally(const ObjectType& parent) const
{
    ObjectType type;
    ObjectType rangeBegin(parent.typeTemplate());
    for(SpecificationMap::const_iterator it = _automaticSpecifications.lower_bound(&rangeBegin);
        it != _automaticSpecifications.end() && it->first->typeTemplate() == parent.typeTemplate();
        ++it)
    {
        if(parent.extendsDirectly(*(it->first)))
        {
            type = it->second;
            type.importParameters(parent);
            break;
        }
    }
    return type;
}

void Module::addParsers(Object &object, const ObjectType &type, const Module &fromModule, const ObjectType &lastType) const
{
    addParsersRecursive(object, type, fromModule, lastType);

    const auto& parsers = object._parsers;
    if (std::any_of(parsers.begin(), parsers.end(), [](const std::unique_ptr<Parser>& parser) {
        if (parser) {
            return parser->needTailParsing();
        } else {
            return false;
        }
    })) {
        object.parse();
    };
}

void Module::addParsersRecursive(Object &object, const ObjectType &type, const Module &fromModule, const ObjectType &lastType) const
{
    //Building the father list
    std::list<ObjectType> fathers;
    ObjectType currentType = type;
    while(currentType.typeTemplate() != lastType.typeTemplate() && !currentType.isNull())
    {
        fathers.push_front(currentType);
        currentType = currentType.parent();
    }

    //Adding the fathers' parsers
    for(ObjectType father : fathers)
    {
        object.setType(father);
        const Module* module = handler(father);
        if(module!=nullptr)
        {
            Parser* parser = module->getParser(father, object, fromModule);
            object.addParser(parser);
        }
    }
    //Type specification
    ObjectType specification = specify(object.type());
    if(!specification.isNull())
    {
        addParsersRecursive(object, specification, fromModule, object.type());
    }
}

void Module::setSpecification(const ObjectType& parent, const ObjectType& child)
{
    if (!parent.typeTemplate().isVirtual()) {
        Log::error("Cannot forward ",parent," to ",child," because ",parent.typeTemplate(), " is not virtual ");
    }
    const ObjectType* parentPtr = new ObjectType(parent);
    _automaticSpecifications.insert(std::make_pair(parentPtr, child));
}

Object* Module::handleFile(const ObjectType& type, File &file, VariableCollector &collector) const
{
    return handle(type, file, nullptr, collector, *this);
}

Object* Module::handle(const ObjectType& type, Object &parent) const
{
    return handle(type, parent.file(), &parent, parent.collector(), *this);
}


bool Module::canHandle(const ObjectType &type) const
{
    if(hasParser(type))
        return true;
    for(const Module* module : _importedModulesChain)
    {
        if(module->canHandle(type))
            return true;
    }
    return false;
}

const Module *Module::handler(const ObjectType &type) const
{
    if(hasParser(type))
        return this;

    for(const Module* module : _importedModulesChain)
    {
        if(module->canHandle(type))
            return module->handler(type);
    }
    return nullptr;
}

Object* Module::handle(const ObjectType& type, File& file, Object* parent, VariableCollector &collector, const Module& fromModule) const
{
    Object* object;

    if(parent != nullptr) {
        object = new Object(file, parent->beginningPos() + parent->pos(), parent, collector);
        parent->_lastChild = object;
    } else {
        object = new Object(file, 0, nullptr, collector);
    }

    addParsers(*object, type, fromModule);
    return object;
}

const ObjectTypeTemplate& Module::getTemplate(const std::string &name) const
{
    const auto it = _templates.find(name);

    if(it != _templates.end())
        return *it->second;

    for(const Module* module: _importedModulesChain)
    {
        const ObjectTypeTemplate& temp = module->getTemplate(name);
        if(!temp.isNull())
            return temp;
    }

    return ObjectTypeTemplate::nullTypeTemplate;
}

bool Module::hasTemplate(const std::string& name) const
{
    return _templates.find(name) != _templates.end();
}

int64_t Module::getFixedSize(const ObjectType &type) const
{
    int64_t size = type.fixedSize(*this);

    if(size == HM_PARENT_SIZE)
        return getFixedSize(getFather(type));

    return size;
}

bool Module::canHandleFunction(const std::string& name) const
{
    if(doCanHandleFunction(name))
        return true;

    for(const Module* importedModule: _importedModulesChain)
    {
        if(importedModule->canHandleFunction(name))
            return true;
    }

    return false;
}

const Module *Module::functionHandler(const std::string &name) const
{
    if(doCanHandleFunction(name))
        return this;

    for(const Module* importedModule: _importedModulesChain)
    {
        const Module* result = importedModule->functionHandler(name);
        if(result != nullptr)
            return result;
    }

    return nullptr;
}

Variable Module::executeFunction(const std::string &name, const Variable &params) const
{
    return executeFunction(name, params, *this);
}

void Module::addFormatDetection(StandardFormatDetector::Adder &/*formatAdder*/)
{

}

void Module::requestImportations(std::vector<std::string> &/*formatRequested*/)
{
}

bool Module::doLoad()
{
    return true;
}

bool Module::load()
{
    if(!_loaded)
    {
        _loaded = doLoad();
    }
    return _loaded;
}

Variable Module::executeFunction(const std::string &name, const Variable &params, const Module &fromModule) const
{
    const Module* handlerModule = functionHandler(name);
    if(handlerModule == nullptr)
        return Variable();

    return handlerModule->doExecuteFunction(name, params, fromModule);
}

const std::vector<std::string> &Module::getFunctionParameterNames(const std::string &name) const
{
    const Module* handlerModule = functionHandler(name);
    if(handlerModule == nullptr)
        return emptyParameterNames;

    return handlerModule->doGetFunctionParameterNames(name);
}

const std::vector<bool> &Module::getFunctionParameterModifiables(const std::string &name) const
{
    const Module* handlerModule = functionHandler(name);
    if(handlerModule == nullptr)
        return emptyParameterModifiables;

    return handlerModule->doGetFunctionParameterModifiables(name);
}

const std::vector<Variant> &Module::getFunctionParameterDefaults(const std::string &name) const
{
    const Module* handlerModule = functionHandler(name);
    if(handlerModule == nullptr)
        return emptyParameterDefaults;

    return handlerModule->doGetFunctionParameterDefaults(name);
}

bool Module::isLoaded() const
{
    return _loaded;
}


void Module::addTemplate(const ObjectTypeTemplate& typeTemplate)
{
    _templates[typeTemplate.name()] = &typeTemplate;
}

ObjectTypeTemplate &Module::addTemplate(ObjectTypeTemplate* typeTemplate)
{
    _ownedTemplates.push_back(std::unique_ptr<ObjectTypeTemplate>(typeTemplate));
    _templates[typeTemplate->name()] = typeTemplate;
    return *typeTemplate;
}

ObjectTypeTemplate &Module::newTemplate(const std::string &name, const std::vector<std::string> &parameters)
{
    ObjectTypeTemplate* temp = new ObjectTypeTemplate(name, parameters);
    _ownedTemplates.push_back(std::unique_ptr<ObjectTypeTemplate>(temp));
    addTemplate(*temp);
    return *temp;
}

const Module::ParserGenerator& Module::nullGenerator = []parserLambda{return nullptr;};

void Module::addParser(const std::string &name, const Module::ParserGenerator &parserGenerator)
{
    _map[name] = parserGenerator;
}

void Module::addParser(const std::string &name)
{
    addParser(name, nullGenerator);
    setFixedSize(name, 0);
}

void Module::setFixedSize(const std::string &name, const FixedSizeGenerator &fixedSizeFunction)
{
    _sizes[name] = fixedSizeFunction;
}

void Module::setFixedSize(const std::string &name, int64_t fixedSize)
{
    _sizes[name] = [fixedSize] fixedSizeLambda {return fixedSize;};
}

void Module::setFixedSizeFromArg(const std::string &name, int arg)
{
    _sizes[name] = [arg]fixedSizeLambda {
             if(type.parameterSpecified(arg))
                 return type.parameterValue(arg).toInteger();
             return -1;
    };
}

void Module::addFunction(const std::string &name,
                            const std::vector<std::string> &parameterNames,
                            const std::vector<bool> &parameterModifiables,
                            const std::vector<Variant> &parameterDefaults,
                            const Module::Functor &functor)
{
    _functions[name] = std::make_tuple(parameterNames, parameterModifiables, parameterDefaults, functor);
}

bool Module::hasParser(const ObjectType &type) const
{
    return hasTemplate(type.typeTemplate().name());
}

Parser *Module::getParser(const ObjectType &type, Object &object, const Module &fromModule) const
{
    return type.parseOrGetParser(static_cast<ParsingOption&>(object), fromModule);
}

int64_t Module::doGetFixedSize(const ObjectType &type, const Module &module) const
{
    return type.fixedSize(module);
}

bool Module::doCanHandleFunction(const std::string &name) const
{
    return _functions.find(name) != _functions.end();
}

Variable Module::doExecuteFunction(const std::string &name, const Variable &params, const Module &fromModule) const
{
    auto it = _functions.find(name);

    if(it == _functions.end())
        return Variable();

    const Functor& function = std::get<3>(it->second);

    return function(params, fromModule);
}


const std::vector<std::string> &Module::doGetFunctionParameterNames(const std::string &name) const
{
    auto it = _functions.find(name);

    if(it == _functions.end())
        return emptyParameterNames;

    const std::vector<std::string>& parameterNames = std::get<0>(it->second);
    return parameterNames;
}

const std::vector<bool> &Module::doGetFunctionParameterModifiables(const std::string &name) const
{
    auto it = _functions.find(name);

    if(it == _functions.end())
        return emptyParameterModifiables;

    const std::vector<bool>& parameterNames = std::get<1>(it->second);
    return parameterNames;
}

const std::vector<Variant> &Module::doGetFunctionParameterDefaults(const std::string &name) const
{
    auto it = _functions.find(name);

    if(it == _functions.end())
        return emptyParameterDefaults;

    const std::vector<Variant>& parameterDefaults = std::get<2>(it->second);
    return parameterDefaults;
}

