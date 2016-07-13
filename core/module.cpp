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
    : _loaded(false)
{
}

void Module::import(const Module &module)
{
    if (_importedModulesMap.find(module.name()) == _importedModulesMap.end()) {
        for (const Module* importedModule : reverse(module._importedModulesChain))
        {
            if (_importedModulesMap.find(importedModule->name()) == _importedModulesMap.end()) {
                _importedModulesChain.insert(_importedModulesChain.begin(), importedModule);
                _importedModulesMap[importedModule->name()] = importedModule;
            }
        }
        _importedModulesChain.insert(_importedModulesChain.begin(), &module);
        _importedModulesMap[module.name()] = &module;
    } else {
        Log::error("Module ", module.name(), " has already been imported by ", name());
    }
}

const Module &Module::getImportedModule(const std::string &name) const
{
    return *(_importedModulesMap.find(name)->second);
}

ObjectType Module::specify(const ObjectType &parent) const
{
    if (!parent.typeTemplate().isVirtual()) {
       return ObjectType();
    }

    ObjectType child = specifyLocally(parent);
    if(child.isNull())
    {
        for(const Module* importedModule : _importedModulesChain)
        {
            child = importedModule->specifyLocally(parent);
            if(!child.isNull())
                break;
        }
    }
    Log::error("Forwarding ", parent, " to ", child);
    return child;
}


ObjectType Module::specifyLocally(const ObjectType& parent) const
{
#if 1
    auto it = _specializers.find(const_cast<ObjectTypeTemplate*>(&parent.typeTemplate()));
    if (it == _specializers.end())
    {
        return ObjectType();
    }

    ObjectType type = it->second.specialize(parent);
    if (!type.isNull())
    {
        type.importParameters(parent);
    }

    return type;
#else

    ObjectType type;
    ObjectType rangeBegin = ObjectTypeCreator::Create(parent.typeTemplate());
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
#endif
}
 #if 0
void addParsersRecursive(Object &object, const ObjectType &type, const ObjectType &lastType)
{
    const Module& fromModule = object._fromModule;
    ObjectType specification;

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
        Parser* parser = father.parseOrGetParser(static_cast<ParsingOption&>(object), fromModule);
        object.addParser(parser);
    }
    //Type specification
    specification = fromModule.specify(object.type());

    if (!specification.isNull())
    {
        addParsersRecursive(object, specification, object.type());
    }
}
#endif
void Module::addParsers(Object &object, const ObjectType &type) const
{
    //Building the father list

    ObjectType currentType = type;
    ObjectType lastType;
    while (!currentType.isNull()) {
        std::list<ObjectType> fathers;
        while(currentType.typeTemplate() != lastType.typeTemplate() && !currentType.isNull())
        {
            fathers.push_front(currentType);
            currentType = currentType.parent();
        }

        //Adding the fathers' parsers
        for(ObjectType father : fathers)
        {
            object.setType(father);
            Parser* parser = father.parseOrGetParser(static_cast<ParsingOption&>(object), *this);
            object.addParser(parser);
        }
        //Type specification
        lastType= object.type();
        currentType = specify(lastType);
    }

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



void Module::setSpecification(const ObjectType& parent, const ObjectType& child)
{
    if (!parent.typeTemplate().isVirtual()) {
        Log::error("Cannot forward ",parent," to ",child," because ",parent.typeTemplate().name(), " is not virtual ");
    }
    const ObjectType* parentPtr = new ObjectType(parent);
    _automaticSpecifications.insert(std::make_pair(parentPtr, child));

    auto it = _specializers.find(const_cast<ObjectTypeTemplate*>(&parent.typeTemplate()));
    if (it == _specializers.end()) {
        _specializers.emplace(std::piecewise_construct, std::make_tuple(const_cast<ObjectTypeTemplate* >(&parent.typeTemplate())), std::make_tuple(parent, child));
    } else {
        it->second.forward(parent, child);
    }

}

Object* Module::handleFile(const ObjectType& type, File &file, VariableCollector &collector) const
{
    return handle(type, file, nullptr, collector);
}

Object* Module::handle(const ObjectType& type, Object &parent) const
{
    return handle(type, parent.file(), &parent, parent.collector());
}


bool Module::canHandle(const ObjectType &type) const
{
    if(hasParser(type))
        return true;
    for(const Module* importedModule : _importedModulesChain)
    {
        if(importedModule->hasParser(type))
            return true;
    }
    return false;
}

const Module *Module::handler(const ObjectType &type) const
{
    if(hasParser(type))
        return this;

    for(const Module* importedModule : _importedModulesChain)
    {
        if(importedModule->hasParser(type))
            return importedModule->handler(type);
    }
    return nullptr;
}

Object* Module::handle(const ObjectType& type, File& file, Object* parent, VariableCollector &collector) const
{
    Object* object;

    if(parent != nullptr) {
        object = new Object(file, parent->beginningPos() + parent->pos(), parent, collector, *this);
        parent->_lastChild = object;
    } else {
        object = new Object(file, 0, nullptr, collector, *this);
    }

    addParsers(*object, type);
    return object;
}

const ObjectTypeTemplate& Module::getTemplate(const std::string &name) const
{
    const auto it = _templates.find(name);

    if(it != _templates.end())
        return *it->second;

    for(const Module* importedModule : _importedModulesChain)
    {
        const auto it = importedModule->_templates.find(name);

        if(it != _templates.end())
            return *it->second;
    }

    return ObjectTypeTemplate::nullTypeTemplate;
}

bool Module::hasTemplate(const std::string& name) const
{
    return _templates.find(name) != _templates.end();
}

bool Module::canHandleFunction(const std::string& name) const
{
    if(doCanHandleFunction(name))
        return true;

    for(const Module* importedModule: _importedModulesChain)
    {
        if(importedModule->doCanHandleFunction(name))
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
        if(importedModule->doCanHandleFunction(name))
            return importedModule;
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

ObjectTypeTemplate &Module::addTemplate(ObjectTypeTemplate* typeTemplate)
{
    _ownedTemplates.push_back(std::unique_ptr<ObjectTypeTemplate>(typeTemplate));
    _templates[typeTemplate->name()] = typeTemplate;
    return *typeTemplate;
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

