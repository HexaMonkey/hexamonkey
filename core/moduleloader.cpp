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

#include "moduleloader.h"
#include "hmcmodule.h"
#include "fromfilemodule.h"
#include "defaultmodule.h"

#include "strutil.h"
#include "fileutil.h"

#include "interpreterconstructor.h"
#include "objecttypetemplate.h"

#define loaderLambda () -> Module*

ModuleLoader::ModuleLoader()
{
    addModule(""     , new DefaultModule);
}

const Module &ModuleLoader::loadModule(File &file) const
{
    return loadModule(formatDetector.getFormat(file));
}

const Module &ModuleLoader::loadModule(const std::string &key) const
{

    auto it = modules.find(key);

    if(it != modules.end())
    {
        Module& module = *(it->second);
        if(!module._loaded)
        {
            std::cout<<"load <"<<key<<">"<<std::endl;
            std::vector<std::string> importRequirements;
            module.requestImportations(importRequirements);
            if(!importRequirements.empty())
            {
                for(const std::string& importRequirement : importRequirements)
                {
                    module.import(loadModule(importRequirement));
                }
            }
            else if(!key.empty())
            {
                module.import(loadModule(""));
            }
            module.load();
        }
        return module;
    }

    return loadModule("");
}

void ModuleLoader::addModule(const std::string &key, Module *module)
{
    if (module != nullptr)
    {
        StandardFormatDetector::Adder* adder = formatDetector.newAdder(key);
        module->addFormatDetection(*adder);
        delete adder;
        modules[key] = std::shared_ptr<Module>(module);
    }
}

void ModuleLoader::addFolder(const std::string &folderName, const InterpreterConstructor& interpreterConstructor)
{
    std::vector<std::string> files;
    getDirContent(folderName, files);
    std::set<std::string> selected;
    for(const std::string& file : files)
    {
        std::string ext = extension(file);
        if(ext == "hm")
            selected.insert(file.substr(0, file.size()-3));
        else if(ext == "hmc")
            selected.insert(file.substr(0, file.size()-4));
    }

    for(const std::string& file : selected)
    {
        addModule(file, new FromFileModule(folderName+file, interpreterConstructor.newInstance()));
    }
}


