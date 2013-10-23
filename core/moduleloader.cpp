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

#include "programloader.h"
#include "objecttypetemplate.h"

#include <memory>

#define loaderLambda () -> Module*

ModuleLoader::ModuleLoader()
{
    addModule("", new DefaultModule);
}

const Module &ModuleLoader::getModule(File &file) const
{
    return getModule(formatDetector.getFormat(file));
}

const Module &ModuleLoader::getModule(const std::string &key) const
{

    auto it = modules.find(key);

    if(it != modules.end())
    {
        Module& module = *(it->second);
        if(!module._loaded)
        {

            std::vector<std::string> importRequirements;
            module.requestImportations(importRequirements);
            if(!importRequirements.empty())
            {
                for(const std::string& importRequirement : importRequirements)
                {
                    module.import(getModule(importRequirement));
                }
            }
            else if(!key.empty())
            {
                module.import(getModule(""));
            }
            std::cerr<<"[ModuleLoader::getModule] Load module : <"<<key<<">"<<std::endl;
            module.load();
        }
        return module;
    }

    return getModule("");
}

void ModuleLoader::addModule(const std::string &key, Module *module)
{
    if (module != nullptr)
    {
        std::unique_ptr<StandardFormatDetector::Adder> adder(formatDetector.newAdder(key));
        module->addFormatDetection(*adder);
        modules[key].reset(module);
    }
}

void ModuleLoader::addDirectories(const std::vector<std::string> &directories, const ProgramLoader &programLoader)
{
    std::map<std::string, std::string> selected;

    for(const std::string& directory : directories)
    {
        std::vector<std::string> files;
        getDirContent(directory, files);

        for(const std::string& file : files)
        {
            std::string ext = extension(file);
            if(ext == "hm" || ext == "hmc")
            {
                std::string key = file.substr(0, file.size()-ext.size()-1);
                selected[key] = directory+key;
            }
        }
    }

    for(const auto& entry: selected)
    {
        addModule(entry.first, new FromFileModule(programLoader.fromFile(entry.second)));
    }
}


