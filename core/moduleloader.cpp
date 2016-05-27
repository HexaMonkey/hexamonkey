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

#include <memory>

#include "core/moduleloader.h"
#include "core/objecttypetemplate.h"
#include "core/interpreter/fromfilemodule.h"
#include "core/interpreter/programloader.h"
#include "core/modules/default/defaultmodule.h"
#include "core/modules/hmc/hmcmodule.h"
#include "core/util/fileutil.h"
#include "core/util/strutil.h"

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
    if (key == "bestd") {
        Log::warning("Standard module bestd is deprecated, all object are now directly available in default module");
        Log::warning("Endianness is handled by adding @endianness = \"bigEndian\"; in the definition of the root object");
    }

    if (key == "lestd") {
        Log::warning("Standard module lestd is deprecated, all object are now directly available in default module");
        Log::warning("Endianness is handled by adding @endianness = \"littleEndian\"; in the definition of the root object");
    }

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
                    module.import(getModule(importRequirement), importRequirement);
                }
            }
            else if(!key.empty())
            {
                module.import(getModule(""), "");
            }
            Log::info("Load module : <", key, ">");
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

void ModuleLoader::setDirectories(const std::vector<std::string> &directories, const ProgramLoader &programLoader)
{
    this->directories = directories;
    refreshDirectories(programLoader);
}

void ModuleLoader::refreshDirectories(const ProgramLoader &programLoader)
{
    std::unordered_map<std::string, std::string> selected;

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


