#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <map>
#include <string>
#include <functional>
#include <memory>

#include "standardformatdetector.h"

#include "formatdetector.h"

class Module;
class InterpreterConstructor;
/*!
 * \brief The ModuleGenerator class
 */
class ModuleLoader
{
public:
    ModuleLoader();

    const Module& loadModule(File &file) const;
    const Module& loadModule(const std::string& key) const;

    void addModule(const std::string& key, Module* module);
    void addFolder(const std::string& folderName, const InterpreterConstructor& interpreterConstructor);

private:
    std::map<std::string, std::shared_ptr<Module> > modules;

    StandardFormatDetector formatDetector;

    //Non-copyable
    ModuleLoader(const ModuleLoader&) = delete;
    ModuleLoader& operator=(const ModuleLoader&) = delete;
};

#endif // MODULELOADER_H
