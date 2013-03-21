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

#include "module.h"

#include "object.h"
#include "objecttypetemplate.h"

#include "iterutil.h"


Module::Module()
    :_loaded(false)
{
}

void Module::import(const Module &module)
{
    _importedModules.push_back(&module);
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

void Module::setExtension(const ObjectTypeTemplate &childTemplate, const ObjectType &parent, const std::map<int, int> &bindings)
{
    setExtension(childTemplate, [parent, bindings](const ObjectType& type)
    {
        ObjectType father = parent;
        for(const auto& binding : bindings)
        {
            father.setParameter(binding.second, type.parameterValue(binding.first));
        }
        return father;
    });
}



ObjectType Module::getFather(const ObjectType &child) const
{
    //Searching locally
    auto it = _extensions.find(&child.typeTemplate());
    if(it != _extensions.end())
    {
        ObjectType father = (it->second)(child);
        if(!father.isNull())
        {
            return father;
        }
    }

    //Searching in imported modules
    for(const Module* module: reverse(_importedModules))
    {
        ObjectType father = module->getFather(child);
        if(!father.isNull())
        {
            return father;
        }
    }
    return ObjectType();
}

ObjectType Module::specify(const ObjectType &parent) const
{
    ObjectType child = specifyLocally(parent);

    if(child.isNull())
    {
        for(const Module* module : reverse(_importedModules))
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
    //Building the father list
    std::list<ObjectType> fathers;
    ObjectType currentType = type;
    while(currentType.typeTemplate() != lastType.typeTemplate() && !currentType.isNull())
    {
        fathers.push_front(currentType);
        currentType = fromModule.getFather(currentType);
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
        addParsers(object, specification, fromModule, object.type());
    }
}

void Module::setSpecification(const ObjectType& parent, const ObjectType& child)
{
    const ObjectType* parentPtr = new ObjectType(parent);
    _automaticSpecifications.insert(std::make_pair(parentPtr, child));
}

Object* Module::handle(const ObjectType& type, File& file, Object *parent) const
{
    return handle(type, file, parent, *this);
}


bool Module::canHandle(const ObjectType &type) const
{
    if(hasParser(type))
        return true;
    for(std::list<const Module*>::const_reverse_iterator it = _importedModules.rbegin();
        it != _importedModules.rend();
        ++it)
    {
        if((*it)->canHandle(type))
            return true;
    }
    return false;
}

const Module *Module::handler(const ObjectType &type) const
{
    if(hasParser(type))
        return this;

    for(const Module* module : reverse(_importedModules))
    {
        if(module->canHandle(type))
            return module->handler(type);
    }
    return nullptr;
}

Object* Module::handle(const ObjectType& type, File& file, Object* parent, const Module& fromModule) const
{
    Object* object = new Object(file);
    if(parent != nullptr)
        object->_parent = parent;
    addParsers(*object, type, fromModule);
    return object;
}

const ObjectTypeTemplate& Module::getTemplate(const std::string &name) const
{
    const auto it = _templates.find(name);

    if(it != _templates.end())
        return *it->second;

    for(const Module* module: reverse(_importedModules))
    {
        const ObjectTypeTemplate& temp = module->getTemplate(name);
        if(!temp.isNull())
            return temp;
    }

    return nullTypeTemplate;
}

bool Module::hasTemplate(const std::string& name) const
{
    return _templates.find(name) != _templates.end();
}

int64_t Module::getFixedSize(const ObjectType &type) const
{
    const Module* module = handler(type);

    if(module == nullptr)
        return -1;

    int64_t size = module->doGetFixedSize(type, *this);

    if(size == 0)
        return getFixedSize(getFather(type));

    return size;
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

Parser *Module::getParser(const ObjectType &/*type*/, Object &/*object*/, const Module &/*fromModule*/) const
{
    return nullptr;
}

bool Module::hasParser(const ObjectType &/*type*/) const
{
    return false;
}

int64_t Module::doGetFixedSize(const ObjectType &/*type*/, const Module &/*module*/) const
{
    return -1;
}

void Module::addTemplate(const ObjectTypeTemplate& typeTemplate)
{
    _templates[typeTemplate.name()] = &typeTemplate;
}

const ObjectTypeTemplate &Module::newTemplate(const std::string &name)
{
    return newTemplate(name, std::vector<std::string>());
}

const ObjectTypeTemplate &Module::newTemplate(const std::string &name, const std::vector<std::string> &parameters)
{
    ObjectTypeTemplate* temp = new ObjectTypeTemplate(name, parameters);
    _ownedTemplates.push_back(std::unique_ptr<ObjectTypeTemplate>(temp));
    addTemplate(*temp);
    return *temp;
}
