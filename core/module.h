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

#ifndef MODULE_H
#define MODULE_H

#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <memory>

#include "objecttype.h"
#include "objecttypetemplate.h"
#include "standardformatdetector.h"

class Object;
class Parser;
class Variable;
class Scope;

/*!
 * @brief The Module class
 */
class Module
{
public:
    Module();

    void import(const Module& module);

    bool isExtension(const ObjectType& child, const ObjectType& parent) const;
    void setExtension(const ObjectTypeTemplate& childTemplate, const std::function<ObjectType(const ObjectType &)> &parentFunction);
    void setExtension(const ObjectTypeTemplate& childTemplate, const ObjectType& parent);
    void setExtension(const ObjectTypeTemplate& childTemplate, const ObjectType& parent, const std::map<int, int> &bindings);
    ObjectType getFather(const ObjectType& child) const;

    ObjectType specify(const ObjectType& parent) const;
    void setSpecification(const ObjectType& parent, const ObjectType& child);

    Object* handle(const ObjectType& type, File& file, Object* parent = nullptr) const;
    bool  canHandle(const ObjectType& type) const;
    const Module* handler(const ObjectType &type) const;

    const ObjectTypeTemplate& getTemplate(const std::string& name) const;
    bool hasTemplate(const std::string& name) const;
    int64_t getFixedSize(const ObjectType& type) const;

    bool canHandleFunction(const std::string &name) const;
    const Module *functionHandler(const std::string &name) const;
    Variable* executeFunction(const std::string &name, Scope &params) const;
    const std::vector<std::string>& getFunctionParameterNames(const std::string& name) const;
    const std::vector<bool>& getFunctionParameterModifiables(const std::string& name) const;
    const std::vector<Variant>& getFunctionParameterDefaults(const std::string& name) const;

protected:
    virtual void addFormatDetection(StandardFormatDetector::Adder& formatAdder);
    virtual void requestImportations(std::vector<std::string>& formatRequested);
    virtual bool doLoad();

    virtual Parser* getParser(const ObjectType &type, Object& object, const Module& fromModule) const;
    virtual bool hasParser(const ObjectType &type) const;
    virtual int64_t doGetFixedSize(const ObjectType& type, const Module& module) const;

    virtual bool doCanHandleFunction(const std::string& name) const;
    virtual Variable* doExecuteFunction(const std::string& name, Scope &params, const Module &fromModule) const;
    virtual const std::vector<std::string>& doGetFunctionParameterNames(const std::string& name) const;
    virtual const std::vector<bool>& doGetFunctionParameterModifiables(const std::string& name) const;
    virtual const std::vector<Variant>& doGetFunctionParameterDefaults(const std::string& name) const;

    void addTemplate(const ObjectTypeTemplate& typeTemplate);
    const ObjectTypeTemplate& newTemplate(const std::string& name);
    const ObjectTypeTemplate& newTemplate(const std::string& name, const std::vector<std::string>& parameters);

private:
    friend class ModuleLoader;
    template<class T>
    struct UnrefCompare : public std::binary_function<T, T, bool>
    {
        bool operator()(T x, T y) const
        {
            return *x < *y;
        }
    };

    typedef std::map<const ObjectType*, ObjectType, UnrefCompare<const ObjectType*> > SpecificationMap;

    typedef std::map<const ObjectTypeTemplate*, std::function<ObjectType(const ObjectType&)>, UnrefCompare<const ObjectTypeTemplate*> > ExtensionMap;

    bool load();

    ObjectType specifyLocally(const ObjectType& parent) const;
    void addParsers(Object& data, const ObjectType &type, const Module& fromModule, const ObjectType &lastType = ObjectType()) const;

    Object* handle(const ObjectType& type, File& file, Object *parent, const Module& fromModule) const;

    Variable* executeFunction(const std::string& name, Scope &params, const Module& fromModule) const;

    bool _loaded;

    std::list<const Module*> _importedModules;

    ExtensionMap _extensions;

    SpecificationMap _automaticSpecifications;

    std::map<std::string, const ObjectTypeTemplate*> _templates;
    std::list<std::unique_ptr<ObjectTypeTemplate> > _ownedTemplates;
};

#endif // MODULE_H
