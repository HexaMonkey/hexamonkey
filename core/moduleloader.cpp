#include "moduleloader.h"
#include "mp4module.h"
#include "ebmlmodule.h"
#include "hmcmodule.h"
#include "mkvmodule.h"
#include "tsmodule.h"
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


